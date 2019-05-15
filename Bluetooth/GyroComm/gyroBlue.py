import serial
import time


def decodeRaw2Int(input_data):
    x,y,z = input_data.decode().split(',')
    return (x,y,z)


if __name__ == "__main__":
    print("Start")
    port="/dev/rfcomm0" #This will be different for various devices and on windows it will probably be a COM port.

    bluetooth=serial.Serial(port, 115200)#Start communications with the bluetooth unit

    print("Connected")
    bluetooth.flushInput() #This gives the bluetooth a little kick
    
    while True:
        input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
        #input_data = input_data[:-1]
        #print(type(input_data), input_data, len(input_data))
        (x,y,z) = decodeRaw2Int(input_data)
        print("\taccel, x, y, z: ", x ,",", y,",", z,"\t" )#These are bytes coming in so a decode is needed
        input_data=bluetooth.readline()#This reads the incoming data. In this particular example it will be the "Hello from Blue" line
        (x,y,z) = decodeRaw2Int(input_data)

        print("\tgyro, x, y, z: ", x ,",", y,",", z,"\t" )#These are bytes coming in so a decode is needed
        print("\n")
        time.sleep(0.1) #A pause between bursts
    bluetooth.close() #Otherwise the connection will remain open until a timeout which ties up the /dev/thingamabob
    print("Done")

