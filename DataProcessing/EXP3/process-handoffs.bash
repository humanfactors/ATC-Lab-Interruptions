#!/usr/bin/env bash

DATADIR=$1
export DATADIR
grep -r --include "*FINLAND*.csv" 'handoff' $DATADIR > ./tmpdata/allhandoffs.csv
perl -pi.bak -e 's/^$ENV{'DATADIR'}\/PID_\S+\/Day \d\/EXP3-FINLAND-[P]?(\S+)-DAY(\d).csv:/$1,$2,/g' ./tmpdata/allhandoffs.csv

# echo "pid, day, trial, aircraftname, scenario, interruption, encode, retrieve, tt, flashtimems, actiontime, keypress, correctkey, correct, rt, procerror, nonresponse" > ../data/deferred/deferredhandoff.csv
# python3 handoffs.py ./tmpdata/allhandoffs.csv >> ../data/deferredhandoff.csv

echo "pid, day, trial, scenario, interruption, encode, retrieve, trialtype, correct, falsealarm, miss, flashtime_s, actiontime, rt" > ../data/routinehandoff.csv
python3 handoffs.py ./tmpdata/allhandoffs.csv >> ../data/routinehandoff.csv
