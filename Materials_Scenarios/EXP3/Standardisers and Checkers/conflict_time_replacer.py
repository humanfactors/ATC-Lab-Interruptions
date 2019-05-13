import re
import random
import itertools
import csv
from itertools import cycle

inputfile = 'exp3_nobox-both_Denmark_V5.xml'
outputfile = 'exp3_nobox-both_Denmark_V6.xml'


time1 = [0,0,105,105,186,186,80,80,0,0,171,171,30,30,0,0,142,142,0,0,150,150,95,95,0,0,177,177,40,40,144,144,0,0,85,85,180,180,100,100,0,0,0,0,141,141,25,25,0,0,162,162,100,100,163,163,75,75,0,0,0,0,180,180,140,140,0,0,175,175,105,105,0,0,35,35,180,180,0,0,185,185,75,75,128,128,190,190,10,10,0,0,107,107,192,192]
time2 = [38,38,191,191,284,284,245,245,123,123,263,263,166,166,49,49,261,261,44,44,257,257,253,253,90,90,244,244,222,222,258,258,54,54,260,260,274,274,208,208,72,72,56,56,213,213,223,223,80,80,237,237,288,288,256,256,183,183,81,81,84,84,277,277,213,213,57,57,287,287,204,204,80,80,220,220,280,280,103,103,278,278,191,191,254,254,269,269,118,118,82,82,227,227,273,273]

o = open(outputfile,'wb')


conflag=False
counter = -1
with open(inputfile,'U') as f:
    for line in f:
        if "<atc:sky atc:idx" in line:
            scriptID = re.search('script([0-9]{1,2})',line)
            if scriptID:
                scriptID = scriptID.group(1)
                print scriptID
        if "<!-- ==============Conflicts========== -->" in line:
            conflag=True
        if conflag:
            if "atc:aircraft" in line and "</atc:aircraft>" not in line:
                counter = counter + 1
                tempt1 = str(time1[counter])
                tempt2 = str(time2[counter])
                primaryname =  re.search('([a-zA-Z]{1,6}[0-9]{1,5})',line)
                if primaryname:
                    primaryname = primaryname.group(1)
            if "<atc:acceptanceTime>" in line:
                acceptime = re.search('([0-9]{1,3})',line).group(1)
                line = line.replace(str(acceptime),tempt1)
            if "<atc:conflictTime>" in line:
                conflicttime = re.search('([0-9]{1,3})',line).group(1)
                line = line.replace(str(conflicttime),tempt2)
                print primaryname,"OLD:",acceptime,conflicttime,"NEW:",tempt1,tempt2
        if "<!-- ==============Normal================= -->" in line:       
            conflag=False
        o.write(line)
o.close()
f.close()
