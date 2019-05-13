#!/usr/bin/env python3
import sys
import subject
import os
import re



    # subjects = []
    # handoffs = csv.writer()
    # conflicts = csv.writer()
    # for file in :
        #do something
        #csv = xxx
        #log = yyy
        #subject.Subject(log, csv)
        # subject.append(subjects)
        # for subject in subjects:
        #   conflicts.writelines(subject.returnconflicts())
        #   handoffs.writelines(subject.returnhandoffs())
        # pass


def isdirectory(arg):
    if not arg:
        raise IOError("Must take in a directory")

def process_partday(csv, log):
    """returns participant object containing all results"""
    print(csv, log)

datadir = sys.argv[1]
isdirectory(datadir)

partdays = []
for partdir in [f.path for f in os.scandir(datadir) if f.is_dir() and f.name.startswith("PID") ]:
    daydirs = [x.path for x in os.scandir(partdir) if x.is_dir()]
    if len(daydirs) < 2:    # for logfiles in os.scandir(:
        continue
    for daydir in daydirs:
        partdays.append(daydir)

results = []
for daydir in partdays:
    logfiles = os.scandir(daydir)
    for logfile in logfiles:
        logsearch = re.search(r"EXP3-FIN.*\.log", logfile.name)
        csvsearch = re.search(r"EXP3-FIN.*\.csv", logfile.name)
        if logsearch:
            log = logfile.path
        if csvsearch:
            csv = logfile.path
    results.append(process_partday(csv, log))

