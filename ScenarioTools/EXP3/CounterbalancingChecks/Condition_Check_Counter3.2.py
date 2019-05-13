# laboratory: HFACL
# date: 08/04/2016
# username: michael
# description: this script will iterate
#through script files from experiment
#three to ensure counts are correct


import random
import re
import os
import csv
from collections import Counter

##############################################################
##This extracts the cells from day 1 and two and sorts them into
## participant and day. It takes the scenario numbers but it
## can be modified to extract any value from them for checks.
def make_total(data,col):

    subtotal_scen_B = []
    for i in range(1,9):#change this depending on how many scripts you have
        scenario1 = []
        scenario2 = []
        f.seek(0)
        for row in data:
            print row
            if "Participant"+str(i) in row[0] and "DAY1" in row[1]:
                scenario1.append(row[col])
            if "Participant"+str(i) in row[0] and "DAY2" in row[1]:
                scenario2.append(row[col])    
        subtotal_scen_B.append([scenario1]+[scenario2])
        
    return subtotal_scen_B



#Key variables
output = []
scriptn = ""
launchterm = ""
#Local Variables
interruptioncondition = []
timecondition = []
subtrialcondition = []

#Global Variables
launchcount = []
totaltrialcondition = []

#This section defines all the variables which we then use later to write to csv
for filename in os.listdir(os.getcwd()):
    if filename.endswith(".xml"):
        #Sets local variables
        interruptioncondition = []#stores list of interruption condition variables
        timecondition = []#stores list of time condition names
        subtrialcondition = []#stores a local list of both these combined
        
        day = re.search('(DAY[1-2])',filename).group(1)
        ID = re.search('(Participant[1-8])',filename).group(1)
        #Print filename for header
        #print '\n',filename+'\n'
        #Start extraction loop
        f = open(filename, 'rb')
        for line in f.readlines():
            if re.search('(sky=\')([0-9]{1,2})([a-zA-Z][a-zA-Z])',line):
                #These are local loop variables true for each participant
                trialnumber = re.search('(sky=\')([0-9]{1,2})([a-zA-Z][a-zA-Z])',line).group(2)
                timecondition = re.search('(sky=\')([0-9]{1,2})([a-zA-Z][a-zA-Z])',line).group(3)
                launchstring = re.search('launch([0-8]*).VBS', line)
                if launchstring:
                    interruptioncondition = str("ATC"+launchstring.group(1))
                    launchcount.append(launchstring.group(1))
                elif not launchstring:
                    interruptioncondition= "None"
                #These are global variables                           
                subtrialcondition.append((ID,day,str(trialnumber),str(timecondition),interruptioncondition))
        totaltrialcondition.append(subtrialcondition)
        f.close()

#This section outputs all the participant data by section
##o = open('check_participant_values.csv','wb')
##csvw = csv.writer(o)
##for list in totaltrialcondition:
##    csvw.writerows(list)
##o.close()

#This is just a nicer csv file for excel counters.
##DAY1 = []
##DAY2 = []
##for list in totaltrialcondition:
##    if any("DAY1" in s for s in list):
##        DAY1.append(list)  
##    if any("DAY2" in s for s in list):
##        DAY2.append(list)
##
##zipper = zip(DAY1, DAY2)
##
##with open('check_cb_merged-days.csv', 'wb') as f:
##    writer = csv.writer(f)
##    for list in zipper:
##        writer.writerows(list)

###############################################################
##This area is for doing the proofs outlined in the checklist##
###############################################################

#defines all the key variable names
ATCstrings = ["ATC1","ATC2","ATC3","ATC4","ATC5","ATC6","ATC7","ATC8"]
timingstrings = ["SS","SL","LS","LL"]
conditionstrings = ["ATC","None"]
IDstring = []
for i in range(1,9):
	IDstring.append("Participant"+str(i))
days = ["DAY1","DAY2"]

#opens the CSV created in section above.
f = open('check_participant_values.csv','rb')
csvr = csv.reader(f)

#This checks all scenarios should have an equal number of observations
total_scen_A = []
for row in csvr:
    total_scen_A.append(row[2])
print "####################################################################"
print "1. CHECK TO SEE IF ALL SCENARIOS HAVE EQUAL NUMBER OF OBSERVATIONS"
print "Observations across all participants for each scenario number",Counter(total_scen_A)
print "####################################################################"
print "\n\n"

#Each participant should see each scenario 2x
f.seek(0)
total_scen_B = make_total(csvr,2)
print "####################################################################"
print "2. CHECK TO SEE IF PART. SEES ALL SCENARIOS ONCE EACH DAY"
for i, j in enumerate(total_scen_B):
    print "Participant"+str(i+1)+"On Day 1 count:",Counter(j[0])
    print "Participant"+str(i+1)+"On Day 2 count:",Counter(j[0])
print "####################################################################"
print "\n\n"

#No scenario is associated with a time condition more than others.
f.seek(0)
merge_scen_C = []
for row in csvr:
    merge_scen_C.append(str(row[2]+row[3]))
print "####################################################################"
print "3. CHECK TO SEE IF SCENARIOS ASSOCIATED WITH TIME CONDITIONS EQUALLY"
count_scen_C = Counter(merge_scen_C)
print count_scen_C                        
print "####################################################################"
print "\n\n"

#No scenario is associated with a int condition more than others.
f.seek(0)
merge_scen_D = []
for row in csvr:
    row[4] = re.sub(r'(ATC[0-9])','ATC', row[4])
    merge_scen_D.append(str(row[2]+row[4]))
print "####################################################################"
print "4. CHECK TO SEE IF SCENARIOS ASSOCIATED WITH INT CONDITIONS EQUALLY"
count_scen_D = Counter(merge_scen_D)
print count_scen_D                        
print "####################################################################"
print "\n\n"


#E + D
f.seek(0)
merge_scen_DE = []
for row in csvr:
    row[4] = re.sub(r'(ATC[0-9])','ATC', row[4])
    merge_scen_DE.append(str(row[2]+row[3]+row[4]))
print "########################################################################"
print "5. CHECK TO SEE IF SCENARIOS ASSOCIATED WITH INT+TIME CONDITIONS EQUALLY"
count_scen_DE = Counter(merge_scen_DE)
print count_scen_DE                        
print "####################################################################"
print "\n\n"

#equal observation of all eight conditions
f.seek(0)
total_cond_F = []
for row in csvr:
    row[4] = re.sub(r'(ATC[0-9])','ATC', row[4])
    total_cond_F.append(str(row[3]+row[4]))
print "####################################################################"
print "6. CHECK TO ENSURE ALL CONDITIONS OCCUR AND EQUAL NUMBER OF TIMES"
count_scen_F = Counter(total_cond_F)
print count_scen_F                        
print "####################################################################"
print "\n\n"

#Each participant should see each interruption 2x and none 8x
f.seek(0)
total_scen_B = make_total(csvr,4)
print "####################################################################"
print "7. CHECK TO SEE IF PART. SEES ALL ATC INT ONCE EACH DAY and NONE 8x"
for i, j in enumerate(total_scen_B):
    print "Participant"+str(i+1)+"On Day 1 count:",Counter(j[0])
    print "Participant"+str(i+1)+"On Day 2 count:",Counter(j[0])
print "####################################################################"
print "\n\n"

#Each participant should see each condition 4 times
f.seek(0)
total_scen_B = make_total(csvr,3)
print "####################################################################"
print "8. CHECK TO SEE IF PART. SEES ALL ATC INT ONCE EACH DAY"
for i, j in enumerate(total_scen_B):
    print "Participant"+str(i+1)+"On Day 1 count:",Counter(j[0])
    print "Participant"+str(i+1)+"On Day 2 count:",Counter(j[0])
print "####################################################################"
print "\n\n"

#Shows which scenarios are randomly more assocaited with a ATC condition.
f.seek(0)
merge_scen_int = []
for row in csvr:
    if "ATC" in row[4]:
        merge_scen_int.append(str(row[2]+row[4]))
print "####################################################################"
print "9. CHECK TO SEE WHICH SCENARIOS RANDOMLY MORE ASSOCIATED WITH INT"
count_scen_int = Counter(merge_scen_int).most_common()
print count_scen_int                        
print "####################################################################"
print "\n\n"

#Check even distribtion of everything for participant.
f.seek(0)
merge_scen_int = []
for row in csvr:
    if "ATC" in row[4]:
        merge_scen_int.append(str(row[2]+row[4]))
print "####################################################################"
print 
count_scen_int = Counter(merge_scen_int).most_common()
print count_scen_int                        
print "####################################################################"
print "\n\n"
#Each participant should see each condition 4 times

f.seek(0)
total_scen_R = make_total(csvr,3)
print "####################################################################"
print "10. CHECK TO SEE ALL CONDITION TYPES EVEN FOR ALL PARTICIPANTS"
for i, j in enumerate(total_scen_R):
    print "Participant"+str(i+1)+"On Day 1 count:",Counter(j[0])
    print "Participant"+str(i+1)+"On Day 2 count:",Counter(j[0])
print "####################################################################"
print "\n\n"
			
