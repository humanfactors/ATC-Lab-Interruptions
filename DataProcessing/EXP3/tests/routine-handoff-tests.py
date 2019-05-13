import csv
from io import StringIO

HANDOFFROWS = StringIO("""10,1,NoneSL1ID,71,71964.9,BA57,handoff,0,74,75354.7,H,74,75515.3,74,75515.4,0,0,380,
10,1,NoneSL1ID,76,77034.8,BA82,handoff,0,77,78835.1,H,78,79133.8,78,79133.9,0,,380,
10,1,NoneSL1ID,234,237245,EK68,handoff,0,240,243745,H,240,244138,240,244138,0,,380,
10,1,NoneSL1ID,222,225077,GA37,handoff,0,225,228922,H,226,229210,226,229210,0,,380,
10,1,NoneSL1ID,187,189587,GA81,handoff,0,188,190866,Down Arrow,189,191685,189,191685,0,,380,
10,1,NoneSL1ID,22,22278.5,MH03,handoff,0,23,23427.6,H,23,23547.8,23,23547.9,0,,380,
10,1,NoneSL1ID,43,43573.1,MH63,handoff,0,43,44498.6,H,44,45019.6,44,45019.8,0,,380,
10,1,NoneSL1ID,86,87175,MH75,handoff,0,88,89323,H,88,89419.2,88,89419.3,0,,380,
10,1,NoneSL1ID,245,248400,QF12,handoff,0,246,249585,H,246,249818,246,249818,0,,380,
10,1,NoneSL1ID,271,274763,QR21,handoff,0,272,276761,H,273,276863,273,276863,0,,380,
10,1,NoneSL1ID,89,90216.3,QR39,handoff,0,90,91642.9,H,90,91739.1,90,91739.3,0,,380,
10,1,NoneSL1ID,130,131790,QR84,handoff,0,132,133914,H,132,134139,132,134139,0,,380,
10,1,NoneSL1ID,204,206825,SQ02,handoff,0,205,208737,H,206,208914,206,208914,0,,380,
10,1,NoneSL1ID,160,162210,TT26,handoff,0,161,164034,H,162,164305,162,164305,0,,380,
10,1,NoneSL9ID,294,298083,BA47,handoff,0,295,299968,H,296,300184,296,300184,0,1,155,
10,1,NoneSL9ID,37,37486.5,CX05,handoff,0,41,42290.7,H,42,42620.8,42,42621,0,,155,
10,1,NoneSL9ID,43,43570.5,GA25,handoff,0,53,54706.5,H,54,55058.9,54,55059,0,,155,
10,1,NoneSL9ID,200,202769,JQ90,handoff,0,201,204610,Left Arrow,202,205313,202,205314,0,,155,
10,1,NoneSL9ID,222,225076,JQ93,handoff,0,0,-1,,,,,,1,,155,
""")


class RoutineHandoffs():
    def __init__(self, data):
        self.data = data
        for row in self.data:
            self.process_routine_response(row)

    def process_routine_response(self, row):
        self.routho = {'correct': 0, 'falsealarm': 0, 'miss' : 0, 
        'flashtime_sec' : row[3], 'flashtime_ms' : float(row[4]), 'responsetime' : None, 'actiontime' : None}
        if row[7] == '1':
            self.routho = {x: "NA" for x in self.routho}
        elif any("Arrow" in x for x in row):
            self.routho['falsealarm'] = 1
        elif row[10] == 'H':
            self.routho["correct"] = 1
            self.routho['responsetime'] = float(row[12])
            self.routho['actiontime'] = float(row[12]) - float(row[4])
        elif row[9] == "-1":
            self.routho['miss'] = 1
        elif row[10] != "H":
            self.find_last_response(row)
        return [self.routho['correct'], self.routho['falsealarm'], self.routho['miss'], self.routho['flashtime_sec'], self.routho['actiontime']]

    def find_last_response(self, row):
        lastitemint = max(loc for loc, val in enumerate(row) if val == 'H')
        if row[lastitemint] == "H":
            self.routho["correct"] = 1
            self.routho['actiontime'] = float(row[lastitemint+2])
            self.routho['responsetime'] = float(row[lastitemint+2]) - float(row[4])
        else:
            pass

def main():
    csvr = csv.reader(HANDOFFROWS)
    r = RoutineHandoffs(csvr)

if __name__ == '__main__':
    main()