#!/usr/bin/env python3

import experimentinfo

class Trial():
    def __init__(self, trialstring):
        self.trialstring = trialstring
        self.ordernumber = str()
        self.conditioninfo(trialstring)
        self.deferredhandoffinfo(trialstring)
        
    def conditioninfo(self, trialstring):
        einf = experimentinfo.gettrialinfodict(trialstring)
        self.trialstring = einf['trialstring']
        self.interruption = einf['interruption']
        self.retrieve = einf['retrieve']
        self.encode = einf['encode']
        self.scenario = einf['scenario'] # Scenario Number
        self.trialtype = einf['trialtype'] # LS, LL, SL, SS
        return self

    def deferredhandoffinfo(self, trialstring):
        self.conditioninfo(trialstring)
        pinf = experimentinfo.PMINFO[self.scenario][self.retrieve]
        self.pmcorectkey = pinf['key']
        self.interruptiontime = int(pinf['interruptiontime'])
        return self

    def writeout(self):
        return self