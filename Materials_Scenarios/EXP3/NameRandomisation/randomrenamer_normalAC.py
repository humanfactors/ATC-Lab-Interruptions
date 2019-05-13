#This script will rename all non-conflict and non-PM aircraft a random string of digits
import re
import random

#input file is without randomised
inputfile = 'exp3_short_short_normalfix.xml'
#output file is with randomised
outputfile = 'exp3_short_short_random.xml'

#Regex for aircraft name
acName = re.compile('(.+[a-zA-Z]{1,6}[0-9]{1,5})')

#define call signs based on real Australian aircraft CS 2 Digit
callsigns = ['QF','QR', 'EK','CX','MH','NA','TT','BA','SQ','JQ']

#opens an output file
##o = open(outputfile,'w')

#Sets initial flag to be false
flag=False
with open(inputfile,'U') as f:
     for line in f:
          #track scenario number
          if "<atc:sky atc:idx" in line:
               scriptID = re.search('script([0-9]{1,2})',line)
               if scriptID:
                    scriptID = scriptID.group(1)
          #sorts out the non-special aircraft
          if "<!-- ==============Normal================= -->" in line:
            flag=True
          #the meat of the program
          if flag:
               #defines random sample of callnumbers for this trial, and thisAC counts up to ensure no repeats
               callnum = random.sample(xrange(1,99), 40)
               thisAC = -1
               #ensures only looking at atc:aircraft node
               if "atc:aircraft" in line and "</atc:aircraft>" not in line:
                    #acName can match either callsign or flight type
                    if acName.match(line):
                         #get out current name
                         currentname = re.search('idx="([a-zA-Z]{1,6}[0-9]{1,5}.)"',line)
                         if currentname:
                              currentname = currentname.group(1)
                              #Assigns new name
                              thisAC = thisAC + 1
                              flightnum = str(callnum[thisAC]).zfill(2)
                              airline = str(callsigns[random.randrange(0,10,1)])
                              #replaces old name
                              line = line.replace(str(currentname),airline + flightnum)
                              #print old and new
                              print scriptID,",",currentname,",",airline + flightnum
          if '<!-- ==============Box================ -->' in line:
               flag=False


#### Toggle 
	##o.write(line)
##o.close()             
