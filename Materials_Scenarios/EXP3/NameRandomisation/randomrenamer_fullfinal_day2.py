#This script will rename all non-conflict and non-PM aircraft a random string of digits
import re
import random
import itertools
from itertools import cycle
import csv

#input file is without randomised, output is randomised
inputfile = 'exp3_nobox-both_Finland_V1-DAY1.xml'
outputfile = 'exp3_nobox-both_Finland_V2-DAY1-TEMP.xml'

#define key variables for whole script
acName = re.compile('(.+[a-zA-Z]{1,6}[0-9]{1,5})')
callsigns = ['QF','QR', 'EK','CX','MH','NA','TT','BA','SQ','JQ','VA','GA']

#Set empty lists which will be used
aircraft = []
conflict = []
pairs = []
pmlist = []
subpmlist = []
conflist = []
subconflist = []

#Sets initial flags to be false
conflag=False
normflag=False
pmflag=False
#opens an output file
o = open(outputfile,'w')
with open(inputfile,'U') as f:
     for line in f:
          ##track scenario number incase you need for output
          if "<atc:sky atc:idx" in line:
               scriptID = re.search('script([0-9]{1,2})',line)
               if scriptID:
                    scriptID = scriptID.group(1)
                    thisAC = -1
                    callnum = random.sample(xrange(1,99), 40)
          ##identifies start of conflict section as seed
          if "<!-- ==============NEW PM============ -->" in line:
               conflag=False
               normflag=False
               pmflag=True
          if pmflag:          
               if "atc:aircraft" in line and "</atc:aircraft>" not in line:
                    pmlist = []
                    primaryname =  re.search('([a-zA-Z]{1,6}[0-9]{1,5})',line)
                    if primaryname:
                         primaryname = primaryname.group(1)
                         thisAC = thisAC + 1
                         flightnum = str(callnum[thisAC]).zfill(2)
                         airline = str(random.choice(callsigns))
                         newname = airline+flightnum
                         line = line.replace(str(primaryname),newname)
                         pmlist.append(scriptID)
                         pmlist.append(primaryname)
                         pmlist.append(newname)
                         subpmlist.append([pmlist])
          if "<!-- ==============Conflicts========== -->" in line:
               conflag=True
               normflag=False
               pmflag=False
          if conflag:          
               if "atc:aircraft" in line and "</atc:aircraft>" not in line:
                    conflist = []
                    primaryname =  re.search('([a-zA-Z]{1,6}[0-9]{1,5})',line)
                    if primaryname:
                         primaryname = primaryname.group(1)
                         thisAC = thisAC + 1
                         flightnum = str(callnum[thisAC]).zfill(2)
                         airline = str(random.choice(callsigns))
                         newname = airline+flightnum
                         line = line.replace(str(primaryname),newname)
                         pairs.append(newname)
                         conflist.append(primaryname)
                         conflist.append(newname)
                         subconflist.append([conflist])                         
          if "<!-- ==============Normal================= -->" in line:
               conflag=False
               normflag=True
               pmflag=False
          if normflag:
               if "atc:aircraft" in line and "</atc:aircraft>" not in line:
                    #acName can match either callsign or flight type
                    if acName.match(line):
                         #get out current name
                         currentname = re.search('idx="([a-zA-Z]{0,6}[0-9]{0,5}.)"',line)
                         if currentname:
                              currentname = currentname.group(1)
                              thisAC = thisAC + 1
                              flightnum = str(callnum[thisAC]).zfill(2)
                              airline = str(random.choice(callsigns))
                              line = line.replace(str(currentname),airline + flightnum)
          o.write(line)
o.close()
f.close()

#This section fixes the conflict detail parts of conflict section
#Split the conflict pairs into lists within a list.
pairsSplit = [pairs[x:x+2] for x in xrange(0, len(pairs),2)]

#Now too get primary indexes of all items in pairSplit for the cycler
#enumerate(xs) gives back a list of tuples of the form (index, item) from a list, see note 1
indexes = [i for i, l in enumerate(pairsSplit) for num in l]

#Sets a toggler starting at 1
myIterator = cycle(range(2))
myIterator.next()

inputfile = outputfile
outputfile = 'exp3_nobox-both_Finland_V1-DAY2.xml'

o = open(outputfile,'w')
#Sets the conflict pair at zero
ACnum = -1
with open(inputfile,'U') as f:
     for line in f:
          if "<atc:conflictAircraft>" in line:
               confdetail =  re.search('([a-zA-Z]{1,6}[0-9]{1,5})',line)
               if confdetail:
                    ACnum = ACnum +1
                    confdetail = confdetail.group(1)
                    line = line.replace(str(confdetail),pairsSplit[indexes[ACnum]][myIterator.next()])
          o.write(line)
o.close()  

csvp = open('Denmark_Day2_PM-Names.csv','wb')
writer = csv.writer(csvp, delimiter=',')

for list in subpmlist:
    writer.writerows(list)    

csvp.close()

csvc = open('Denmark_Day2_Conflict-Names.csv','wb')
writer = csv.writer(csvc, delimiter=',')

for list in subconflist:
    writer.writerows(list)    

csvc.close()

#Note 1
##indexes = []
##for i, l in enumerate(x):
##    for num in l:
##        indexes.append(i)
