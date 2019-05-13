import random

# General comment: some of the script might be confusing because python
# uses zero-based numbering to index arrays - Simon

# read the full list of 16 scenario x 8 condition combiniations (128 in there)
f = open('ScenarioList.xml', 'U')
data = f.read()
f.close()

# read in the main portion of the data file (all before scenariolist). Different versions for Day 1 and Day 2 (due to randomisation).
f = open('EXP3_FinlandV4_Day1.xml', 'U')
MainScript1 = f.read()
f.close()

f = open('EXP3_FinlandV4_Day2.xml', 'U')
MainScript2 = f.read()
f.close()

# read the final few lines at the end of the file to compile nicely.
f = open('interruptionstest2.xml', 'U')
EndScript = f.read()
f.close()

#Specifies the padders which go between trials for Workload, Anxiety, and a Rest Break.
restshort = '\t\t\t<atc:instruction atc:idxref="Rest"/>\n\t\t\t<atc:instruction atc:idxref="restend"/>'
restlong = '\t\t\t<atc:instruction atc:idxref="Restlong"/>\n\t\t\t<atc:instruction atc:idxref="restend"/>'

restshortday2 = '\t\t\t<atc:instruction atc:idxref="Workload1"/>\n\t\t\t<atc:instruction atc:idxref="Anxiety1"/>\n\t\t\t<atc:instruction atc:idxref="Rest"/>\n\t\t\t<atc:instruction atc:idxref="restend"/>'
restlongday2 = '\t\t\t<atc:instruction atc:idxref="Workload1"/>\n\t\t\t<atc:instruction atc:idxref="Anxiety1"/>\n\t\t\t<atc:instruction atc:idxref="Restlong"/>\n\t\t\t<atc:instruction atc:idxref="restend"/>'

#Specifies the ATC interruption launch commands. These are the real references to open each of the eight ATC interruptions. 
launch = ['launch1.VBS','launch2.VBS','launch3.VBS','launch4.VBS','launch5.VBS','launch6.VBS','launch7.VBS','launch8.VBS']

#This specifies which scenarios are in which counterbalancing group
cGroups = [[0,8],
[1,9],
[2,10],
[3,11],
[4,12],
[5,13],
[6,14],
[7,15]]

#this is the counterbalancing scheme for the first day (latin square)
cSchemeDay1 = [[1,2,3,4,5,6,7,8],
[2,3,4,5,6,7,8,1],
[3,4,5,6,7,8,1,2],
[4,5,6,7,8,1,2,3],
[5,6,7,8,1,2,3,4],
[6,7,8,1,2,3,4,5],
[7,8,1,2,3,4,5,6],
[8,1,2,3,4,5,6,7]]

##This is for the output on the first day of testing
for subj in range(65,97):#   change the second index in the range to loop up to 65(64 participants); set to 17(16 participants) now for testing.
    cRow = cSchemeDay1[(subj-1)%8]# use the modulus operator to find out which row to use in counterbalancing

    random.shuffle(launch)# Shuffles the ATC interruption launch commands into a random order.
    datalaunch = data#Makes a copy of the scenarioList for this participant.
    for i in range(0,8):#Replaces the blunt[0-8] in the scenarioList file with a random real launcher. By using static references in scenario list, we ensure all 8 ATC interruptions occur for each participant.
        datalaunch = datalaunch.replace('blunt'+str(cRow[i])+'.vbs',str(launch[i]))
       
    inst = datalaunch.split("\n")#split scenario presentation tags
    conds = [inst[0:16],inst[16:32],inst[32:48],inst[48:64],inst[64:80],inst[80:96],inst[96:112],inst[112:128]]#Place into eight conditions, order: ATC(SS,LS,SL,LL) then None(SS,LS,SL,LL).

    scenArray = []

    # loop across scenario groups and look up their assigned interruption condition for this subj
    for group in range(0,8):
        #conds[cScheme[group]][i]
        scenArray.extend([ conds[cRow[group]-1][i] for i in cGroups[group]]) # use extend and not append here - Simon.

    # randomize order of arrays---this is something you might modify to control this a bit more - Simon
    random.shuffle(scenArray)

    #Change scene array to include workload and rest breaks, with the exception of 8 which is a long rest
    testingArray = sum(([x,restshort] if i != 8 else [x,restlong] for (i,x) in enumerate(scenArray)), [])

    #compiles all together.
    f = open('EXP3-FINLAND-P' + str(subj) + '-DAY1' + '.' + 'xml', 'w')
    f.write(MainScript1 +'\r\n\r\n'.join(testingArray) + '\n' + EndScript)
    print str(subj),"Day 1 File Complete"
    f.close()


###Day Two data is exactly same as day one, except we use cSchemeDay2 as cRow
for subj in range(65,97):
    cRow = cSchemeDay1[(subj-1)%8]# use the modulus operator to find out which row to use in counterbalancing table
    
    random.shuffle(launch)
    datalaunch = data
    
    for i in range(0,8):
        datalaunch = datalaunch.replace('blunt'+str(cRow[i])+'.vbs',str(launch[i]))
       
    inst = datalaunch.split("\n")
    conds = [inst[0:16],inst[16:32],inst[32:48],inst[48:64],inst[64:80],inst[80:96],inst[96:112],inst[112:128]]    
    conds2 = []
    conds2.extend(conds[::-1])
    scenArray = []

    # loop across scenario groups and look up their assigned interruption condition for this subj
    for group in range(0,8):
        #conds[cScheme[group]][i]
        scenArray.extend([ conds2[cRow[group]-1][i] for i in cGroups[group]]) # use extend and not append here

    # randomize order of arrays---this is something you might modify to control this a bit more
    random.shuffle(scenArray)

    #insert workload and rest breaks with the exception of 8 which is a long rest
    testingArray = sum(([x,restshortday2] if i != 8 else [x,restlongday2] for (i,x) in enumerate(scenArray)), [])
    f = open('EXP3-FINLAND-P' + str(subj) + '-DAY2' + '.' + 'xml', 'w')
    f.write(MainScript2 +'\r\n\r\n'.join(testingArray) + '\n' + EndScript)
    print str(subj),"Day 2 File Complete"
    f.close()

