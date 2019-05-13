#!/usr/bin/python3

import io
import re
import csv
import subprocess
import sys

import experimentinfo
import atcregex
from trial import Trial
import atctiming


class PartDayConflictResults(object):
    """docstring for PartDayConflictResults."""

    def __init__(self, logfile):
        self.logfile = logfile
        self.day = experimentinfo.fromfilepath(logfile, 'day')
        self.id = experimentinfo.fromfilepath(logfile, 'id')
        self.conflictinfo = ConflictInfo(self.day)
        self.hpctimes = atctiming.HPCtimes(logfile)
        self.loadedtrials = []
        
        self.respondedtrials = []
        self.correct = []
        self.misses = []
        self.early = []
        self.late = []
        self.falsealarms = []

    def parse(self):
        '''
        Main conflict parser. Iterates over the logfile.abs
        If line is trialstring - set the trialinfo, trialid and add to loaded trials
        If line is level variation intervention - parse the conflict with parse_intervention
        '''
        hpctimes = atctiming.HPCtimes(self.logfile)
        log = open(self.logfile, 'r')
        ordernum = 0
        for line in log:
            if atcregex.TRIAL.search(line):  # Pull out trial informationself.trial = Trial(trialname)
                trialstring = atcregex.TRIAL.search(line).group(0)
                scenario = experimentinfo.getscenario(trialstring)
                trial = Trial(trialstring)
                ordernum += 1
                trial.ordernumber = ordernum
                self.loadedtrials.append(trial)

            elif atcregex.INTERVENTION.search(line):  # Pull out conflict information
                callsign = atcregex.CALLSIGN.search(line).group(1)

                if self.conflictinfo.checkvalid(scenario, callsign):
                    conf = self.conflictinfo.getvalidconflict(scenario, callsign)
                    if self.alreadyresponded(conf):
                        pass
                        
                    elif not self.alreadyresponded(conf):
                        conf.recordresponse(line, hpctimes)
                        conf.trialstring = trialstring
                        self.respondedtrials.append(conf)

                if self.conflictinfo.checkvalid(scenario, callsign) == False:
                    fa = FalseAlarm(line)
                    fa.trialstring = trialstring
                    fa.scenario = scenario
                    fa.gettype()
                    self.falsealarms.append(fa)

        self.nonresponsetrials()
            # Then get all non responded trials
            # Then get all false alarms etc
            # Then write out to csv
        log.close()

    def alreadyresponded(self, conflict):
        for conf in self.respondedtrials:
            if conf.pairid == conflict.pairid:
                return True
        return False

    def nonresponsetrials(self):
        responded_pairs = set([conf.pairid for conf in self.respondedtrials])
        for conflict in self.conflictinfo.list:
            if conflict.pairid not in responded_pairs:
                thismissconf = conflict                
                thismissconf.trialstring = [i.trialstring for i in self.loadedtrials if i.scenario == thismissconf.scenario][0]
                thismissconf.t1_ms = self.hpctimes.fromscenario(thismissconf.scenario, thismissconf.t1)
                thismissconf.t2_ms = self.hpctimes.fromscenario(thismissconf.scenario, thismissconf.t2)
                print(conflict.pairid)
                self.misses.append(thismissconf)

    def writeresponses(self, outfilename):
        with open(outfilename, 'a') as w:
            writer = csv.writer(w)
            for conflict in self.respondedtrials:
                writer.writerow([self.id, self.day] + conflict.returntocsv())
            for conflict in self.misses:
                writer.writerow([self.id, self.day] + conflict.returntocsv())
        w.close()
    
    def writefalsealarms(self, outfilename):
        pass



class ConflictInfo(object):
    """docstring for ConflictInfo."""

    def __init__(self, day):
        self.day = day
        self.list = self.getknownconflicts(day)

    def getknownconflicts(self, day):
        """
        Populate list list of known conflicts for Day 1 or Day 2
        """
        if day == 1 or day == '1':
            return self._populateconflicts("settings/confinfo_day1.csv")
        elif day == 2 or day == '2':
            return self._populateconflicts("settings/confinfo_day2.csv")

    def _populateconflicts(self, filename):
        """
        Returns a list of known conflicts, uses the Conflict class.
        """
        conflicts = list()
        with open(filename) as f:
            for row in csv.DictReader(f):
                if row['acnum'] == '1':
                    c = Conflict()
                    c.pairid, c.scenario = row['pair'], row['scenario']
                    c.t1, c.t2 = int(row['t1']), int(row['t2'])
                    c.aircraft = [row['callsign'], row['partner']]
                    c.altitudes[row['callsign']] = int(row['altitude'])
                elif row['acnum'] == '2':
                    c.altitudes[row['callsign']] = int(row['altitude'])
                    c.gettype()
                    conflicts.append(c)
        return conflicts

    def checkvalid(self, scenario, callsign):
        """
        checkvalid takes a scenario and callsign and returns a bool as to whether the conflict is known.
        ci = ConflictInfo("1")
        # A valid conflict
        >>> print(ci.checkvalid("1", "MH75"))
        True
        # An invalid conflict
        >>> print(ci.checkvalid("1", "ZZ77"))
        False
        """
        valid = False
        for conflict in self.list:
            if (conflict.scenario == scenario) and (callsign in conflict.aircraft):
                valid = True
        return valid

    def getvalidconflict(self, scenario, callsign):
        """Takes scenario and callsign for a valid pair and returns the conflict object"""
        for conflict in self.list:
            if (conflict.scenario == scenario) and (callsign in conflict.aircraft):
                return conflict

class FalseAlarm(object):
    """docstring for Conflict."""

    def __init__(self, line):
        # Base falsealarm vars
        self.trialstring = str()
        self.scenario = str()
        self.aircraftchanged = atcregex.CALLSIGN.search(line).group(1)
        self.intervenetime_ms = float(atcregex.MSTIME.search(line).group(1))
        self.intervenetime_s = int(self.intervenetime_ms/1000)
        self.type = str()
        self.falsealarm = 1
        

    def gettype(self):
        '''
        Determine if FalseAlarm post-interruption, overlap or pre-interruption
        Based on t1, t2, interruption time and interruption end time (all in s)
        '''
        intstart = int(experimentinfo.PMINFO[self.scenario]["L"]['interruptiontime'])
        intend = intstart + 27
        if self.intervenetime_s > intend:
            self.type = "post"
        elif self.intervenetime_s < intstart:
            self.type = "pre"
        else:
            self.type = "overlap"
        return self


class Conflict(object):
    """docstring for Conflict."""

    def __init__(self):
        # Base conflict vars
        self.trialstring = str()
        self.pairid = str()
        self.scenario = str()
        self.ordernum = 0
        self.aircraft = []
        self.altitudes = {}
        self.t1 = int()
        self.t2 = int()
        self.type = str()        
        
        # Measure vars gathered implied from a response. Defaults to error.
        self.nonresponse = 1
        self.correct = 0
        self.miss = 1
        self.oldfl = int()
        self.newfl = int()
        self.aircraftchanged = str()
        self.intervenetime_ms = None
        self.t1_ms = float()
        self.t2_ms = float()
        self.rt = None
        self.early = 0
        self.late = 0

    def gettype(self):
        '''
        Determine if conflict post-interruption, overlap or pre-interruption
        Based on t1, t2, interruption time and interruption end time (all in s)
        '''
        intstart = int(experimentinfo.PMINFO[self.scenario]["L"]['interruptiontime'])
        intend = intstart + 27
        if self.t1 > intend:
            self.type = "post"
        elif self.t1 < intstart and self.t2 > (intend):
            self.type = "overlap"
        elif (self.t1 < intstart) and (self.t2 < intstart):
            self.type = "pre"
        return self
    
    def recordresponse(self, interventionline, hpctimes):
        """recordresponse takes a valid intervention line and an hpctimes dict updates all conflict values to reflect the response"""
        # Get type
        self.gettype()
        # Get t1_ms and t2_ms
        self.t1_ms = hpctimes.fromscenario(self.scenario, self.t1)
        self.t2_ms = hpctimes.fromscenario(self.scenario, self.t2)
        # Regex patterns for intervention details
        self.intervenetime_ms = float(atcregex.MSTIME.search(interventionline).group(1))
        self.aircraftchanged = atcregex.CALLSIGN.search(interventionline).group(1)
        self.oldfl = int(atcregex.OLDFL.search(interventionline).group(1))
        self.newfl = int(atcregex.NEWFL.search(interventionline).group(1))
        # Setting the final results
        self.nonresponse = 0
        result = self._intervention_result()
        if result == "correct":
            self.correct = 1
            self.miss = 0
            self.rt = self.intervenetime_ms - self.t1_ms
        if result == "late":
            self.late = 1
        if result == "early":
            self.early = 1

    def _intervention_result(self):
        '''
        Checks if an intervention was correct or performed too late
        '''
        if self.t1_ms <= self.intervenetime_ms <= self.t2_ms:
            return "correct"
        elif self.intervenetime_ms > self.t2_ms:
            return "late"
        elif self.intervenetime_ms < self.t1_ms:
            return "early"

    def returntocsv(self):
        thistrial = Trial(self.trialstring)
        csvline = [self.trialstring, self.scenario, self.pairid, thistrial.interruption, thistrial.encode, thistrial.retrieve,
                   self.type, self.t1_ms, self.t2_ms, self.intervenetime_ms, self.rt, self.correct,
                   self.miss, self.nonresponse, self.late, self.early]
        return csvline


def main():
    ci = PartDayConflictResults(sys.argv[1])
    # ci = PartDayConflictResults("/home/michael/phd/exp3-trainingdata/EXP3-FINLAND-P51-DAY2.log")
    ci.parse()
    ci.writeresponses("tmpdata/conflicts.csv")
    
if __name__ == "__main__":
    main()
