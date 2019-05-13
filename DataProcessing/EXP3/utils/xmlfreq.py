import xml.etree.ElementTree as ET
import csv
import pandas


# Create Pandas Dataframe

class Scenario(object):
    def __init__(self):
        numberofaircraft = 0


tree = ET.parse('EXP3_AltFixFinland_Day2_Fix_PID-6.xml')
root = tree.getroot()

# fileopen = open('confinfo_day2.csv', 'w')
# write = csv.writer(fileopen)

# header = ['trial', 'callsign', 'altitude', 'conflictTime', 'acceptanceTime', 'pair']
# write.writerow(header)
# conflicts = root.findall("*//{atc}sky[{atc}conflictDetails]")
# for conflict in conflicts:
#     callsign = conflict.attrib['{atc}idx']
#     altitude = conflict.find("./{atc}altitude").text
#     confinfo = conflict.find("./{atc}conflictDetails")
#     conftime = confinfo.find('{atc}conflictTime').text
#     conftrial = confinfo.find('{atc}trial').text
#     conft1 = confinfo.find('{atc}acceptanceTime').text
#     conft2 = confinfo.find('{atc}conflictTime').text
#     confother = confinfo.find('{atc}conflictAircraft').text
#     write.writerow([conftrial, callsign, altitude, conft2, conft1, confother])

# fileopen.close()

trials = root.findall("*//{atc}sky")

trial_lengths = []
for trial in trials:
    x = []
    scenarioname = (trial.attrib['{atc}idx']).replace("LL","")
    for ac in (trial.findall("{atc}aircraft")):
        x.append(ac)
    # print(x)
    print(scenarioname, len(x), sep=',')