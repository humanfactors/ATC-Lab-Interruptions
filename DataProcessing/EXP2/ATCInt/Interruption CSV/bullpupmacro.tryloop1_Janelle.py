import csv
import os
import re
from decimal import *
import pdb
## J: Need these next two. 
from scipy.optimize import leastsq
import numpy

numpy.seterr(invalid='ignore')

#This creates the list of filenames which contains all the participant files
filenames = []
for file in os.listdir('.'):
    if file.startswith("participant"):
        filenames.extend([file])

#This just loads up some basic headers. I don't think I even use them in the write csv file at this stage. Though would be awesome if I could.
headerpy = []
with open ('headers.csv','rb') as h:
    headers = csv.reader(h, delimiter=',')
    for row in headers:
        headerpy.extend([row[0]])
        #print headerpy
h.close()

#This just takes some of the example headers from the participant script file for accept/handoff. I only used this during testing, currently not actually used.
inputheaders = []
with open('participant_01_Bullpup_1.xml.csv', "rb") as h:
    inputheaderspy = csv.reader(h, delimiter=',')
    for line in inputheaderspy:
        if 'Flash Time (HP)' in line:
            inputheaders = line
h.close()
    
#This creates csv and array which will contain the final results
FinalData = []
open('FinalATCPhase.csv', 'wb')

#Here I begin the actual macro. It loops through all participant files.
for file in filenames:
    #This section here is just extracting the participant numbers and scenario number for the currently loaded file
    if file.startswith("participant"):
        participant = file.split("_")[1]
        scenariosplit = file.replace('.','_')
        scenarionum = scenariosplit.split("_")[3]
        scenario = int(float(scenarionum))
        print participant
        print scenario
##Conflicts##
#This loads the list of aircraft which are involved in conflicts.
    conflist = []
    with open('bpconf.csv', 'rb') as c:
        conflistr = csv.DictReader(c)
        #conflistr = enumerate(conflistr)
        for line in conflistr:
            #print line["Scenario"]
            conflist.extend([line])    
##The purpose of this function is it can, for any given scenario number (x) output the corresponding data for a given variable (e.g. T1). Index [0] = first aircraft, Index[1] = second aircraft
    def conflictlookup(var, x):
        with open('bpconf.csv', 'rb') as c:
            conflistr = csv.DictReader(c)         
            if x == 1:
                ac = []
                for line in conflistr:
                    ac.append(line[var])
                ac = ac[0:2]
                return ac
            if x == 2:
                ac = []
                for line in conflistr:
                    ac.append(line[var])
                ac = ac[2:4]
                return ac
            if x == 3:
                ac = []
                for line in conflistr:
                    ac.append(line[var])
                ac = ac[4:6]
                return ac
            if x == 4:
                ac = []
                for line in conflistr:
                    ac.append(line[var])
                ac = ac[6:8]
                return ac
            if x == 5:
                ac = []
                for line in conflistr:
                    ac.append(line[var])
                ac = ac[8:10]
                return ac
#This is the part of the script which actually looks up the conflicts
    with open(file, "rb") as p:
#This part just readers the open file in its entirety
        part1 = csv.reader(p, delimiter=',')
#sets all the variables that will be used to a null value
        cleanrows = []
        confhits = 0
        confmisses = 0
        confmissovertime =0
        confFA = 0
        CDT = 0

        
        ## J: I moved this section up because I wanted to load the data to calculate the conversion coefficient, then use this coefficient on...
        ## the conflict times as they are imported from bpconf.csv
        #Now I extract only rows which contain a row from the part1 file to the cleanrows file
        for row in part1:
            if row:
                cleanrows.append(row)
#This part of the script loads JUST the data in cleanrows STARTING FROM the conflicts portion of the script. This ensures that if an AC in a conflict was handedoff or accept, it isnt searched 2x
        for cell in (i[0:] for i in cleanrows):
            if "Intervene Time" in cell:
                cutoffrow = cell
                cutoffindex = cleanrows.index(cutoffrow)
            else:
                pass

        ## J: CALCULATE CONVERSION
        ## J: To fill out the data I used data from several columns
        conv_hd = ['Flash Time','Flash Time (HP)','Selection Time','Selection Time (HP)','Time','Time (HP)','Acceptance Time','Acceptance Time (HP)']
        conv_ind = []
        ## J: Get the columns associated with the headings above
        for h in conv_hd:
            conv_ind.append(cleanrows[0].index(h))
        
        conv_data = [] ## J: blank matrix to store the data
        
        ## J: get the data from the cleanrows list
        for h in cleanrows:
            if len(h) == len(cleanrows[0])+1: ## J: only interested in the longer rows
                conv_data.append([h[i] for i in conv_ind])
        
        ## J: This section converts the data to a numpy array which is much easier to work with.
        testA = numpy.zeros((len(conv_data),len(conv_data[0])))

        for a in range(numpy.size(testA,0)):
            for b in range(numpy.size(testA,1)):
                try:
                    testA[a,b] = conv_data[a][b]
                except:
                    testA[a,b] = numpy.nan ## J: if there's no number there the convert to nan
        
        dataC = numpy.reshape(testA,(numpy.size(testA,0)*4,2)) ## J: reshape the data from 8 columns into 2 columns
        ## J: remove odd values (crazy big number and -1)
        indn = numpy.where(dataC[:,1] < 1e15)
        dataC = dataC[numpy.unique(indn[0]),:]
        dataC[numpy.where(dataC == -1)] = 0

        ## J: REGRESSION FOR CONVERSION
        funcLine=lambda tpl,x : tpl[0]*x ## J: Set linear function to go through origin
        func=funcLine
        ErrorFunc=lambda tpl,x,y: func(tpl,x)-y ## J: Set error function
        tplInitial1=(1.0) ## J: initial coefficient (to start from)
        tplFinal1,success=leastsq(ErrorFunc,tplInitial1,args=(dataC[:,0],dataC[:,1])) ## J: perform least squares regression
        print "Conversion Coef" , tplFinal1

        conv = tplFinal1 ## J: sort the conversion coefficient

#Using the conflictlookup function defined earlier, this defines the four aircraft that conflict and the corresponding T1s and T2s
        AC1A = conflictlookup(var = "AC1", x = scenario)[0]#Pair1-Aircraft1
        AC1B = conflictlookup(var = "AC2", x = scenario)[0]#Pair1-Aircraft2
        AC2A = conflictlookup(var = "AC1", x = scenario)[1]#Pair2-Aircraft1
        AC2B = conflictlookup(var = "AC2", x = scenario)[1]#Pair2-Aircraft2

        ## J: Converted times here to make code cleaner later on
        AC1T2 = float(conflictlookup(var = "T2", x = scenario)[0]) * conv #Pair1-T2
        AC2T2 = float(conflictlookup(var = "T2", x = scenario)[1]) * conv #Pair2-T2
        AC1T1 = float(conflictlookup(var = "T1", x = scenario)[0]) * conv #Pair1-T1
        AC2T1 = float(conflictlookup(var = "T1", x = scenario)[1]) * conv #Pair2-T1
        ACNFA = [AC1A, AC1B, AC2A, AC2B]


#This part of the script is a bit dodgy but where the real work happens. It calculates the CDT and the number of confmisses. I beleive it is looking at each i in each row for cleanrows.

        ## J: CALCULATION CONFLICT TIMES
        ## J: Initialise a few variables need later on
        AC1_data = [];
        AC2_data = [];
        FA_total = 0;
        Miss_total = 0;

        for data in (i for i in cleanrows[cutoffindex:]):            
            ## J: For each intervention check if it's AC1A/B and AC2A/B - if so store intervention time...
            ## J: if not count as false alarm
            if AC1A in data or AC1B in data:
                AC1_data.append(float(data[1]))
            elif AC2A in data or AC2B in data:
                AC2_data.append(float(data[1]))
            elif data[0].isdigit():
                FA_total += 1

        ## J: Store detection times
        CDT_AC1 = []; 
        CDT_AC2 = []; 

        ## J: These loops check the intervene times, and store time/miss/false alarm accordingly
        ## J: For AC1 data then again for AC2 data
        for intTime in AC1_data:
            if intTime <= AC1T2 and intTime >= AC1T1:
                CDT_AC1.append(float(intTime - AC1T1))
            elif intTime > AC2T2:
                CDT_AC1.append('m')
            elif intTime < AC2T1:
                CDT_AC1.append('fa')

        for intTime in AC2_data:
            if intTime <= AC2T2 and intTime >= AC2T1:
                CDT_AC2.append(float(intTime - AC2T1))
            elif intTime > AC2T2:
                CDT_AC2.append('m')
            elif intTime < AC2T1:
                CDT_AC2.append('fa')

        ## J: get the minimum CDT times (if no time (e.g. 'm' or 'fa') then store as nan)
        k = numpy.array([min(CDT_AC1 or [numpy.nan]), min(CDT_AC2 or [numpy.nan])])
        ## J: calculate the average
        ave_CDT = numpy.mean(k[~numpy.isnan(k)])

        ## J: Calculate the number of misses
        if ("m" in CDT_AC1 and len(CDT_AC1)==1):
            Miss_total +=1
        elif CDT_AC1 == []:
            Miss_total +=1

        if ("m" in CDT_AC2 and len(CDT_AC2)==1):
            Miss_total +=1
        elif CDT_AC2 == []:
            Miss_total +=1

        ## J: Calculate the number of false alarms
        ## J: First get any 'correct' interventions that happened after already intervened with
        nAC1 = numpy.genfromtxt(numpy.array(CDT_AC1,dtype=str)) ## J: only get numbers from array
        try: ## J: look for any number vales that are not the minimum value calculated above (and ignore nan) 
            n1 = [s for s in nAC1 if s != k[0] and ~numpy.isnan(s)]
        except: ## J: if no number values then store as empty
            n1 = []

        ## J: repeat for CDT_AC2
        nAC2 = numpy.genfromtxt(numpy.array(CDT_AC2,dtype=str))
        try:
            n2 = [s for s in nAC2 if s != k[1] and ~numpy.isnan(s)]
        except:
            n2 = []

        ## J: Add any FA that were <T1
        f1 = [s for s in CDT_AC1 if s == 'fa']
        f2 = [s for s in CDT_AC2 if s == 'fa']

        ## J: Add above FAs to total
        FA_total = FA_total + len(n1) + len(n2) + len(f1) + len(f2)

            # if AC2A in data or AC2B in data:
            #     pair2intv = Decimal(data[1])
            #     if pair2intv > (AC2T2 * Decimal(1013.870422)):
            #         confmissovertime = confmissovertime +1
            #     elif pair2intv < (AC2T2 * Decimal(1013.870422)) and pair2intv < (AC2T1 * Decimal(1013.870422)):
            #         confFA=confFA+1
            #     elif pair2intv < (AC2T2 * Decimal(1013.870422))and pair2intv > (AC2T1 * Decimal(1013.870422)):
            #         confhits = confhits+1
            #         CDT = CDT + pair2intv - (AC2T1 * Decimal(1013.870422))
            #     else:
            #         print "ERROR on participant1", participant, scenario                
            # if AC1A in data or AC1B in data:
            #     pair1intv = Decimal(data[1])
            #     if pair1intv > (AC1T2 * Decimal(1013.870422)):
            #         confmissovertime = confmissovertime +1
            #     elif pair1intv < (AC1T2 * Decimal(1013.870422)) and pair1intv < (AC2T1 * Decimal(1013.870422)):
            #         confFA=confFA+1
            #     elif pair1intv < (AC1T2 * Decimal(1013.870422))and pair1intv > (AC2T1 * Decimal(1013.870422)):
            #         confhits = confhits+1
            #         CDT = CDT + pair1intv - (AC1T1 * Decimal(1013.870422))


#What I try to do in this section is lookup all aircraft in the data after this point, and if it isn't one of the conflict aircraft for that scenario, then this is a false alarm
##            matchob = re.search("\D\D\d\d",str(data))
##            print str(data)
##            if str(matchob) not in ACNFA:
##                print data
##              #if 2 numbers/letters wildcard then false alarm                     
##            elif AC2A and AC2B in data:
##                pass #do something here
##            elif AC1A and AC1B in data:
##                pass #do something here

# #Simple way of getting the conflict misses, this is working fine
#         confmisses = 2 - confhits
# #This just outputs the CDT assuming confhits not zero to avoid divide by zero error        
#         if confhits > 0:
#             CDT = CDT/confhits
#         else:
#             CDT = 0
            
#ACCEPT AND HANDOFF DATA#            
#Create blank accept and handoff variables. Then open data files for accept/handoff data
    accept = []
    handoff = []
    with open(file, "rb") as p:
        part1 = csv.reader(p, delimiter=',')
        for row in part1:
            if 'accept' in row:
                accept.extend([row[:16]])
            if 'handoff' in row:
                handoff.extend([row[:16]])
    p.close()

    #Accept data
    #This just calculates the number of acceotence misses pretty simple. Working fine.
    acceptmisses = 0
    for time in (i[10] for i in accept):
        if time == "":
          acceptmisses = acceptmisses+1
    #This calculates the acceptance reaction time, again - pretty simple. Working fine.
    acceptRTtemp = 0
    acceptnum = 0
    RTscoresacc = 0
    for i in accept:
        if i[10]  == "":
            pass     
        else:
            acceptnum = acceptnum+1
            AcceptTime = Decimal(i[10])
            FlashTime = Decimal(i[2])
            if FlashTime > 12376900 or FlashTime == 0:
                acceptRTtemp = AcceptTime
                RTscoresacc = RTscoresacc + acceptRTtemp 
            else:
                acceptRTtemp = AcceptTime-FlashTime
                RTscoresacc = RTscoresacc + acceptRTtemp
    if acceptnum > 0:
        RTacceptfinal = RTscoresacc / acceptnum
    else:
        RTacceptfinal = 0

    #Handoff data
    #This just calculates the number of handoff misses pretty simple. Working fine.
    handoffmisses = 0
    for time in (i[10] for i in handoff):
        if time == "":
          handoffmisses = handoffmisses+1
    #print handoffmisses
    #This calculates the handoff reaction time, again - pretty simple. Working fine.
    handoffRTtemp = 0
    handoffnum = 0
    RTscoresHO = 0

    for i in handoff:
        if i[10]  == "":
            pass     
        else:
            handoffnum = handoffnum+1
            handoffTime = Decimal(i[10])
            FlashTime = Decimal(i[2])
            if FlashTime > 12376900 or FlashTime == 0:
                handoffRTtemp = handoffTime
                RTscoresHO = RTscoresHO + handoffRTtemp
            else:
                handoffRTtemp = handoffTime-FlashTime
                RTscoresHO = RTscoresHO + handoffRTtemp
                
    if handoffnum > 0:
        RThandofffinal = RTscoresHO / handoffnum
    else:
        RThandofffinal = 0

    ## J: Instead of writing to a csv file everytime, store in a matrix and then write the file at the end
    FinalData.append([participant, scenario, RThandofffinal, handoffmisses, RTacceptfinal, acceptmisses, ave_CDT, Miss_total, FA_total])

## J: WRITE HEADINGS AND FINAL DATA TO FILE
with open('FinalATCPhase.csv','wb') as f1:
    writer=csv.writer(f1, delimiter=',')
    writer.writerow(headerpy) ## J: write headings 
    for row in FinalData:
        writer.writerow(row) ## J: write data
## J: another option would be to use numpy.savetxt, seems more straightforward but I could be wrong.        

    # #This is where it all compiles. In this part, FinalData is a list of all values needed in the final CSV, then it simply writes all of this to a csv.
    # FinalData = [RThandofffinal, handoffmisses, RTacceptfinal, acceptmisses, ave_CDT, Miss_total, FA_total]
    # print FinalData
    # with open('FinalATCPhase.csv','a') as f1:
    #     writer=csv.writer(f1, delimiter=',', lineterminator='\n', )
    #     writer.writerow([participant]+[scenario]+FinalData)
    # #print participant, scenario,"FINISHED"
    
