import serial
import serial.tools.list_ports
import os

ComPorts = list(serial.tools.list_ports.comports())
FileName = "Pomiary/HydrivePomiaryPradu_0.txt"


for port in ComPorts:
    if "BTHENUM" in port.hwid:
        StartOfAdress = port.hwid.rfind("&")
        EndOfAdress = port.hwid.rfind("_")

        adress = port.hwid[StartOfAdress + 1:EndOfAdress]

        if int(adress, 16):
            OutGoing = port
            print("port wychodzacy: ", port.name)

        else:
            Incoming = port
            print("port przychodzacy: ", port.name)

excample = serial.Serial('COM9', 9600)

while os.path.isfile(FileName):
    nazwa, rozszerzenie = os.path.splitext(FileName)

    numer = int(nazwa[-1]) + 1
    FileName = ''.join([nazwa[:-1], str(numer), rozszerzenie])


f = open(FileName, "w")

while 1:
    pomiar = excample.readline()
    print(pomiar)

    f = open(FileName, "a")
    f.write(str(pomiar))
    f.write('\n')
    f.close()
