import serial
import time
import math


def decodeRaw2Int(input_data):
    x,y,z = input_data.decode().split(',')
    #x = int(x)
    #y = int(y)
    #z = int(z)
    return (x,y,z)


def fuseData(x,y,z):
    pass

def filterData(x,y,z):
    weight = 0.2
    #xfil = (1-weight)*filteredData + weight*newData

def getAngle(Ax,Ay,Az):

    x = int(Ax)
    y = int(Ay)
    z = int(Az)

    roll = math.atan2(y,z)#(X)
    pitch = math.atan2(x,math.sqrt((y*y) + (z*z))) * (90/79) #(Y)

    #converting radians into degrees
    pitch = pitch * (180.0/3.14)
    roll = roll * (180.0/3.14) 
    return (pitch, roll)


def oldmain():
    print("Start")
    port="/dev/rfcomm1" #This will be different for various devices and on windows it will probably be a COM port.

    bluetooth=serial.Serial(port, 115200)#Start communications with the bluetooth unit

    print("Connected")
    bluetooth.flushInput() #This gives the bluetooth a little kick
    try:
        while True:
            input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
            print(input_data)
            inputD = input_data.decode('utf-8')
            print(inputD)
            if(inputD[:5] == "Begin"):
               
                for i in range(0,2):
                    print("======= Sensor",i+1,"========")
                    
                    input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
                    (x,y,z) = decodeRaw2Int(input_data)
                    print("\n")
                    print("accel x, y, z: ", x ,",", y,",", z,"\t" )#These are bytes coming in so a decode is needed
                    pitch, roll = getAngle(x,y,z)

                    input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
                    (x,y,z) = decodeRaw2Int(input_data)
                    print("gyro x, y, z: ", x ,",", y,",", z )#These are bytes coming in so a decode is needed
                    print("Pitch(Y): ",pitch, " \nRoll(X): ", roll)
                    print("\n")
                
                input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
                delta = input_data.decode()
                print("RunTime: ", delta)#These are bytes coming in so a decode is needed
            
            time.sleep(0.2) #A pause between bursts
    except Exception as ex:
        print(ex)
        bluetooth.close() #Otherwise the connection will remain open until a timeout which ties up the /dev/thingamabob
        print("Done")




if __name__ == "__main__":
    print("Start")
    port="/dev/rfcomm1" #This will be different for various devices and on windows it will probably be a COM port.

    bluetooth=serial.Serial(port, 115200)#Start communications with the bluetooth unit

    print("Connected")
    bluetooth.flushInput() #This gives the bluetooth a little kick

    try:
        while True:
            input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
            print(input_data)
            inputD = input_data.decode('utf-8')
            print(inputD)

    except Exception as ex:
        print(ex)
        bluetooth.close() #Otherwise the connection will remain open until a timeout which ties up the /dev/thingamabob
        print("Done")