import re
from os.path import basename
import os

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


DISMISS = re.compile(r'((ATC|None)[L-S]{1}[L-S]{1}[0-9]{1,2}ID,)([A-Z]{1,2}[0-9]{1,2},)(\d+,)(\d+)')


for root, dirs, files in os.walk("/home/michael/phd/exp3-datafinal/"):
    for file in files:
        if file.endswith(".csv") and "FINLAND" in file:
            pid = fromfilepath(file)
            day = fromfilepath(file, value="day")
            o = open(root + os.path.sep + file)
            for line in o:
                if DISMISS.search(line):
                    diss = DISMISS.search(line)
                    dismissinfo = diss.group(0)
                    trialinfo = ",".join([pid, day, dismissinfo])
                    print(trialinfo)

            

