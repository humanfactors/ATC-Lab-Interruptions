import unittest
import csv
from collections import defaultdict

def confinfo(day):
    if day == 1 or day == '1':
        return _getconfinfo("../settings/confinfo_day1.csv")
    elif day == 2 or day == '2':
        return _getconfinfo("../settings/confinfo_day2.csv")

def _getconfinfo(filename):
    """CONFINFO['1']['C44'] = [altitude,conflictTime,acceptanceTime,pair, partner]"""
    conflicts = defaultdict(dict)
    with open(filename) as f:
        for row in csv.DictReader(f):
            if row['acnum'] == '1':
                c = Conflict(row['pair'])
                c.scenario, c.t1, c.t2 = row['scenario'], row['t1'], row['t2']
                c.aircraft = [row['callsign'], row['partner']]
                c.altitudes[row['callsign']] = row['altitude']
            elif row['acnum'] == '2':
                c.altitudes[row['callsign']] = row['altitude']
                conflicts[row['pair']] = c
    return conflicts

class Conflict(object):
    """docstring for Conflict."""
    def __init__(self, pairid):
        super(Conflict, self).__init__()
        self.pairid = pairid
        self.scenario = str()
        self.aircraft = []
        self.altitudes = {}
        self.t1 = float()
        self.t2 = float()

confs = confinfo('1')
# print(confs)

for k, v in confs.items():
    print(vars(v))