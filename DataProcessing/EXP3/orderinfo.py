
import re
import io
import subprocess
import sys
import csv

import atcregex
import experimentinfo
from trial import Trial


def extract(logfile):
    loadedtrials = []
    ordernum = 0
    greppedlog = _grep_logfile(logfile)
    for line in greppedlog:
        if atcregex.TRIAL.search(line):
            ordernum += 1
            trial = Trial(atcregex.TRIAL.search(line).group(0))
            trial.ordernumber = ordernum
            loadedtrials.append(trial)
    return loadedtrials

def _grep_logfile(logfile):
    '''
    Subprocess to grep to extract level variations (altitude changes)
    and all interruptions and task_ids
    '''
    p = subprocess.Popen(['grep',
                            '-e', r'<task_id>ATC\|<task_id>None',
                            logfile], stdout=subprocess.PIPE)
    return io.TextIOWrapper(p.stdout)

def main():
    o = csv.writer(sys.stdout)
    logfile = sys.argv[1]
    loadedtrials = extract(logfile)
    pid = experimentinfo.fromfilepath(logfile, "id")
    day = experimentinfo.fromfilepath(logfile, "day")
    for i in loadedtrials:
        o.writerow([pid, day, i.trialstring, i.ordernumber, i.scenario, i.encode, i.retrieve, i.interruptiontime, i.interruption])

if __name__ == '__main__':
    main()