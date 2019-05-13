import csv


output = 'aircraft_list_V7FN.csv'
logcsv = 'EXP3_FIN_PID_shortbox-both_Denmark_V7.xml.csv'

##acceptlines = []
##handofflines = []

def getlines(input,type):
    acceptlines = []
    handofflines = []
    with open(input,'U') as f:
        for row in f:
            if type is 'accept':
                if 'accept' in row:
                    acceptlines.append(row.split(",",))
            if type is 'handoff':
                if 'handoff' in row:
                    handofflines.append(row.split(",",))
        if type is 'accept':
            for row in acceptlines:
                print row[0],",",row[3],",",row[1],",",row[4]
        if type is 'handoff':
            for row in handofflines:
                print row[0],",",row[3],",",row[1],",",row[4]

getlines(logcsv, 'handoff')
getlines(logcsv, 'accept')
