## PM Info Box replacement script
## Insert short and long box information generated from an excel spreadsheet
## Then assign inputs and outputs as required

import re
import os

#counter used in loop later
counter = -1

#speciy the short or long box required and load box information
f = open('short.txt', 'U')
short = f.read()
f.close()
shortboxes = short.split("\n")

f = open('long.txt', 'U')
longe = f.read()
f.close()
longboxes = longe.split("\n")

#Filenames for shortlong templates
longscript = 'exp3_nobox-long_Denmark_V2.xml'
shortscript = 'exp3_nobox-short_Denmark_V2.xml'

#create file to write to FOR SS
o = open("EXP3_shortbox-short_Denmark_V3.xml",'w')
#specify retreival template you want to use
with open(shortscript, 'r+') as script:
    for line in script:#This then iterates through the occurance of the Box Tag and replaces with appropriate new box and renames sky accordingly.
        if "<atc:sky atc:idx" in line:
            ScriptID = re.search('(script[0-9]{1,2})',line).group(1)
            TrialNum = re.search('(script)([0-9]{1,2})',line).group(2)
            if ScriptID:
                line = line.replace(str(ScriptID),str(TrialNum)+"SS")
                print line,"Done"
        if '<!-- ==============Box================ -->' in line:
            counter = counter + 1
            line = line.replace('<!-- ==============Box================ -->', shortboxes[counter])
        o.write(line)
o.close()
counter = -1
#create file to write to for LS
o = open("EXP3_longbox-short_Denmark_V3.xml",'w')
with open(shortscript, 'r+') as script:
    for line in script:
        if "<atc:sky atc:idx" in line:
            ScriptID = re.search('(script[0-9]{1,2})',line).group(1)
            TrialNum = re.search('(script)([0-9]{1,2})',line).group(2)
            if ScriptID:
                line = line.replace(str(ScriptID),str(TrialNum)+"LS")
                print line,"Done"
        if '<!-- ==============Box================ -->' in line:
            counter = counter + 1
            line = line.replace('<!-- ==============Box================ -->', longboxes[counter])
        o.write(line)
o.close()
counter = -1


#create file to write to for SL
o = open("EXP3_shortbox-long_Denmark_V3.xml",'w')
with open(longscript, 'r+') as script:
    for line in script:
        if "<atc:sky atc:idx" in line:
            ScriptID = re.search('(script[0-9]{1,2})',line).group(1)
            TrialNum = re.search('(script)([0-9]{1,2})',line).group(2)
            if ScriptID:
                line = line.replace(str(ScriptID),str(TrialNum)+"SL")
                print line,"Done"
        if '<!-- ==============Box================ -->' in line:
            counter = counter + 1
            line = line.replace('<!-- ==============Box================ -->', shortboxes[counter])
        o.write(line)
o.close()
counter = -1   
#create file to write to for LL
o = open("EXP3_longbox-long_Denmark_V3.xml",'w')
with open(longscript, 'r+') as script:
    for line in script:
        if "<atc:sky atc:idx" in line:
            ScriptID = re.search('(script[0-9]{1,2})',line).group(1)
            TrialNum = re.search('(script)([0-9]{1,2})',line).group(2)
            if ScriptID:
                line = line.replace(str(ScriptID),str(TrialNum)+"LL")
                print line,"Done"
        if '<!-- ==============Box================ -->' in line:
            counter = counter + 1
            line = line.replace('<!-- ==============Box================ -->', longboxes[counter])
        o.write(line)
o.close()
counter = -1
o = open("EXP3_Denmark_CompiledAll_V3.xml",'w')
for filename in os.listdir(os.getcwd()):
    flag = False
    if filename.startswith("EXP3"):
        f = open(filename, 'rU')
        for line in f.readlines():
            if "<!-- =========================== SCRIPTS ================================= -->" in line:
                flag = True
            if flag:
                o.write(line)
            if "<!-- =========================== TEST ORDER ============================== -->" in line:
                flag = False
                f.close()
o.close()
