#!/usr/bin/env python3
import subprocess
import io

import atcregex 

from atctiming import HPCtimes
from trial import Trial
import experimentinfo

class SubjectDay():
    def __init__(self, logfile, csv):
        self.pid = experimentinfo.fromfilepath(logfile, 'id')
        self.day = experimentinfo.fromfilepath(logfile, 'day')
        self.logfile = logfile
        self.csv = None
        self.Timing = HPCtimes(logfile)
        self.trials = []
        # self.process_orderinfo = process_orderinfo(logfile)
        # self.ConflictResults = ConflictResults(logfile, self.HPCtimes)
        # self.HandoffResults = HandoffResults(self.csv)
        # self.RoutineResults = RoutineResults(self.csv)

    def process_orderinfo(self, logfile):
        logfile = self._grep_logfile(logfile)
        loadedtrials = []
        for line in logfile:
            if atcregex.TRIAL.search(line):
                trialname = atcregex.TRIAL.search(line).group(0)
                loadedtrials.append(trialname)
        for i, trialid in enumerate(loadedtrials):
            tmptrial = Trial(trialid)
            tmptrial.ordernumber = i
            self.trials.append(tmptrial)
        return self
        
    def _grep_logfile(self, logfile):
        '''
        Subprocess to grep to extract interruption order and task_ids
        '''
        p = subprocess.Popen(['grep',
                              '-e', r'<task_id>ATC\|<task_id>None',
                              "-e", "Interruption",
                              logfile], stdout=subprocess.PIPE)
        return io.TextIOWrapper(p.stdout)


def main():
    t = SubjectDay(r"/home/michael/phd/exp3-trainingdata/PID_07/Day 1/EXP3-FINLAND-P07-DAY1.log", 
                   r"/home/michael/phd/exp3-trainingdata/PID_07/Day 1/EXP3-FINLAND-P07-DAY1.csv")
    t.process_orderinfo(t.logfile)
    print(vars(t))
    print(vars(t.trials[5]))

if __name__ == "__main__":
    main()
