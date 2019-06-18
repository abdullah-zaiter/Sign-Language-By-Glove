from __future__ import print_function
from bluepy import btle
import sys

__author__ = "Abdullah Zaiter"
__copyright__ = "Copyright 2019, Sign-Language-By-Glove"
__license__ = "MIT"
__version__ = "1.0"


CHARACTERISTIC_SEND_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"
CHARACTERISTIC_RECEIVE_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26aa"
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
ESP32_MAC = '80:7d:3a:99:df:66'
ADD_TYPE = 'public'

def connect(mac, add_type):
    print(f"Connecting to: {mac}, address type: {add_type}")
    tries = -1
    conn = None
    while tries < 3:
        tries += 1
        try:
            conn = btle.Peripheral(mac, add_type)
        except btle.BTLEDisconnectError:
            print(f"Failed, trying again..{tries+1}")
            continue
        break
    
    if conn is None:
        print("ERROR: Cant find device. Check the MAC address and adress type and try again")
        exit()
    else:
        print("Connected!")
        return conn

def findService(srv_uuid ,peripheral):
    service = None 
    for srv in peripheral.services:
        if (srv.uuid.getCommonName() == srv_uuid):
            service = srv
            pass
        pass
    if service is None:
        print(f"ERROR: Cant find service '{srv_uuid}'... Check the code used on your ble device")
        exit()
    else:
        print(f"Service '{srv_uuid}' Found!")
        return service

def findCharacteristics(uuids, service):
    characteristics = []
    for uuid in uuids:
        aux = service.getCharacteristics(forUUID=uuid)
        if len(aux) != 0:
            characteristics.append(aux[0])
            pass
        pass
    if len(characteristics) != len(uuids):
        print("WARNING: Some characteristics couldnt be found, check the uuids and the code on your ble device")
        pass
    return characteristics

if __name__ == "__main__":
    conn = connect(ESP32_MAC, ADD_TYPE)
    try:        
        svc = findService(SERVICE_UUID, conn)
        chrs = findCharacteristics([CHARACTERISTIC_SEND_UUID ,CHARACTERISTIC_RECEIVE_UUID],svc)
        print(chrs[1].read())
        chrs[0].write(str.encode("start"))
        while True:
            print(int.from_bytes(chrs[1].read(), byteorder='big'))
        # for ch in svc.getCharacteristics():
        #     print(f"\t{ch}, hnd={hex(ch.handle)}, supports {ch.propertiesToString()}")
        #     if (ch.supportsRead()):
        #         try:
        #             print("\t->", repr(ch.read()))
        #         except btle.BTLEException as e:
        #             print("\t->", e)

    finally:
        conn.disconnect()
    pass
