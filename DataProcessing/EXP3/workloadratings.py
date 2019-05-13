#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
import csv
import re
from glob import iglob, glob
from atcregex import *
from experimentinfo import fromfilepath

def is_workload_line(line):
    if WORKLOAD.match(line[0]) and WORKLOAD.match(line[1]):
        return True

def parse_file_workload(file):

    outwriter = csv.writer(sys.stdout)
    subject = fromfilepath(file).zfill(2)
    day = "2"

    with open(file) as f:
        csvreader = csv.reader(f)
        for line in csvreader:

            if len(line) < 2:
                continue
            if TRIAL.match(line[0]):
                scenario = line[0].replace("ID", "")
            if is_workload_line(line):
                questionid = line[0]
                response = line[1]
                outwriter.writerow([subject, "2", scenario, questionid, response])
                questionid, response = "NA"

def main():
    outwriter = csv.writer(sys.stdout)
    outwriter.writerow(["subject","day","trialstring","questionnumber","workloadrating"])
    for filename in iglob('/home/michael/phd/exp3-datafinal/*/Day 2/EXP3-FINLAND*-DAY2.csv'):
        parse_file_workload(filename)
if __name__ == '__main__':
    main()