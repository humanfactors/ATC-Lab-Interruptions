import re

INTERVENTION = re.compile("level_variation")
MSTIME = re.compile(r'<hpc_elapsed_time>(.+?)</hpc_elapsed_time>')
CALLSIGN = re.compile(r'<call>(.+?)</call>')
NEWFL = re.compile(r'<new_cfl>([0-9]+)</new_cfl>')
OLDFL = re.compile(r'<old_cfl>([0-9]+)</old_cfl>')
TRIAL = re.compile(r'(ATC|None)([L-S]{1})([L-S]{1})([0-9]{1,2})')
WORKLOAD = re.compile(r'^[0-9]{1,2}$')
