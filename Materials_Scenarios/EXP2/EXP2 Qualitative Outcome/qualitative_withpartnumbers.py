import os
import re
import csv
#defines question strings

q1 ="Questions\n1. Describe how you felt when you were interrupted."
q1a ="a. How did this differ between different forms of interruptions you were exposed to (i.e. blank vs filled)?"
q2 ="2. Describe what (if any) strategies you used when you were interrupted?"
q2a ="a. How did these differ between different forms of interruptions you were exposed to (i.e. blank vs filled)?"

with open('qualitativeoutput.csv', 'wb') as csvfile:
    qualcsv = csv.writer(csvfile, delimiter=',')
    for file in os.listdir("."):
        if file.endswith(".txt"):
            filename = file
            filenamecut = filename.split("EXP2part")[-1].split(".txt")[0]
            print filenamecut
            #Starts the actual script calling stuff
            q = open(file, 'rU')
            #sets qualData to be read and opened
            qualData = q.read()
            #simple check to proof all questions still remain in file
            if q1 and q1a and q2 and q2a in qualData:
                print "All question strings remain"
            else:
                print "There was a string missing..."
            #Now to extract Q1 data
            q1out = qualData.split(q1)[-1].split(q1a)[0]
            q1outclean = q1out.replace('\n', '')
            print q1outclean
            #and Q1A data
            q1aout = qualData.split(q1a)[-1].split(q2)[0]
            q1aoutclean = q1aout.replace('\n', '')
            print q1aoutclean
            #and Q2 data
            q2out = qualData.split(q2)[-1].split(q2a)[0]
            q2outclean = q2out.replace('\n', '')
            print q2outclean
            #and finally Q2A data
            q2aout = qualData.rsplit(q2a)[-1].rsplit(q2)[0]
            q2aoutclean = q2aout.replace('\n', '')
            print q2aoutclean
            #finally print all of them together in the CSV file
            qualcsv.writerow([str(filenamecut)] + [str(q1outclean)] + [str(q1aoutclean)]+ [str(q2outclean)]+ [str(q2aoutclean)])
            q.close()

