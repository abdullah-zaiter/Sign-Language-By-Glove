import matplotlib
import matplotlib.pyplot as plt
import numpy as np

# name = 'Com_Sensores'
name = 'Sem_Sensores'


def saveToFile(nameFile, y):
    np.save(nameFile+name+'array.npy', y)    # .npy extension is added if not given

def loadFromFile(nameFile):
    d = np.load(nameFile+name+'array.npy')
    return d

def graphAcc(y, x):
    # x = np.arange(100)
    fig = plt.figure()
    ax = plt.subplot(111)
    ax.plot(x, y, label='$'+name)

    plt.title('Pacotes Recebidos x Delay Entre 100 Pacotes de 30 bytes')
    plt.xlabel('Delay(us)')
    plt.ylabel('Pacotes Recebidos(%)')
    # fig.savefig('img/pacotesBLE'+name+'.png')

def graphPyTime(y, x):
    # x = np.arange(100)
    fig = plt.figure()
    ax = plt.subplot(111)
    ax.plot(x, y, label='$'+name)

    plt.title('Tempo Decorrido de Captura x Delay Entre 100 Pacotes de 30 bytes')
    plt.xlabel('Delay(us)')
    plt.ylabel('Tempo da Captura de Python(s)')
    # fig.savefig('img/pyCaptTime'+name+'.png')
    plt.show()

if __name__ == "__main__":
    acerto = np.zeros((100,1))
    tempDec = np.zeros((100,1))

    leitAtual = 0
    while(leitAtual < 100):
        totalSum = leitAtual
        lastnum = 66

        print("Leitura[",leitAtual,"]")
        
            
        
        acc = (totalSum)/100
        # print("Acerto: ", acc)
        acerto[leitAtual] = acc
        # acerto = np.insert(acerto, leitAtual, acc, axis=1)
        # print(acerto)
        elapsed_time = 53
        tempDec[leitAtual] = elapsed_time

        # tempDec = np.insert(tempDec, leitAtual, elapsed_time, axis=1)
        # print("Tempo decorrido: ", tempDec)
        print("\n")
        leitAtual+=1  
    
    acerto = np.reshape(acerto, (100, ))

    tempDec = np.reshape(tempDec, (100, ))
    print(acerto)
    print("Tempo decorrido: ", tempDec) 

    delay = np.arange(200000, 200,-1998)
    print('Delay ', delay)



    saveToFile('delay', delay)
    saveToFile('acerto', acerto)
    saveToFile('TempoDec', tempDec)
    print('Opening Files')

    delay = loadFromFile('delay')
    acerto = loadFromFile('acerto')
    tempDec = loadFromFile('TempoDec')


    graphAcc(acerto, delay)
    graphPyTime(tempDec, delay)