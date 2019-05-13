#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Handoffs processes a grepped set of logfiles. 
"""
# perl -pi.bak -e 's/^.\/PID_\S+\/Day \d\/EXP3-FINLAND-(\S+)-DAY(\d).csv:/$1,$2,/g'  exp-handoffdata.csv

import sys
import csv
import re
import experimentinfo



'''
Process handoffs from collated CSV; direct CSV to stdout
'''
def process_handoffs(file):
    outwriter = csv.writer(sys.stdout)
    with open(file) as f:
        csvreader = csv.reader(f)
        for line in csvreader:
            handoff = DeferredTrialCase(line)
            # if handoff.check_pm():
                # outwriter.writerow(handoff.process_output_row())
            if handoff.check_pm() == False:
                outwriter.writerow(handoff.process_routine_row())

class DeferredTrialCase(object):
    """A single handoff row from a collated CSV. Will extract PM response info IF it was a PM response"""

    def __init__(self, datarow):
        self.handoffcase = datarow
        self.pid = datarow[0]
        self.day = datarow[1]
        self.daystring = 'day' + self.day
        self.trialstring = datarow[2]
        self.trialinfo = experimentinfo.gettrialinfodict(self.trialstring)
        self.pminfo = experimentinfo.PMINFO[self.trialinfo['scenario']][self.trialinfo['retrieve']]
        self.correctkey = self.pminfo['key']
        # If case is PM, chain the output
        self.check_pm()

    def check_pm(self):
        '''
        Requires trial PM aircraft and a target aircraft;
        Processes PM (and returns true) if PM
        '''
        self.pmaircraftname = self.pminfo[self.daystring]
        if self.pmaircraftname == self.handoffcase[5]:
            self._process_pm(self.handoffcase, 0)
            return True
        elif self.pmaircraftname != self.handoffcase[5]:
            self._process_routine_response(self.handoffcase)
            return False
    
    def _process_routine_response(self, row):
        self.routho = {'correct': 0, 'falsealarm': 0, 'miss' : 0, 
        'flashtime_sec' : row[3], 'flashtime_ms' : float(row[4]), 'responsetime' : None, 'actiontime' : None}
        if row[7] == '1':
            self.routho = {x: "NA" for x in self.routho}
        elif any("Arrow" in x for x in row):
            self.routho['falsealarm'] = 1
            self.routho['actiontime'] = float(row[12])
            self.routho['responsetime'] = float(row[12]) - float(row[4])
        elif row[10] == 'H':
            self.routho["correct"] = 1
            self.routho['actiontime'] = float(row[12])
            self.routho['responsetime'] = float(row[12]) - float(row[4])
        elif row[9] == "-1":
            self.routho['miss'] = 1
        elif row[10] != "H":
            self.find_last_response(row)
        self.routine_performance = [self.routho['correct'], self.routho['falsealarm'], self.routho['miss'], self.routho['flashtime_sec'], self.routho['actiontime'], self.routho['responsetime'] ]
        return self.routine_performance

    def find_last_response(self, row):
        lastitemint = max(loc for loc, val in enumerate(row) if val == 'H')
        if row[lastitemint] == "H":
            self.routho["correct"] = 1
            self.routho['actiontime'] = float(row[lastitemint+2])
            self.routho['responsetime'] = float(row[lastitemint+2]) - float(row[4])
        else:
            pass
    def _process_pm(self, row, resnum):
        '''
        Main extraction function. Returns all relevant performance info for a PM trial;
        Note that due to the expanding length of rows if incorrect key pushed, the function
        does recurse to extend another row. It will only go two levels.
        '''
        self.dh_performance = {
            'correctdo': None,
            'flashtime_sec': self.handoffcase[3],
            'flashtime_ms': self.handoffcase[4],
            'actiontime': None,
            'responsetime': None,
            'procerror': None,
            'nonresponse': None,
            'keypress': self.handoffcase[10]}

        if row[10 + resnum] == self.correctkey:
            self.dh_performance['correctdo'] = 1
            self.dh_performance['actiontime'] = float(row[12 + resnum])
            self.dh_performance['responsetime'] = float(
                row[12 + resnum]) - float(row[4])
            self.dh_performance['procerror'] = 0
            self.dh_performance['nonresponse'] = 0

        elif row[9] == '-1':
            self.dh_performance['correctdo'] = "NA"
            self.dh_performance['actiontime'] = "NA"
            self.dh_performance['responsetime'] = "NA"
            self.dh_performance['procerror'] = "NA"
            self.dh_performance['nonresponse'] = 1

        elif row[10 + resnum] == "H":
            self.dh_performance['correctdo'] = 0
            self.dh_performance['actiontime'] = float(row[12 + resnum])
            self.dh_performance['responsetime'] = float(
                row[12 + resnum]) - float(row[4])
            self.dh_performance['procerror'] = 0
            self.dh_performance['nonresponse'] = 0

        elif "Arrow" in row[10 + resnum] and row[10 + resnum] != self.correctkey:
            self.dh_performance['correctdo'] = "NA"
            self.dh_performance['actiontime'] = float(row[12 + resnum])
            self.dh_performance['responsetime'] = float(
                row[12 + resnum]) - float(row[4])
            self.dh_performance['procerror'] = 1
            self.dh_performance['nonresponse'] = 0

        elif row[10 + resnum] != "H" and row[10 + resnum] != self.correctkey:
            if resnum == 0:
                self._process_pm(row, 3)
            else:
                self.dh_performance['correctdo'] = 0
                self.dh_performance['actiontime'] = float(row[12 + resnum])
                self.dh_performance['responsetime'] = float(
                    row[12 + resnum]) - float(row[4])
                self.dh_performance['procerror'] = 0
                self.dh_performance['nonresponse'] = 0
        # print(self.dh_performance)
        return self.dh_performance

    def process_output_row(self):
        '''
        CSV row variables
        '''
        self.output = [self.pid, self.day, self.trialstring, self.pmaircraftname,
                       self.trialinfo["scenario"], self.trialinfo["interruption"],
                       self.trialinfo["encode"], self.trialinfo["retrieve"],
                       self.trialinfo["trialtype"], self.dh_performance["flashtime_ms"],
                       self.dh_performance["actiontime"], self.dh_performance['keypress'],
                       self.correctkey, self.dh_performance["correctdo"],
                       self.dh_performance["responsetime"], self.dh_performance["procerror"],
                       self.dh_performance["nonresponse"]]
        return self.output

    def process_routine_row(self):
        '''
        CSV row variables
        '''
        self.output = [self.pid, self.day, self.trialstring,
                       self.trialinfo["scenario"], self.trialinfo["interruption"],
                       self.trialinfo["encode"], self.trialinfo["retrieve"],
                       self.trialinfo["trialtype"]] + self.routine_performance
        return self.output

def check_sysargs():
    if len(sys.argv) > 2:
        raise NotImplementedError(
            "Script only takes single file at this point in time")
    if len(sys.argv) == 1:
        raise IOError("Please specify a valid log file as arument")


def main():
    process_handoffs(sys.argv[1])
    # print("processing %s " % sys.argv[1])


if __name__ == '__main__':
    check_sysargs()
    main()
