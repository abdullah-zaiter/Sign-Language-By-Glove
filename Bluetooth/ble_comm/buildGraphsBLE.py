
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

name = 'Com_Sensores'
# name = 'Sem_Sensores'

def graphAcc(y, x, y2=None):
    # x = np.arange(100)
    fig = plt.figure()
    ax = plt.subplot(111)
    savefig = ''
    if (y2 is None) :
        ax.plot(x, y, label='$'+name)
        plt.ylabel('Pacotes Recebidos(%)')
        savefig = 'img/pacotesBLE'+name+'.png'
    else:
        # ax.plot(x, y, 'r',label='Com_Sensores')
        # ax.plot(x, y2, 'b',label='Sem_Sensores')
        plt.ylabel('Pacotes Recebidos(%)')
        savefig = 'img/pacotesBLE.png'
    plt.title('Pacotes Recebidos x Delay Entre 100 Pacotes de 30 bytes')
    plt.xlabel('Delay(us)')

    plt.legend()
    fig.savefig(savefig)


    plt.show()

def graphPyTime(y, x, y2=None):
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
    graphAcc(acerto, delay)
    # graphPyTime(tempDec, delay)