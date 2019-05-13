#!/bin/bash
# -*- coding: utf-8 -*-


# Accepts and handoffs
grep -r --include "*BULLPUP*.csv" "accept\|handoff"  ~/phd/exp3-datafinal/ > interruption-performance/int_acchand.csv
perl -pi.bak -e 's/^\/home\/michael\/phd\/exp3-datafinal\/PID_\S+\/Day \d\/EXP3-(BULLPUP\d)-[P]?(\S+)-DAY(\d).csv:/$2,$3,$1,/g' ./interruption-performance/int_acchand.csv


# Load conflict responses
grep -r --include "*BULLPUP*.csv" "^[0-9]*," ~/phd/exp3-datafinal > interruption-performance/int_conflicts.csv
perl -pi.bak -e 's/^\/home\/michael\/phd\/exp3-datafinal\/PID_\S+\/Day \d\/EXP3-(BULLPUP\d)-[P]?(\S+)-DAY(\d).csv:/$2,$3,$1,/g' ./interruption-performance/int_conflicts.csv

# Save Points
grep -r --include "*BULLPUP*.csv" '^INTID[0-8],-*[0-9]*$' ~/phd/exp3-datafinal/ > interruption-performance/points.csv
perl -pi.bak -e 's/^\/home\/michael\/phd\/exp3-datafinal\/PID_\S+\/Day \d\/EXP3-(BULLPUP\d)-[P]?(\S+)-DAY(\d).csv:/$2,$3,$1,/g' ./interruption-performance/points.csv
