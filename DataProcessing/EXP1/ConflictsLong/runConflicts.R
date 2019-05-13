# Set the directory to the Phase2.R (and associated) files are. 
# This is also where the results file will be created
R_Dir = "/Users/janellebraithwaite/Documents/Programming/Psych/2015/Michael Macro/Phase2 Versions/Conflict_Results/"

# Set the directory of the raw data files
Dir = "/Users/janellebraithwaite/Documents/Programming/Psych/2015/Michael Macro/Phase2 Versions/Conflict_Results/"
  
# Run the script that calculates results
source(paste(R_Dir,"Conflicts.R",sep=""))




#####################
#### INFORMATION ####
# This R code will cycle through all the csv files in the Dir folder and 
# output a results file in that same folder.

# You will need to place the following in the R_Dir folder
# 1. runConflicts.R
# 2. Conflicts.R
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
# The results file will output to the R_Dir with the file name "ConflictResults.xlsx"