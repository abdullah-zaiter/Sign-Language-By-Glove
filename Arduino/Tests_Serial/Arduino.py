import serial

class Arduino():
    def __init__(self,Port='/dev/ttyUSB0',Boud=9600,connState=0): 
        self.parent=self
        self.port=Port
        self.boud=Boud
        self.connState=connState
        self.timeount=1
        self.ser=None
        self.connect()

    def connect(self): 
        try:
            self.ser=serial.Serial(self.port,self.boud,timeout=0.0001)
            self.connState=1
            return [1,'connect']
        except:
            self.connState=0
            return [0,'no hardware found']


    def loadData(self):     
        self.buffer=self.ser.read(1)        
        if (self.buffer!=''):
            try:
                print (self.buffer)
            except Exception:
                pass

    def readLineWByteConversion(self):
        data = self.ser.readline()[:-2] #the last bit gets rid of the new-line chars
        if data:
            msg = data.decode('utf-8')
            print (msg)
    

ard=Arduino('/dev/ttyACM0', 115200)

while True:
    if ard.connState:
        #ard.loadData()
        ard.readLineWByteConversion()

    else:
        print ("Arduino not found")
        break

