import os

os.getcwd()

for i in range(1,65):
    filename = "PID_"+str(i).zfill(2)
    os.mkdir(filename)
