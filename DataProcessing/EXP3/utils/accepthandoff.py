import csv
import pandas as pd
import re
from collections import defaultdict

TRIAL = re.compile(r'(ATC|None)([L-S]{1})([L-S]{1})([0-9]{1,2})')

def removekey(d, key):
    '''
    Takes list d and key for removal;
    returns list d without key
    '''
    r = dict(d)
    del r[key]
    return r

def getscenario(trialstring):
    if TRIAL.search(trialstring).group(4):
        return TRIAL.search(trialstring).group(4)

def gettrialinfodict(trialstring):
    """
    Takes a trialstring; returns trial info
    ---------------------------------------
    string - trialstring
    string - interruption
    string - encode, retrieve
    string - scenario
    string - trialtype
    """
    trial_string = TRIAL.search(trialstring)
    try:
        trialinfo = {
            "trialstring" : trialstring,
            "interruption": trial_string.group(1),
            "encode": trial_string.group(2),
            "retrieve": trial_string.group(3),
            "scenario": trial_string.group(4),
            "trialtype": trial_string.group(2) + trial_string.group(3)}
        return trialinfo
    except AttributeError:
        pass

def getpminfo(filename="settings/exp3-pminfo.csv"):
    '''
    Generates PMINFO dict from settings file
    ----------------------------------------
    trial,condition,day1,day2,key,interruptiontime,encode_s,encode_l,ret_s,ret_l

    Call PMINFO with keys for trial and short or long condition.
    '''
    pmdict = defaultdict(dict)
    with open(filename) as f:
        for row in csv.DictReader(f):
            trialvals = removekey(row, 'trial')
            pmdict[row['trial']] = trialvals
        return pmdict
PMINFO = getpminfo("exp3-pminfo.csv")



aho = open("handoffaccepts.txt")
ah = csv.reader(aho)

conditionstrings = [i[0] for i in ah]
dats = []


for handacc in ah:
    t = gettrialinfodict(handacc[0])
    ft = handacc[1]
    cs = handacc[3]
    task = handacc[4]  
    dats.append({
        'callsign' : cs,
        'task' : task,
        'flashtime' : ft,
        'scenario' : t['scenario'],
        'enc_s' : PMINFO[t['scenario']]['encode_s'],
        'enc_l' : PMINFO[t['scenario']]['encode_l'],
        'ret_s' : PMINFO[t['scenario']]['ret_s'],
        'ret_l' : PMINFO[t['scenario']]['ret_l'],
        })

pd.DataFrame(dats)

print(pd)



    # if t['encode_s'] <= ft <= t['ret_s']:
    #     ss += 1
    # if t['encode_s'] <= ft <= t['ret_l']:
    #     sl += 1
    # if t['encode_l'] <= ft <= t['ret_s']:
    #     ls += 1
    # if t['encode_l'] <= ft <= t['ret_l']:
    #     ll += 1