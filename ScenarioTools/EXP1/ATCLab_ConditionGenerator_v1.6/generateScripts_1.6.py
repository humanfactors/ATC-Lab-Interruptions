import random

# General comment: some of the script might be confusing because python
# uses zero-based numbering to index arrays

# read in the full list of scenario x conditions
f = open('scenarioList.xml', 'U')
data = f.read()
f.close()
inst = data.split("\n\n")

# read in the main portion of the data file (all before scenariolist)
f = open('interruptionstest1.xml', 'U')
MainScript = f.read()
f.close()

# read the final few lines at the end of the file
f = open('interruptionstest2.xml', 'U')
EndScript = f.read()
f.close()

# This specifies which scenarios are in which counterbalancing group
cGroups = [[0,3,6,9,12],
[1,4,7,10,13],
[2,5,8,11,14]]

conds = [inst[0:15],inst[15:30],inst[30:45]] # the xml strings divided up by condition

# this is the counterbalancing scheme (latin square)
cScheme = [[1,2,3],
[1,3,2],
[2 ,1 , 3],
[2 ,  3 ,  1],
[3 ,  1  , 2],
[3,   2 ,  1]]

# change the second index in the range to loop up to 60; set to 12 now for testing
for subj in range(1,61): 

    cRow = cScheme[(subj-1)%6] # use the modulus operator to find out which row to use in counterbalancing table

    scenArray = []

    # loop across scenario groups and look up their assigned interruption condition for this subj
    for group in range(0,3):
        #conds[cScheme[group]][i]

        scenArray.extend([ conds[cRow[group]-1][i] for i in cGroups[group]]) # use extend and not append here

    # randomize order of arrays---this is something you might modify to control this a bit more
    random.shuffle(scenArray)

    #insert workload and rest breaks with the exception of 8 which is a long rest
    testingArray = sum(([x,'\t\t\t<atc:instruction atc:idxref="Workload1"/>\n\t\t\t<atc:instruction atc:idxref="Anxiety1"/>\n\t\t\t<atc:instruction atc:idxref="Rest"/>'] if i != 7 else [x,'\t\t\t<atc:instruction atc:idxref="Workload1"/>\n\t\t\t<atc:instruction atc:idxref="Anxiety1"/>\n\t\t\t<atc:instruction atc:idxref="Restlong"/>'] for (i,x) in enumerate(scenArray)), [])

    f = open('ATC_Golf_Participant' + str(subj) + '.' + 'xml', 'w')
    f.write(MainScript + '\r\n\r\n'.join(testingArray) + '\n' + EndScript)
    f.close()
