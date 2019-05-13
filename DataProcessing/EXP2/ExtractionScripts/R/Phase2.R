#### FUNCTION ####
results = function(Dir,thisFile,ppTimes,pmCraft,conflicts,dismiss){
# thisFile = allFiles[1]
  
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
ATCintType = data.frame(matrix(ncol = 1, nrow = 15))
for (it in 1:length(ITind)) {
  if (is.na(as.character(rawdata[ITind[it],2]))) {
    testTypeNum[it,1] = 0
  } else {
    if (is.na(as.character(rawdata[ITind[it],6]))) {
      testTypeNum[it,1] = 1
    } else {
      testTypeNum[it,1] = 2
      k = as.character(rawdata[ITind[it],6])
      ATCintType[it,1] = as.numeric(substr(k,7,7))  
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

#### WORKLOAD (Section 3) ####
print("Workload")

workload = matrix(nrow=15,ncol=2)
recallInd = grep("Recall No",rawdata[,1])
if (length(recallInd) == 30) {
  k = as.numeric(as.character(rawdata[recallInd+1,2]))
  workload[,1] = k[seq(1, length(k), 2)]
  workload[,2] = k[seq(2, length(k), 2)]

} else {  
  workload[,1] = as.numeric(as.character(rawdata[recallInd+1,2]))
  workload[,2] = as.numeric(as.character(rawdata[recallInd+2,2]))
  # check for any NA values and rectify
  
  
  # check if the indices are correct (incase pressed more than once)
  # even
  ev = which(is.na(rawdata[recallInd+2,2]))
  if (length(ev) > 0) { # if there's blanks
    for (x in ev) { # for each missing even workload
      w = x*2
      n = 0
      while (!(rawdata[recallInd[x]+2+n,1] == w & !is.na(rawdata[recallInd[x]+2+n,2]))) {
        n = n+1
      }
      workload[x,2] = as.numeric(as.character(rawdata[recallInd[x]+2+n,2]))
    } # for each even 
  } # if
} # else workload
  
# RESULTS MATRIX
workloadResults = matrix(nrow=36,ncol=1)
# ACTind
workloadResults[1:5,1] = workload[which(testTypeNum[,1]==2),1] # WORKLOAD 1
workloadResults[16,1] = mean(workload[which(testTypeNum[,1]==2),1])
workloadResults[19:23,1] = workload[which(testTypeNum[,1]==2),2] # WORKLOAD 2
workloadResults[34,1] = mean(workload[which(testTypeNum[,1]==2),2])
# Blank
workloadResults[6:10,1] = workload[which(testTypeNum[,1]==1),1] # WORKLOAD 1
workloadResults[17,1] = mean(workload[which(testTypeNum[,1]==1),1])
workloadResults[24:28,1] = workload[which(testTypeNum[,1]==1),2] # WORKLOAD 2
workloadResults[35,1] = mean(workload[which(testTypeNum[,1]==1),2])
# None
workloadResults[11:15,1] = workload[which(testTypeNum[,1]==0),1] # WORKLOAD 1
workloadResults[18,1] = mean(workload[which(testTypeNum[,1]==0),1])
workloadResults[29:33,1] = workload[which(testTypeNum[,1]==0),2] # WORKLOAD 2
workloadResults[36,1] = mean(workload[which(testTypeNum[,1]==0),2])


#### ACCEPTANCE/HANDOFF (SECTION 4) ####
print("Acceptance/Handoff")

headers = as.matrix(rawdata[which(rawdata[,1]=="Trial" & rawdata[,2]=="Flash Time"),])

for (t in 1:length(testOrderNum[,1])) {
  # Get Pm Aircraft
  pmAC = as.character(pmCraft[testOrderNum[t,1],1])
  
  # Get the last key press
  KeyInd = which(headers[t,]=="Key Pressed")
  testStr = paste("test",testOrderNum$testOrder[t],sep = "")
  if (length(KeyInd)>1) {
    keyPressed = rawdata[which(rawdata[,1]==testStr & (rawdata[,5]=="accept" | rawdata[,5]=="handoff")),KeyInd]
    firstKey = as.character(keyPressed[,1])
    lastKey = data.frame(key = character(15), stringsAsFactors=FALSE)
    for (a in 1:length(keyPressed[,1])){
      b = (which(!is.na(keyPressed[a,])))
      f = as.integer(tail(b,1))
      if (length(f) == 0) {
        lastKey[a,1] = NA
      } else {
        lastKey[a,1] = as.character(keyPressed[a,tail(b,1)])
      }
    }
    keyPressed = lastKey
  } else {
    keyPressed = as.data.frame(rawdata[which(rawdata[,1]==testStr & (rawdata[,5]=="accept" | rawdata[,5]=="handoff")),KeyInd]) 
    firstKey = as.character(rawdata[which(rawdata[,1]==testStr & (rawdata[,5]=="accept" | rawdata[,5]=="handoff")),KeyInd])
  }
  colnames(keyPressed) = "KeyPressed"
  
  AHTime = as.data.frame(rawdata[which(rawdata[,1]==testStr & (rawdata[,5]=="accept" | rawdata[,5]=="handoff")),which(headers[t,]=="Acceptance Time (HP)")])
  colnames(AHTime) = "AccTime"
  
  firstData = as.data.frame(rawdata[which(rawdata[,1]==testStr & (rawdata[,5]=="accept" | rawdata[,5]=="handoff")),1:6])
  
  ahData = cbind(firstData,keyPressed,AHTime)
  ahData[,3]=as.numeric(as.character(ahData[,3]))
  if (length(which(ahData[,2]==0)) != 0) {
    ahData[which(ahData[,2]==0),3] = 0
  }
  
  # pre and post
  ahData$prepost = rep(0,length(ahData[,1]))
  ahData[which(as.numeric(as.character(ahData[,3])) <= (ppTimes[testOrderNum[t,1],1])),9] = 1 # Pre = 1
  ahData[which(as.numeric(as.character(ahData[,3])) >= (ppTimes[testOrderNum[t,1],2])),9] = 2 # Post = 2
  if (sum(as.numeric(as.character(ahData[,6]))) > 0) {
    ahData[which(as.numeric(as.character(ahData[,6])) == 1),9] = NA # Exclude Automatic
  }
  
  # work out which to exclude from calculations
  ahData$exclude = rep(0,length(ahData[,1]))
  ahData$exclude[which(ahData[,4]==pmAC & ahData[,5]=="handoff")] = 1 # PM aircraft
  ahData$exclude[as.numeric(grep("Arrow",firstKey))] = 1 # arrow in first key press
  ahData$exclude[which(is.na(ahData[,9]))] = 1 # between Int start and Int end
  ahData$exclude[which(ahData[,6]==1 & ahData[,5]=="accept")] = 1 # 1 in Automatic Column
  #ahData$exclude[which((is.na(ahData$AccTime) | ahData$AccTime == 0) & ahData[,6]==0)] = 1 # exclude misses
  
  # Aggregate Data
  if (t==1){
    allData = ahData
  } else {
    allData = rbind(allData,ahData)
  }
}


# Change some datatypes in allData
allData[,2] = as.numeric(as.character(allData[,2]))
allData[,3] = as.numeric(as.character(allData[,3]))
allData[,8] = as.numeric(as.character(allData[,8]))
# Get reaction time
allData$RT = allData[,8] - allData[,3]
# Set test type
allData$TestType = rep(NA,length(allData[,1]))
for (t in 1:length(testOrderNum[,1])) {
  testStr = paste("test",testOrderNum$testOrder[t],sep = "")
  allData$TestType[which(allData[,1]==testStr)]=as.numeric(testTypeNum[t,1])
}

# Misses
allData$Misses = rep(0,length(allData[,1]))
allData$Misses[which((is.na(allData$AccTime) | allData$AccTime == 0) & allData[,6]==0)] = 1 # exclude misses

acceptRT = matrix(nrow=length(testOrderNum[,1]),ncol=3)
handoffRT = matrix(nrow=length(testOrderNum[,1]),ncol=3)

for (t in 1:length(testOrderNum[,1])) {
  # Individual accept and handoff RTs
  testStr = paste("test",testOrderNum$testOrder[t],sep = "")
  acceptRT[t,1] = mean(allData[which(allData[,1]==testStr & allData[,5]=="accept" & allData[,10] == 0 & allData[,13] == 0),11])
  handoffRT[t,1] = mean(allData[which(allData[,1]==testStr & allData[,5]=="handoff" & allData[,10] == 0 & allData[,13] == 0),11])
  
  # pre
  acceptRT[t,2] = mean(allData[which(allData[,1]==testStr & allData[,5]=="accept" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 1 ),11])
  handoffRT[t,2] = mean(allData[which(allData[,1]==testStr & allData[,5]=="handoff" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 1),11])
  
  # post
  acceptRT[t,3] = mean(allData[which(allData[,1]==testStr & allData[,5]=="accept" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 2),11])
  handoffRT[t,3] = mean(allData[which(allData[,1]==testStr & allData[,5]=="handoff" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 2),11])
}

typeAccRT = matrix(nrow=6,ncol=1)
typeHoffRT = matrix(nrow=6,ncol=1)

typeAccRT[1,1] = mean(allData[which(allData[,5]=="accept" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 1 & allData[,12] == 2),11])
typeAccRT[2,1] = mean(allData[which(allData[,5]=="accept" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 1 & allData[,12] == 1),11])
typeAccRT[3,1] = mean(allData[which(allData[,5]=="accept" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 1 & allData[,12] == 0),11])

for (t in 1:3) {
  # Pre
  typeAccRT[t,1] = mean(allData[which(allData[,5]=="accept" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 1 & allData[,12] == (3-t)),11])
  typeHoffRT[t,1] = mean(allData[which(allData[,5]=="handoff" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 1 & allData[,12] == (3-t)),11])
  
  # Post
  typeAccRT[t+3,1] = mean(allData[which(allData[,5]=="accept" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 2 & allData[,12] == (3-t)),11])
  typeHoffRT[t+3,1] = mean(allData[which(allData[,5]=="handoff" & allData[,10] == 0 & allData[,13] == 0 & allData[,9] == 2 & allData[,12] == (3-t)),11])
}

missesAccIndv = matrix(nrow=length(testOrderNum[,1]),ncol=2)
missesHoffIndv = matrix(nrow=length(testOrderNum[,1]),ncol=2)

missesAcc = matrix(nrow=6,ncol=1)
missesHoff = matrix(nrow=6,ncol=1)

# Individual misses
for (t in 1:length(testOrderNum[,1])) {
  testStr = paste("test",testOrderNum$testOrder[t],sep = "")
  
  #pre
  missesAccIndv[t,1] = length(which(allData[,1]==testStr & allData[,5]=="accept" & allData[,10] == 0 & allData[,9] == 1 & allData[,13] == 1))  
  missesHoffIndv[t,1] = length(which(allData[,1]==testStr & allData[,5]=="handoff" & allData[,10] == 0 & allData[,9] == 1 & allData[,13] == 1))
  
  #post
  missesAccIndv[t,2] = length(which(allData[,1]==testStr & allData[,5]=="accept" & allData[,10] == 0 & allData[,9] == 2 & allData[,13] == 1))
  missesHoffIndv[t,2] = length(which(allData[,1]==testStr & allData[,5]=="handoff" & allData[,10] == 0 & allData[,9] == 2 & allData[,13] == 1))
}


# Overall misses
for (t in 1:3) {
  # Pre
  missesAcc[t,1] = (length(which(allData[,5]=="accept" & allData[,12] == (3-t) & allData[,10] == 0 & allData[,9] == 1 & allData[,13] == 1))) /
    (length(which(allData[,5]=="accept" & allData[,12] == (3-t) & allData[,9] == 1 & allData[,10] == 0)) + 
       length(which(allData[,5]=="accept" & allData[,12] == (3-t) & allData[,9] == 1 & allData[,13] == 1)))
  
  missesHoff[t,1] = (length(which(allData[,5]=="handoff" & allData[,12] == (3-t) & allData[,10] == 0 & allData[,9] == 1 & allData[,13] == 1))) /
    (length(which(allData[,5]=="handoff" & allData[,12] == (3-t) & allData[,9] == 1 & allData[,10] == 0)) + 
       length(which(allData[,5]=="handoff" & allData[,12] == (3-t) & allData[,9] == 1 & allData[,13] == 1)))
  
  # Post
  missesAcc[t+3,1] = (length(which(allData[,5]=="accept" & allData[,12] == (3-t) & allData[,10] == 0 & allData[,9] == 2 & allData[,13] == 1))) /
    (length(which(allData[,5]=="accept" & allData[,12] == (3-t) & allData[,9] == 2 & allData[,10] == 0)) + 
       length(which(allData[,5]=="accept" & allData[,12] == (3-t) & allData[,9] == 2 & allData[,13] == 1)))
  
  missesHoff[t+3,1] = (length(which(allData[,5]=="handoff" & allData[,12] == (3-t) & allData[,10] == 0 & allData[,9] == 2 & allData[,13] == 1))) /
    (length(which(allData[,5]=="handoff" & allData[,12] == (3-t) & allData[,9] == 2 & allData[,10] == 0)) + 
       length(which(allData[,5]=="handoff" & allData[,12] == (3-t) & allData[,9] == 2 & allData[,13] == 1)))
}    


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
resConf = cbind(resConf,as.numeric(rep(NA,length(resConf[,1]))),as.numeric(rep(0,length(resConf[,1]))))
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
  id = which((allConf[,4] == resConf[n,2] | allConf[,4] == resConf[n,3]) & allConf[,1] == resConf[n,1])
  if (length(id) > 0) { # got it
    id=id[1] # only use first time pressed
    allConf[id,5] = 1
    if (allConf[id,3] > as.numeric(resConf[n,4]) & allConf[id,3] < (as.numeric(resConf[n,5]))) { # greater than T1 and < T2
      resConf[n,7] = allConf[id,3] - as.numeric(resConf[n,4])
    } else if (allConf[id,3] > (as.numeric(resConf[n,5]))) { # greater than T2 
      resConf[n,7] = as.numeric(resConf[n,5]) - as.numeric(resConf[n,4]) # reaction time
      resConf[n,8] = 1 # missed
    } else { # less than T1
      allConf[id,5] = NA
    }  
  } else { # missed
    resConf[n,7] = as.numeric(resConf[n,5]) - as.numeric(resConf[n,4])
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

#### N-BACK TASK SCORES (SECTION 6) ####
# print("NBack")
# 
# NbInd = grep("Stimuli Number",rawdata[,2])
# NbInd = NbInd[Nback]
# NbScores = matrix(,nrow=0,ncol=4)
# 
# for (ni in 1:5) {
#   NbInd2 = which(is.na(rawdata[NbInd[ni]:(NbInd[ni]+20),2])) + NbInd[ni]
#   NbInd2 = NbInd2[1] - 2
#   tempNb = as.matrix(rawdata[(NbInd[ni]+1):NbInd2[1],1:4])  
#   if (length(which(tempNb[2:4,3] == tempNb[1,3])) == 3) {
#     tempNb[1:4,3] = NA
#   }
#   NbScores = rbind(NbScores,tempNb)
# }
# 
# Nbtest = unique(NbScores[,1])
# Nbresponse = c("hit","miss","correct_rejection","false_alarm")
# NbResults = matrix(NA,nrow=6,ncol=4)
# 
# # record tests with NA
# NbNA = matrix(unique(NbScores[which(is.na(NbScores[,3])),1]),nrow=1)
# 
# for (ni in 1:5) {
#   for (nr in 1:4) {
#     NbResults[ni,nr] = length(which(NbScores[,1] %in% Nbtest[ni] & NbScores[,4] %in% Nbresponse[nr]))
#     if (ni == 5) {# get the average
#       NbResults[6,nr] = mean(NbResults[1:5,nr])
#     }
#   }
# }


#### DISMISS REJECTION CHECK (SECTION 7) ####
print("Dismiss")

# Extract dismess raw data
dissInd = grep("Callsign",rawdata[,2])
dissData = matrix(,nrow=0,ncol=4)

for (d in 1:length(dissInd)) {
  if (!is.na(rawdata[(dissInd[d]+1),1])){
    dissData = rbind(dissData,rawdata[(dissInd[d]+1):(dissInd[d]+2),1:4])
  }
}

dissData[,3] = as.numeric(as.character(dissData[,3]))
dissData[,4] = as.numeric(as.character(dissData[,4]))

if (length(which(is.na(dissData[,1]))) > 0) {
  naInd = which(is.na(dissData[,1]))
  dissData[naInd,1] = dissData[naInd-1,1]
  dissData[naInd,2:4] = 0
}

# change to 0 is missing
if (length(which(is.na(dissData[,3]))) > 0) {
  dissData[is.na(dissData[,3]),4] = 0
  dissData[is.na(dissData[,3]),3] = 0
}
dissPM = matrix(,nrow=15,ncol=1)
dissRS = matrix(,nrow=15,ncol=1)
dPM = 1
dRS = 1

for (e in 1:length(dissData[,1])) {
  thisDiss = dismiss[which(dismiss[,1] == as.character(dissData[e,1])),]
  w = which(thisDiss == as.matrix(dissData[e,2]))
  if (length(w) == 0) {
    if (length(grep(thisDiss[1,2],dissData[grep(thisDiss[1,1],dissData[,1]),2])) == 0) {
      dissPM[dPM,1] = 0
      dPM = dPM + 1
    } else if (length(grep(thisDiss[1,4],dissData[grep(thisDiss[1,1],dissData[,1]),2])) == 0) {
      dissRS[dRS,1] = 0
      dRS = dRS + 1
    }
  } else if (w == 2) {
    dissPM[dPM,1] = as.numeric(as.character(dissData[e,4]))
    dPM = dPM + 1
  } else if (w == 4){
    dissRS[dRS,1] = as.numeric(as.character(dissData[e,4]))
    dRS = dRS + 1
  }
}


#### COMPILE RESULTS ####
allResults[1,1] = part

# Workload
allResults[1,2:37] = workloadResults

# Conflict Reaction Time
# Handoff
allResults[1,38:52] = c(handoffRT[which(testTypeNum==2),2],handoffRT[which(testTypeNum==1),2],handoffRT[which(testTypeNum==1),2])
allResults[1,53:55] = typeHoffRT[1:3,1]
allResults[1,56:70] = c(handoffRT[which(testTypeNum==2),3],handoffRT[which(testTypeNum==1),3],handoffRT[which(testTypeNum==1),3])
allResults[1,71:73] = typeHoffRT[4:6,1]
# Accept
allResults[1,74:88] = c(acceptRT[which(testTypeNum==2),2],acceptRT[which(testTypeNum==1),2],acceptRT[which(testTypeNum==0),2])
allResults[1,89:91] = typeAccRT[1:3,1]
allResults[1,92:106] = c(acceptRT[which(testTypeNum==2),3],acceptRT[which(testTypeNum==1),3],acceptRT[which(testTypeNum==0),3])
allResults[1,107:109] = typeAccRT[4:6,1]

# Accept/Handooff Misses
allResults[1,110:124] = missesHoffIndv[,1]
allResults[1,125:127] = missesHoff[1:3,1]
allResults[1,128:142] = missesHoffIndv[,2]
allResults[1,143:145] = missesHoff[4:6,1]
allResults[1,146:160] = missesAccIndv[,1]
allResults[1,161:163] = missesAcc[1:3,1]
allResults[1,164:178] = missesAccIndv[,2]
allResults[1,179:181] = missesAcc[4:6,1]

# CDT Times
allResults[1,182:199] = CDT[,1]
allResults[1,200:217] = CDT[,2]
allResults[1,218:227] = CDT[1:10,3]
allResults[1,228:229] = CDT[16:17,3]

# CDT Misses
allResults[1,230:247] = missCDT[,1]
allResults[1,248:265] = missCDT[,2]
allResults[1,266:275] = missCDT[1:10,3]
allResults[1,276:277] = missCDT[16:17,3]

# Conflict False Alarms
allResults[1,278:295] = confFA

# NBack Task Scores
# allResults[1,296:315] = matrix(NbResults[1:5,], nrow = 1)
# allResults[1,316:319] = NbResults[6,]

# PM Dismiss
allResults[1,296:310] = c(dissPM[which(testTypeNum==2),1],dissPM[which(testTypeNum==1),1],dissPM[which(testTypeNum==0),1])
allResults[1,311:325] = c(dissRS[which(testTypeNum==2),1],dissRS[which(testTypeNum==1),1],dissRS[which(testTypeNum==0),1])

# ATC Launch Num
allResults[1,326:330] = ATCintType[which(testTypeNum==2),1]

# Add Slope for ms calculations
allResults[1,331] = slope

# Add Tests with NA in NBack
# if (length(NbNA) > 0) {
#   allResults[1,351:(350+length(NbNA))] = NbNA
# }
return(allResults)
}

#### CYCLE THROUGH FILES ####
library(xlsx)
options(warn=1)

# Get the files
allFiles = list.files(path = Dir, pattern = "csv")

#### INPUT SHEETS NEEDED ####
# Results headings
headings = as.matrix(read.xlsx(paste(Dir,"Phase2_Input.xlsx",sep = ""),sheetName="outputsheet_phase2.5",header=FALSE))
headings = cbind(headings,"ms conversion")

# Pre/post times
ppTimes = read.xlsx(paste(Dir,"Phase2_Input.xlsx",sep = ""),sheetName="resumption_sheet2.5",colIndex=c(8,9))

# PM Aircraft
pmCraft = read.xlsx(paste(Dir,"Phase2_Input.xlsx",sep = ""),sheetName="PM_sheet1.5",colIndex=2)

# Conflict data
conflicts = read.xlsx(paste(Dir,"Phase2_Input.xlsx",sep = ""),sheetName="conflictdata_phase2")

# Import dismiss data
dismiss = as.data.frame(read.xlsx(paste(Dir,"Phase2_Input.xlsx",sep = ""),sheetName="dismiss_check2.5"))

compiledResults = as.data.frame(matrix(,nrow=length(allFiles),ncol = 331))
names(compiledResults) = headings

for (f in 1:length(allFiles)) {
  print(paste("File",f,":",allFiles[f]))
  compiledResults[f,] = results(Dir,allFiles[f],ppTimes,pmCraft,conflicts,dismiss)
  #compiledResults[f,which(is.na(compiledResults[f,351:365]))+350] = ""
}

write.xlsx(compiledResults,paste(R_Dir,"Phase2Results.xlsx",sep=""),col.names=TRUE,row.names=FALSE)

print("The results file is now ready")

## 2. Make a 'run' file for Michael
