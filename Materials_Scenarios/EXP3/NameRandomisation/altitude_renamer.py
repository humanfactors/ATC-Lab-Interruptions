import re
import random
import itertools
from itertools import cycle

inputfile = 'EXP3_allbox_finalBahrain_random_record.xml'
outputfile = 'exp3_allbox_finalBahrain_random_altitudes.xml'

altitudes = [0,0,0,0,0,0,0,100,100,200,200,300,300,400,400,500,500,500,500,500,0,0,0,0,0,0,0,-100,-100,-200,-200,-300,-300,-400,-400,-500,-500,-500,-500,-500]
count = 0
new40s = [37000,38000,39000,40000]
confalts = [0,0,0,0,0,0,0,500,500,500,-500,-500,100,200,300,400,600,700,800,900,0,0,0,0,0,0,500,500,-500,-500,-500,-100,-200,-300,-400,-600,-700,-800,-900]

o = open(outputfile,'w')

conflag=False
normflag=False

#Sets a toggler starting at 1
myIterator = cycle(range(2))

with open(inputfile,'U') as f:
    for line in f:
          ##track scenario number incase you need for output
        if "<atc:sky atc:idx" in line:
            scriptID = re.search('script([0-9]{1,2})',line)
            if scriptID:
                scriptID = scriptID.group(1)
                print "--------",scriptID,"--------"
        if "<!-- ==============Conflicts========== -->" in line:
            conflag=True
            normflag=False
        if conflag:
            if "<atc:altitude>" in line:
                checkpoint = re.search('(\t\t\t\t\t\t<atc:altitude>)',line)
                if not checkpoint:
                    myIterator.next()
                    if myIterator.next() is 0:
                        thisaltitude = int((re.search('([0-9]{1,5})',line).group(1)))
                        tempalt = str(thisaltitude + random.choice(altitudes))
                        tempvar = line.replace(str(thisaltitude),tempalt)
                    if myIterator.next() is 1:
                        thisaltitude = int((re.search('([0-9]{1,5})',line).group(1)))
                        line = line.replace(str(thisaltitude),str(int(tempalt)+random.choice(confalts)))
                        newaltitude = int((re.search('([0-9]{1,5})',line).group(1)))
                        print tempalt,newaltitude

        if "<!-- ==============Normal================= -->" in line:
            conflag=False
            normflag=True
        if normflag:
            if "atc:aircraft" in line and "</atc:aircraft>" not in line:
                primaryname =  re.search('(idx=.[a-zA-Z]{1,6}[0-9]{1,5})',line)
                if primaryname:
                    primaryname = primaryname.group(1)
            if "<atc:altitude>" in line:
                checkpoint = re.search('(\t\t\t\t\t\t<atc:altitude>)',line)
                if not checkpoint:
                    thisaltitude = int((re.search('([0-9]{1,5})',line).group(1)))
                    if thisaltitude > 40000:
                        thisaltitude = random.choice(new40s)
                    line = line.replace(str(thisaltitude),str(thisaltitude + random.choice(altitudes)))

                    
        if "<!-- ============== PM Box" in line:
            normflag=False
        o.write(line)
o.close()
f.close()

                    #acName can match either callsign or flight type

print count
