import os
import re

basedirectory = '/mnt/HF-ATC/EXP3/EXP3-DATA/'
checklist = []

# Takes EXP3 directory for a directory, returns day number.
def extr_day(directory):
    day = re.search(r'(Day )([1-2])', directory).group(2) #get day
    return day

# Takes EXP3 directory for a directory, returns participant ID.
def extr_pid(directory):
    pid = re.search(r'(PID_)([0-9]{1,2}[B-C]{0,1})', directory).group(2) #get participant ID
    return pid

# renames a finland file
def rename_fl(filename, directory):
    """Renames main trial strings"""
    ext = re.split('_|\.', filename)[-1]
    pid = extr_pid(directory)
    day = extr_day(directory)
    newfilename = 'EXP3-FINLAND-P%s-DAY%s.%s' % (pid, day, ext)
    return newfilename

def rename_bp(filename, directory):
    """Renames interruption trial strings"""
    ext = re.split('_|\.', filename)[-1]
    bpn = re.split('_|\.', filename)[1]
    pid = extr_pid(directory)
    day = extr_day(directory)
    newfilename = 'EXP3-BULLPUP%s-P%s-DAY%s.%s' % (bpn, pid, day, ext)
    return newfilename

# Test to check every renamed filename is unique
# for root, dirs, files in os.walk(basedirectory):
#     for file in files:
#         if 'Finland' in file and not file.endswith('xml'):
#             if rename_fl(file, root) in checklist:
#                 raise EnvironmentError
#             else:
#                 checklist.append(rename_fl(file, root))
#             print('OK')

# Renames files for git master commit 2003dc2
# for root, dirs, files in os.walk(basedirectory):
#     for file in files:
#         if 'Finland' in file and not file.endswith('xml'):
#             newfilename = rename_fl(file, root)
#             newfilepath = os.path.join(root, newfilename)
#             oldfilepath = os.path.join(root, file)
#             os.rename(oldfilepath, newfilepath)

# Rename the bullpup interruption files master commit dccb0a5
# for root, dirs, files in os.walk(basedirectory):
#     for file in files:
#         if 'Random' in file and not file.endswith('xml'):
#             newfilename = rename_bp(file, root)
#             newfilepath = os.path.join(root, newfilename)
#             oldfilepath = os.path.join(root, file)
#             os.rename(oldfilepath, newfilepath)
