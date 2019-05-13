import csv
import re
import subprocess
import io
import os
from os.path import basename
from collections import defaultdict

from atcregex import TRIAL

def fromfilepath(filepath, value="id"):
    """ Returns ID or Day from filename
    getfilenameinfo("EXP3-FINLAND-P08-DAY2.csv", "day")
    """
    bname = basename(filepath)
    filename, file_extension = os.path.splitext(bname)
    namesplit = filename.split(sep="-")
    if value == "id":
        return namesplit[2].replace("P","")
    if value == "day":
        return namesplit[3].replace("DAY","")
    else:
        raise NameError("Error in filename, please check manually.")

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

def removekey(d, key):
    '''
    Takes list d and key for removal;
    returns list d without key
    '''
    r = dict(d)
    del r[key]
    return r

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
            condition = removekey(row, 'condition')
            trialvals = removekey(row, 'scenario')
            pmdict[row['scenario']][row['condition']] = trialvals
        return pmdict
PMINFO = getpminfo("settings/exp3-pminfo.csv")