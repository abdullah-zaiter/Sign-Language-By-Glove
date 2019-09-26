from __future__ import print_function
from bluepy import btle
import sys
import time
    #For Time ellapsed analysis

__author__ = "Abdullah Zaiter"
__copyright__ = "Copyright 2019, Sign-Language-By-Glove"
__license__ = "MIT"
__version__ = "1.0"


CHARACTERISTIC_SEND_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#CHARACTERISTIC_RECEIVE2_0_UUID = "5b0c6f3f-4968-4851-8003-5ac7e184cd0e"
#CHARACTERISTIC_RECEIVE2_1_UUID = "2afe407a-963d-4806-b293-f8c3abee4a95"
#CHARACTERISTIC_RECEIVE2_2_UUID = "b6af070b-9c1c-4ad4-a5b8-2aaf7730047e"
#CHARACTERISTIC_RECEIVE2_3_UUID = "3c6f02ec-4037-4ac1-afc8-ee407f5f130f"
#CHARACTERISTIC_RECEIVE2_4_UUID = "6eed4e5b-7dbb-4a05-b1b1-b350ddd099a4"


CHARACTERISTIC_RECEIVE_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26aa"
SERVICE_UUID = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#ESP32_MAC = '80:7d:3a:99:df:66'
ESP32_MAC = 'c4:4f:33:0c:c8:5B'#'C4:4F:33:0C:C8:5B'#'24:0A:C4:96:B1:C2'

ADD_TYPE = 'public'
SUM1TO100 = 4950


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

        
        t = time.time()
        
        sum = 0
        lastnum = 0
        num = 0
        while(num < 99):
        #while True:
            #print(i)
            aux = chrs[1].read()
            newAux = bytearray(aux)

            num = int.from_bytes(newAux[7:8], signed = True,byteorder='little')
            for j in range(30):
            #j = 7
                num2 = int.from_bytes(newAux[j:j+1], signed = True,byteorder='little')
                aux_print = "{0:0=4d}".format(num2)
                if j == 7:
                    print("|", end = "")
                print(aux_print, end = ",")
                if j == 7:
                    print("|", end = "")
                

            if lastnum!= num:
                sum += num             
            lastnum = num
            print("\n")
        error = (sum)/SUM1TO100
        print("Acerto: ", error)
        elapsed_time = time.time() - t
        print(elapsed_time)

            #print("ELAPSED TIME: ", elapsed_time)

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
