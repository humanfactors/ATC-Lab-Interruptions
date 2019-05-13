import re
import io
import subprocess

import atcregex
from trial import Trial

class HPCtimes(object):
    '''TrialTiming takes a logfile, and is used to return timing information for a given trial for a log file.'''
    def __init__(self, logfile):
        self.dict = dict()
        self.loadedtrials = []
        self.extract(logfile)
        self.trials = [Trial(ts) for ts in self.loadedtrials]
    
    def extract(self, logfile):
        greppedlog = self._grep_logfile(logfile)
        for line in greppedlog:
            if atcregex.TRIAL.search(line):
                trialname = atcregex.TRIAL.search(line).group(0)
                self.loadedtrials.append(trialname)
            if "tick" in line:
                self.add_tick(line, trialname)

    def _grep_logfile(self, logfile):
        '''
        Subprocess to grep to extract level variations (altitude changes)
        and all interruptions and task_ids
        '''
        p = subprocess.Popen(['grep',
                              '-e', r'<task_id>ATC\|<task_id>None',
                              '-e', 'tick',
                              logfile], stdout=subprocess.PIPE)
        return io.TextIOWrapper(p.stdout)


    def add_tick(self, tickline, trialname):
        '''
        Adds a float(tick) to the Timing.dict
        '''
        ticktime = float(re.search(r'<hpc_elapsed_time>(.+?)</hpc_elapsed_time>', tickline).group(1))
        if trialname not in self.dict:
            self.dict[trialname] = [0]
        if self.dict[trialname]:
            self.dict[trialname].append(ticktime)

    def fromscenario(self, scenario, seconds):
        '''
        Returns dict for a given scenario from the timing dict
        '''
        scenario = str(scenario)
        for k, v in self.dict.items():
            sdigits = ''.join(ele for ele in k if ele.isdigit())
            if sdigits == scenario:
                return v[seconds]

    def oftrial(self, trialstring):
        '''
        Returns dict for a given trial string from the timing dict
        '''
        return self.dict[trialstring]
    
    def fromseconds(self, trialstring, seconds):
        '''
        Converts seconds to milliseconds for a given trial
        '''
        seconds = int(seconds)
        return self.oftrial(trialstring)[seconds]



def main():
    t = HPCtimes("/home/michael/phd/exp3-trainingdata/EXP3-FINLAND-P07-DAY1.log")
    print(t.dict)
if __name__ == '__main__':
    main()