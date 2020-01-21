
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

name = 'Com_Sensores'
# name = 'Sem_Sensores'

def graphAcc(x, y, y2 = None):
    fig = plt.figure()
    ax = plt.subplot(111)
    savefig = ''
    if (y2 is None) :
        ax.plot(x, y, label='$'+name)
        savefig = 'img/pacotesBLE'+name+'.png'
    else:
        ax.plot(x, y, 'r',label='Com_Sensores')
        ax.plot(x, y2, 'b',label='Sem_Sensores')
        savefig = 'img/pacotesBLE.png'

    plt.ylabel('Pacotes Recebidos(%)')
    plt.title('Pacotes Recebidos x Delay Entre 100 Pacotes de 30 bytes')
    plt.xlabel('Delay(us)')

    plt.legend()
    fig.savefig(savefig)
    # plt.show()

def graphPyTime(x, y, y2=None):
    fig = plt.figure()
    ax = plt.subplot(111)
    savefig = ''
       
    if (y2 is None) :
        ax.plot(x, y, label='$'+name)
        savefig = 'img/pyCaptTime'+name+'.png'
    else:
        ax.plot(x, y, 'r',label='Com_Sensores')
        ax.plot(x, y2, 'b',label='Sem_Sensores')
        savefig = 'img/pyCaptTime.png'

    plt.title('Tempo Decorrido de Captura x Delay Entre 100 Pacotes de 30 bytes')
    plt.xlabel('Delay(us)')
    plt.ylabel('Tempo da Captura de Python(s)')
    
    plt.legend()
    fig.savefig(savefig)
    plt.show()

def saveToFile(nameFile, y):
    np.save('data/'+nameFile+name+'array.npy', y)    # .npy extension is added if not given

def loadFromFile(nameFile):
    d = np.load('data/'+nameFile+name+'array.npy')
    print('Reading from' + 'data/'+nameFile+name+'array.npy')
    return d


if __name__ == "__main__":

    # saveToFile('delay', delay)
    # saveToFile('acerto', acerto)
    # saveToFile('TempoDec', tempDec)
    delay = loadFromFile('delay')
    acerto = loadFromFile('acerto')
    tempDec = loadFromFile('TempoDec')


    name = 'Sem_Sensores'
    print('sem Sensores')

    delaySS = loadFromFile('delay')
    acertoSS = loadFromFile('acerto')
    tempDecSS = loadFromFile('TempoDec')

    # graphAcc(acerto, delay, acertoSS)
    graphAcc(delay, acerto,  acertoSS)
    graphPyTime(delay, tempDec, tempDecSS)