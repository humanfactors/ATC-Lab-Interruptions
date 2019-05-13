#This script will rename all non-conflict and non-PM aircraft a random string of digits
import re
import random
import itertools
from itertools import cycle
import csv

#input file is without randomised, output is randomised
inputfile = 'exp3_nobox-both_Denmark_V6.xml'
outputfile = 'exp3_nobox-both_Denmark_V7.xml'

callsigns = ['QF','QR', 'EK','CX','MH','NA','TT','BA','SQ','JQ','VA','GA']


#Set empty lists which will be used
aircraft = []
conflict = []
lista = []
sublista = []
splitlists = []

pmflag=False

o = open(outputfile,'wb')

with open(inputfile,'rb') as f:
    for line in f:
          ##track scenario number incase you need for output
        if "<atc:sky atc:idx" in line:
            scriptID = re.search('script([0-9]{1,2})',line)
            if scriptID:
                scriptID = scriptID.group(1)
        if "<!-- ==============NEW PM============ -->" in line:
            pmflag=True
            callnum = random.sample(xrange(1,99), 40)
            thisAC = -1  
        if pmflag:          
           if "atc:aircraft" in line and "</atc:aircraft>" not in line:
                lista = []
                primaryname =  re.search('([L,S]PM)',line)
                if primaryname:
                     primaryname = primaryname.group(1)
                     thisAC = thisAC + 1
                     flightnum = str(callnum[thisAC]).zfill(2)
                     airline = str(random.choice(callsigns))
                     newname = airline+flightnum
                     line = line.replace(str(primaryname),newname)
                     lista.append(scriptID)
                     lista.append(primaryname)
                     lista.append(newname)
                     print line
                     sublista.append([lista])
        if "<!-- ==============Conflicts========== -->" in line:
            pmflag=False

        o.write(line)
o.close()
f.close()

csvo = open('Denmark_Day1_PM-Names.csv','wb')
writer = csv.writer(csvo, delimiter=',')

for list in sublista:
    writer.writerows(list)    

csvo.close()

                         
