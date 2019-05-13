#V1: Specify an output and an input file. Script will replace all instances of "Normal" and "Conflict" in script with A formatted one. Then it checks outputted file to ensure it was correct.

counter = 0
counter1 = 0
counter2 = 0

inputfile = 'exp3_nobox_renamed.xml'
outputfile = 'exp3_short_renamed_standard.xml'

#Specify file you want to output to
o = open(outputfile,'w')

with open(inputfile,'U') as f:
   for line in f:
      if "Normal" in line:
         counter = counter +1
         line = '\t\t\t<!-- ==============Normal================= -->\n'
         print "Trial",counter,"Normal Tag Fixed"
      elif "====Conflicts===" in line:
         counter2 = counter2 +1
         line = '\t\t\t<!-- ==============Conflicts========== -->\n'
         print "Trial",counter,"Conflict Tag Fixed"
      o.write(line)
o.close()


#Checker
##counter = 0
##with open(outputfile,'U') as f:
##     for line in f: 
##        if "<!-- ==============Normal================= -->" in line:
##            counter = counter +1
##            print "Trial",counter," Normal"," Checked"
