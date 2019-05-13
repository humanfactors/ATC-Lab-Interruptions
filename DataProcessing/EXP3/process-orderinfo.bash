#!/usr/bin/env bash

DATADIR=$1

find "$DATADIR" -name 'EXP3-FINLAND*.log' -exec sh -c '
	for f do
		python3 orderinfo.py "$f"
	done' sh {} + > ./tmpdata/orderinfo.csv

