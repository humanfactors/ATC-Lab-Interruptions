#!/bin/bash
# -*- coding: utf-8 -*-

grep -r --include "*FINLAND*.csv" '^[0-9]{1,2},[0-9]{1,2}' ~/phd/exp3-datafinal > ./tmpdata/workload.csv
perl -pi.bak -e 's/^$ENV{'DATADIR'}\/PID_\S+\/Day \d\/EXP3-FINLAND-[P]?(\S+)-DAY(\d).csv:/$1,$2,/g' ./tmpdata/allhandoffs.csv

