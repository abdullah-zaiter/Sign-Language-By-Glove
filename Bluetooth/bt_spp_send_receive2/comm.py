"""
A simple Python script to send messages to a sever over Bluetooth using
Python sockets (with Python 3.3 or above).
"""

import socket

serverMACAddress = 'C0:38:96:A2:3C:98'
port = 3
s = socket.socket(socket.AF_BLUETOOTH, socket.SOCK_STREAM, socket.BTPROTO_RFCOMM)
while 1:
    text = input()
    if text == "quit":
        break
    s.send(bytes(text, 'UTF-8'))
s.close()

# """
# A simple Python script to send messages to a sever over Bluetooth
# using PyBluez (with Python 2).
# """

# import bluetooth

# serverMACAddress = 'C0:38:96:A2:3C:98'
# port = 3
# s = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
# s.connect((serverMACAddress, port))
# while 1:
#     text = raw_input() # Note change to the old (Python 2) raw_input
#     if text == "quit":
#         break
#     s.send(text)
# sock.close()