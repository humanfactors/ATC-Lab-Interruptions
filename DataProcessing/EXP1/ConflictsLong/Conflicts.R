#### FUNCTION ####
results = function(Dir,thisFile,ppTimes,pmCraft,conflicts,dismiss){
#thisFile = allFiles[1]
  
# Results matrix
allResults = as.data.frame(matrix(,nrow=1,ncol = 331))

# get the participant number
part = as.numeric(gsub("[^0-9]+","",substring(thisFile, 7, last = 1000000L)))

# import file
rawdata = read.csv(paste(Dir,thisFile,sep=""),header = FALSE,
                   na.strings=c("","NA"),blank.lines.skip=FALSE,
                   col.names = paste("V", seq_len(50), sep = ""))

#### CONVERT MS TIMINGS ####
y = as.matrix(which(grepl("test*",rawdata[,1]) & !grepl("[[:alpha:]]", as.character(rawdata[,2]))))
sec = as.matrix(as.numeric(as.character(rawdata[y,2])))
ms = as.matrix(as.numeric(as.character(rawdata[y,3])))
ms[which(sec==0)]=0

mslm <- lm(ms ~ 0 + sec)
slope = summary(mslm)$coefficients[1,1]

ppTimes = ppTimes * slope

conflicts[,4] = conflicts[,4] * slope
conflicts[,5] = conflicts[,5] * slope

#### TEST ORDERS ####
# test number order
testOrderChar = as.character(unique(rawdata[grep("test*",rawdata[,1]),1]))
testOrderNum = data.frame(as.numeric(gsub("[^0-9]+","",testOrderChar)))
colnames(testOrderNum) = "testOrder"

# get interruption type: 0 = none, 1 = Interruption, 2 = ATCint

ITind = grep("Interruption Type",rawdata[,2])+1
testTypeNum = data.frame(matrix(ncol = 1, nrow = 15))
testType = data.frame(matrix(ncol = 1, nrow = 15))
ATCintType = data.frame(matrix(ncol = 1, nrow = 15))
for (it in 1:length(ITind)) {
  if (is.na(as.character(rawdata[ITind[it],2]))) {
    testTypeNum[it,1] = 0
    testType[it,1] = 'None'
  } else {
    if (!is.na(as.character(rawdata[ITind[it],6]))) {
      testTypeNum[it,1] = 2
      k = as.character(rawdata[ITind[it],6])
      ATCintType[it,1] = as.numeric(substr(k,7,7))
      testType[it,1] = k
    }
    else if (as.character(rawdata[ITind[it],2])=="Interruption") {
      testTypeNum[it,1] = 1
      testType[it,1] = 'Blank'
    } else if (as.character(rawdata[ITind[it],2])=="NBack") {
      testTypeNum[it,1] = 2
      testType[it,1] = 'NBack'
    }
  }
}

colnames(testTypeNum) = "testType"

# get order of interruption types
testTypeOrder = testTypeNum
colnames(testTypeOrder) = "typeOrder"
testTypeOrder[which(testTypeNum[,1]==0),1] = c(1,2,3,4,5)
testTypeOrder[which(testTypeNum[,1]==1),1] = c(1,2,3,4,5)
testTypeOrder[which(testTypeNum[,1]==2),1] = c(1,2,3,4,5)


#### CONFLICTS (SECTION 5) ####
print("Conflicts")

ConInd = as.numeric(grep("Intervene Time",rawdata[,1]))

for (t in 1:length(testOrderNum[,1])) {
  Ind2 = ConInd[t] + match(NA,rawdata[ConInd[t]:(ConInd[t]+15),1]) - 2
  conTemp = rawdata[(ConInd[t]+1):Ind2,1:3]
  if (t == 1) {
    allConf = as.data.frame(cbind(rep(testOrderNum[t,1],length(conTemp[,1])),conTemp,rep(NA,length(conTemp[,1]))))
  } else if (!is.na(conTemp[1,1])) {
    allConf = rbind(allConf,as.data.frame(cbind(rep(testOrderNum[t,1],length(conTemp[,1])),conTemp,rep(NA,length(conTemp[,1])))))
  }
}
colnames(allConf) = c("V1","V2","V3","V4","V5")

resConf = as.matrix(conflicts[which(conflicts[,10]==0),c(1,2,3,4,5,9)])
resConf = cbind(resConf,as.numeric(rep(NA,length(resConf[,1]))),as.numeric(rep(0,length(resConf[,1]))),as.numeric(rep(0,length(resConf[,1]))),as.numeric(rep(0,length(resConf[,1]))),as.numeric(rep(NA,length(resConf[,1]))),as.numeric(rep(0,length(resConf[,1]))))
# change column types
resConf[,1] = as.numeric(as.character(resConf[,1]))
resConf[,4] = as.numeric(as.character(resConf[,4]))
allConf[,4] = as.character(allConf[,4])
allConf[,3] = as.numeric(as.character(allConf[,3]))

# find resumption aircraft and ignore

for (q in 1:length(allConf[,1])) {
  if (length(which(conflicts[,1]==allConf[q,1] & 
                     (conflicts[,2]==allConf[q,4] | conflicts[,3]==allConf[q,4]) & conflicts[,10]==1)) > 0) {
    allConf[q,5] = 0
  }
}

# find reaction times for each conflict aircraft

for (n in 1:length(resConf[,1])) {
  resConf[n,9] = testOrderNum[resConf[n,1],1]
  resConf[n,10] = testType[resConf[n,1],1]
  id = which((allConf[,4] == resConf[n,2] | allConf[,4] == resConf[n,3]) & allConf[,1] == resConf[n,1])
  if (length(id) > 0) { # got it
    id=id[1] # only use first time pressed
    allConf[id,5] = 1
    resConf[n,11] = allConf[id,3]
    if (allConf[id,3] > as.numeric(resConf[n,4]) & allConf[id,3] < (as.numeric(resConf[n,5]))) { # greater than T1 and < T2
      resConf[n,7] = allConf[id,3] - as.numeric(resConf[n,4])
    } else if (allConf[id,3] > (as.numeric(resConf[n,5]))) { # greater than T2 
      resConf[n,7] = NA # reaction time NA
      resConf[n,8] = 1 # missed
      resConf[n,11] = NA
    } else { # less than T1
      allConf[id,5] = NA # reaction time NA
      resConf[n,12] = 1 # False Alarm
      resConf[n,11] = NA
    }  
  } else { # missed
    resConf[n,7] = NA # reaction time NA
    resConf[n,8] = 1 # missed
  }
}

#ACTind1_PreInt_CDT
ACTind = which(testTypeNum[,1] == 2)
Blank = which(testTypeNum[,1] == 1)
None = which(testTypeNum[,1] == 0)

CDT = matrix(NA,nrow=18,ncol=3)
missCDT = matrix(NA,nrow=18,ncol=3)
confFA = matrix(NA,nrow=18,ncol=1)
# Pre | Post | Overlap
typeCDT = c("No Overlap (Pre-Int)","No overlap (Post-Int)","Overlap - Conflict After")

for (v in 1:3) { # PreIntCDT, PostIntCDT, OverlapCDT
  for (r in 1:5) {
    # Conflict Detection Times
    CDT[r,v] = mean(as.numeric(resConf[resConf[,1] == ACTind[r] & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],7]))
    CDT[r+5,v] = mean(as.numeric(resConf[resConf[,1] == Blank[r] & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],7]))
    CDT[r+10,v] = mean(as.numeric(resConf[resConf[,1] == None[r] & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],7]))
    
    # Misses
    missCDT[r,v] = sum(as.numeric(resConf[resConf[,1] == ACTind[r] & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],8]))
    missCDT[r+5,v] = sum(as.numeric(resConf[resConf[,1] == Blank[r] & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],8]))
    missCDT[r+10,v] = sum(as.numeric(resConf[resConf[,1] == None[r] & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],8]))
    
  }
  # Conflict Detection Times
  CDT[16,v] = mean(as.numeric(resConf[as.numeric(resConf[,1]) %in% ACTind & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],7]))
  CDT[17,v] = mean(as.numeric(resConf[as.numeric(resConf[,1]) %in% Blank & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],7]))
  CDT[18,v] = mean(as.numeric(resConf[as.numeric(resConf[,1]) %in% None & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],7]))
  
  # Misses
  missCDT[16,v] = mean(as.numeric(resConf[as.numeric(resConf[,1]) %in% ACTind & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],8]))
  missCDT[17,v] = mean(as.numeric(resConf[as.numeric(resConf[,1]) %in% Blank & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],8]))
  missCDT[18,v] = mean(as.numeric(resConf[as.numeric(resConf[,1]) %in% None & !is.na(resConf[,7]) & resConf[,6] == typeCDT[v],8]))
}

# Conflict False Alarms
for (r in 1:5) {
  confFA[r,1] = length(which(is.na(allConf[which(allConf[,1] == ACTind[r]),5])))
  confFA[r+5,1] = length(which(is.na(allConf[which(allConf[,1] == Blank[r]),5])))
  confFA[r+10,1] = length(which(is.na(allConf[which(allConf[,1] == None[r]),5])))
}
confFA[16,1] = length(which(is.na(allConf[which(allConf[,1] %in% ACTind),5]))) / 
  length(allConf[which(allConf[,1] %in% ACTind),5])

confFA[17,1] = length(which(is.na(allConf[which(allConf[,1] %in% Blank),5]))) / 
  length(allConf[which(allConf[,1] %in% Blank),5])

confFA[18,1] = length(which(is.na(allConf[which(allConf[,1] %in% None),5]))) / 
  length(allConf[which(allConf[,1] %in% None),5])

## NEW RESULTS
newResults = as.data.frame(matrix(,nrow=45,ncol = 12))
# Conflict ID
newResults[,1] = conflicts[,11]
# ms conversion
newResults[,2] = slope
# Participant
newResults[,3] = part
# Trial Number
newResults[,4] = conflicts[,1]
# Order Number
newResults[,5] = resConf[,9]
# Condition
newResults[,6] = resConf[,10]
# Type
newResults[,7] = resConf[,6]
# T1_ms
newResults[,8] = resConf[,4]
# T2_ms
newResults[,9] = resConf[,5]
# response_time_ms
newResults[,10] = resConf[,11]
# Miss
newResults[,11] = resConf[,8]
# FA
newResults[,12] = resConf[,12]

return(newResults)
}


#### CYCLE THROUGH FILES ####
library(xlsx)
options(warn=1)

# Get the files
allFiles = list.files(path = Dir, pattern = "csv")

#### INPUT SHEETS NEEDED ####
# Results headings
newheadings = cbind("Conflict ID", "ms conversion","Participant","Trial Number","Order Number","Condition","Type","T1_ms","T2_ms","response_time_ms","miss","FA")

# Pre/post times
ppTimes = read.xlsx(paste(Dir,"Phase2_Input.xlsx",sep = ""),sheetName="resumption_sheet2.5",colIndex=c(8,9))

# PM Aircraft
pmCraft = read.xlsx(paste(Dir,"Phase2_Input.xlsx",sep = ""),sheetName="PM_sheet1.5",colIndex=2)

# Conflict data
conflicts = read.xlsx(paste(Dir,"Phase2_Input.xlsx",sep = ""),sheetName="conflictdata_phase2")
conflicts = cbind(conflicts[,2:11],conflicts[,1]) # reshuffled the order of this so that column referencing in the function remains the same

# Import dismiss data
dismiss = as.data.frame(read.xlsx(paste(Dir,"Phase2_Input.xlsx",sep = ""),sheetName="dismiss_check2.5"))

newCompResults = as.data.frame(matrix(,nrow=length(allFiles)*45,ncol = 12))
names(newCompResults) = newheadings

for (f in 1:length(allFiles)) {
  print(paste("File",f,":",allFiles[f]))
  i1 = (f-1)*45+1
  i2 = i1+44
  newCompResults[i1:i2,] = results(Dir,allFiles[f],ppTimes,pmCraft,conflicts,dismiss)
}

write.xlsx(newCompResults,paste(R_Dir,"ConflictResults.xlsx",sep=""),col.names=TRUE,row.names=FALSE)

print("The results file is now ready")

