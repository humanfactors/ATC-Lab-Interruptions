import random
import re
import os

#!/usr/bin/python
# -*- coding: utf-8 -*-

output = []
scriptn = ""
launchterm = ""

for filename in os.listdir(os.getcwd()):
    if filename.startswith("EXP3"):
        f = open(filename, 'rU')
        print '\n'
        participant = filename
        for line in f.readlines():
            if re.search('launch([0-8]*).VBS', line):
                launchterm = re.search('launch([0-8]*).VBS', line)
                launchterm = "ATC"+str(launchterm.group(1))
            if not re.search('launch([0-8]*).VBS', line):
                launchterm = "None"
            if re.search('(sky=\'[0-9]{1,2}[a-zA-Z][a-zA-Z])',line):
                scriptn = re.search('(sky=\')([0-9]{1,2})([a-zA-Z][a-zA-Z])',line).group(2)
                cond = re.search('(sky=\')([0-9]{1,2})([a-zA-Z][a-zA-Z])',line).group(3)
                print filename+","+scriptn+","+cond+","+launchterm
f.close()

			
