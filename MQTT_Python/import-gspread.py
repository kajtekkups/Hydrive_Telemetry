import gspread
import time

sa = gspread.service_account()
sh = sa.open("Pomiary").sheet1
while(True):
    with open('HydrivePomiaryPradu.txt', 'r') as f:
        dane = f.read().split("\n")[:-1]
        values = []
        for row in dane:
            try:
                values.append(row.split(" ")[2])
            except IndexError:
                continue
                
        values = [[value] for value in values] # Convert values into 2-dimensional list
        sh.update('A2:A%s' % (len(values)+1), values)
        time.sleep(1)
        print("dziala")