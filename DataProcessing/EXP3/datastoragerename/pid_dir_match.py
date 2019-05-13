import os
import re

# Takes EXP3 directory for a directory, returns Day ID.
def extrdir_day(directory):
    day = re.search(r'(Day )([1-2])', directory).group(2) #get day
    return day

# Takes EXP3 directory for a directory, returns participant ID.
def extrdir_pid(directory):
    pid = re.search(r'(PID_)([0-9]{1,2}[B-C]{0,1})', directory).group(2) #get participant ID
    return pid

# Takes EXP3 directory for a file, returns participant ID.
def extrfile_pid(filename):
    pid = re.search(r'(PID-)([0-9]{1,2}[B-C]{0,1})', filename).group(2) #get participant ID
    return pid

# Tests if match between PID directory, and file for csv and log.
def match_between(fileid, dirid):
    if fileid == dirid:
        return True
    elif fileid in dirid:
        fileid = int(re.sub('[A-Z]', '', fileid))
        dirid = int(re.sub('[A-Z]', '', dirid))
        print("[v] Warning [v]")
        if fileid == dirid:
            return True
    else:
        return False

for root, dirs, files in os.walk('/mnt/HF-ATC/EXP3/EXP3-DATA/'):
    for file in files:
        if 'EXP3_' in file and not file.endswith('xml'):
            print(match_between(extrfile_pid(file), extrdir_pid(root)), extrfile_pid(file), extrdir_pid(root))
