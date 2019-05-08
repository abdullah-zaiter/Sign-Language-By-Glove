import serial
from showAllPorts import *


###TODO CHOOSE ARDUINO PORT BETTER
#def getPort():
#	print(serial_ports())
#	arduinoPort = input("Which port is the Arduino connected?")
#	return arduinoPort
#
#arduinoPort = getPort()
#arduino = serial.Serial(arduinoPort, 115200, timeout=.1)
arduino = serial.Serial('/dev/ttyACM0', 115200, timeout=.1)

while True:
	data = arduino.readline()[:-2] #the last bit gets rid of the new-line chars
	if data:
		msg = data.decode('utf-8')
		print (msg)