# Set the directory to the Phase2.R (and associated) files are. 
# This is also where the results file will be created
R_Dir = "/Users/janellebraithwaite/Documents/Programming/Psych/Michael Macro/Phase2_v4/"

# Set the directory of the raw data files
Dir = "/Users/janellebraithwaite/Documents/Programming/Psych/Michael Macro/Phase2_v4/"
  
# Run the script that calculates results
source(paste(R_Dir,"Phase2.R",sep=""))


#####################
#### INFORMATION ####
# This R code will cycle through all the csv files in the Dir folder and 
# output a results file in that same folder.

# You will need to place the following in the R_Dir folder
# 1. runPhase2.R
# 2. Phase2.R
# 3. Phase2_Input.xlsx

# To RUN the scripts:
# 1. Set the R_Dir variable (line 3). Make sure a filename separator ("/") is at the end.
# 2. Set the Dir variable (line 9). Again, this needs a filename separator at the end
# 3. Execute lines 3,6,9 in the console by:
        # - place cursor in a single line and press Ctrl + Enter (or click run button)
        # - highlight lines 3 to 9 and press Ctrl + Enter (or click the run button)
        # - copy and paste the relevant lines into the console (slow way)

# 4. The code will then run through the files outputting informtion for debugging 
#    (See DEBUGGING below) and will end with: "The results file is now ready"

# RESULTS
# The results file will output to the R_Dir with the file name "Phase2Results.xlsx"
# Note: this will be overwritten if you run this script and the file already exists
# I've added some extra columns at the end for your information
      # 'ms conversion' (ML) = slope conversion to convert seconds to ms
      # 'NBack NA' = the columns after this will list any tests where values were removed
      #      from the 'N-Back TestScores' were excluded because of consecutive repetition

# DEBUGGING
# R isn't the best when it comes to debugging so I've set up the code to print strings 
# to the console to help track down the bug
# As the code runs it will print the file name:
        # e.g. "File 5 : ATC_Golf_Participant19.csv"
# And the the name of code sections
        # "Workload"
        # Acceptance/Handoff"
        # "Conflicts"
        # "NBack"
        # "Dismiss"

# If the code crashes please email me:
        # The relevant file
        # The name of the section where it crashed
        # The error message

# Email me if you get stuck :)



