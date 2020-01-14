from __future__ import print_function
from bluepy import btle
import sys
import time
    #For Time ellapsed analysis

import matplotlib
import matplotlib.pyplot as plt
import numpy as np

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

name = 'Com_Sensores'
# name = 'Sem_Sensores'

def graphAcc(y, x):
    # x = np.arange(100)
    fig = plt.figure()
    ax = plt.subplot(111)
    ax.plot(x, y, label='$'+name)

    plt.title('Pacotes Recebidos x Delay Entre 100 Pacotes de 30 bytes')
    plt.xlabel('Delay(us)')
    plt.ylabel('Pacotes Recebidos(%)')
    fig.savefig('img/pacotesBLE'+name+'.png')

def graphPyTime(y, x):
    # x = np.arange(100)
    fig = plt.figure()
    ax = plt.subplot(111)
    ax.plot(x, y, label='$'+name)

    plt.title('Tempo Decorrido de Captura x Delay Entre 100 Pacotes de 30 bytes')
    plt.xlabel('Delay(us)')
    plt.ylabel('Tempo da Captura de Python(s)')
    fig.savefig('img/pyCaptTime'+name+'.png')
    plt.show()

def saveToFile(nameFile, y):
    np.save(nameFile+name+'array.npy', y)    # .npy extension is added if not given
    print("File Saved")

def loadFromFile(nameFile):
    d = np.load(nameFile+name+'array.npy')
    return d

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
    acerto = np.zeros((100,1))
    tempDec = np.zeros((100,1))
    
    
    conn = connect(ESP32_MAC, ADD_TYPE)
    try:        
        svc = findService(SERVICE_UUID, conn)
        chrs = findCharacteristics([CHARACTERISTIC_SEND_UUID ,CHARACTERISTIC_RECEIVE_UUID],svc)
        print(chrs[1].read())
        chrs[0].write(str.encode("start"))
        
        leitAtual = 0
        while(leitAtual < 100):
            totalSum = 0
            lastnum = 0
            num = 0
            t = time.time()
            print("Leitura[",leitAtual,"]")
            while(num <= 200):
                aux = chrs[1].read()
                newAux = bytearray(aux)
                num = int.from_bytes(newAux[7:8], signed = True,byteorder='little')
                if num == -56:
                    # Value to end current package count
                    aux = chrs[1].read()
                    break
                for j in range(30):
                    num2 = int.from_bytes(newAux[j:j+1], signed = True,byteorder='little')
                    aux_print = "{0:0=4d}".format(num2)
                if lastnum!= num:
                    totalSum += 1     
                lastnum = num
                
            
            acc = (totalSum)/100
            # print("Acerto: ", acc)
            acerto[leitAtual] = acc
            elapsed_time = time.time() - t
            tempDec[leitAtual] = elapsed_time

            # tempDec = np.insert(tempDec, leitAtual, elapsed_time, axis=1)
            # print("Tempo decorrido: ", elapsed_time)
            print("\n")
            leitAtual+=1    
        
        acerto = np.reshape(acerto, (100, ))
        tempDec = np.reshape(tempDec, (100, ))
        # print(acerto)
        # print("Tempo decorrido: ", tempDec) 
        delay = np.arange(200000, 200,-1998)
        # print('Delay ', delay)
        graphAcc(acerto, delay)
        graphPyTime(tempDec, delay)
        print('Loading Files')
        saveToFile('delay', delay)
        saveToFile('acerto', acerto)
        saveToFile('TempoDec', tempDec)
    finally:
        conn.disconnect()
    pass
