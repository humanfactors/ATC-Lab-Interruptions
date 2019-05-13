import random
import re
import os

#!/usr/bin/python
# -*- coding: utf-8 -*-

output = []

for filename in os.listdir(os.getcwd()):
    f = open(filename, 'rU')
    print '\n'
    print filename
    for line in f.readlines():
        if re.search('test[0-9]', line):
            scriptn = re.search('script([0-9]*)', line)
            print scriptn.group()
            
        if re.search('launch([0-5]*).VBS', line):
            launchterm = re.search('launch([0-5]*).VBS', line)
            print launchterm.group()

        if re.search('<!--No Interruption Condition-->', line):
            print 'no interruption' 
            
        if re.search('<atc:interruption', line) and not re.search('launch([0-5]*).VBS', line):
            print 'BLANK INTERRUPTION'
            wasBlank = 1

    f.close()

			
