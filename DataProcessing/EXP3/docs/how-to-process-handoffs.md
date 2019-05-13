# Experiment 3 Extraction Lab Notes

## Critical Information

Data VCS Hash: dccb0a5955e0ed5191062751a13b750151ca0b46
Laste update: Tue Jun 13 15:49:22 AWST 2017

## Extracting Handoff Data

Firsly, I recursive grep all files in the raw data directory which match an experimental file and output all the lines which contain handoff to exp3-handoffs.csv

```sh
grep -r --include \*FINLAND\*.csv 'handoff' . > ~/phd/atc-interruptions-derde/extraction/sh-pmhandoffs/exp3-handoffs.csv
```

Then, I need to regex replace filename entries such as "./PID_02/Day 1/EXP3-FINLAND-P02-DAY1.csv:" with 02,1 

```re
^./PID_\S+/Day \d/EXP3-FINLAND-(\S+)-DAY(\d).csv:
```


 add columns for LL as L,L