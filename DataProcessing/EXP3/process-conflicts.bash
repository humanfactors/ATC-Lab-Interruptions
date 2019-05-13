#!/usr/bin/env bash

DATADIR=$1

find "$DATADIR" -name 'EXP3-FINLAND*.log' -exec sh -c '
	for f do
		echo "Processing conflicts for $f"
		python3 conflicts.py "$f"
	done' sh {} +

# The python script will output three csv: conflictdetection, falsealarms, misses
# The following simply adds headers to these files and aggergates all responses

echo "subject, day, trialstring, scenario, pairid, interruption, encode, retrieve, type, t1_ms, t2_ms, intervenetime_ms, rt, correct, miss, nonresponse, late, early" > ~/phd/atc-interruptions-derde/data/conflicts.csv
cat tmpdata/conflicts.csv >> ~/phd/atc-interruptions-derde/data/conflicts.csv

