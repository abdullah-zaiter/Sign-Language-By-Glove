
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

def graphAcc(x, y, y2 = None, name=None):

    fig = plt.figure()
    ax = plt.subplot(111)
    savefig = ''
    if (y2 is None) :
        ax.plot(x, y,'r.', label=name)
        savefig = 'img/pacotesBLE'+name+'.png'
    else:
        ax.plot(x, y, 'r.', label='Com_Sensores')
        ax.plot(x, y2, 'b.',label='Sem_Sensores')
        savefig = 'img/pacotesBLE.png'

    plt.ylabel('Pacotes Recebidos(%)')
    plt.title('Pacotes Recebidos x Delay Entre 100 Pacotes de 30 bytes')
    plt.xlabel('Delay(us)')

    plt.legend()
    fig.savefig(savefig)
    # plt.show()

def graphPyTime(x, y, y2=None, name=None):
    fig = plt.figure()
    ax = plt.subplot(111)
    savefig = ''
       
    if (y2 is None) :
        ax.plot(x, y,'r.', label=name)
        savefig = 'img/pyCaptTime'+name+'.png'
    else:
        ax.plot(x, y, 'r.',label='Com_Sensores')
        ax.plot(x, y2, 'b.',label='Sem_Sensores')
        savefig = 'img/pyCaptTime.png'

    plt.title('Tempo Decorrido de Captura x Delay Entre 100 Pacotes de 30 bytes')
    plt.xlabel('Delay(us)')
    plt.ylabel('Tempo da Captura de Python(s)')
    
    plt.legend()
    fig.savefig(savefig)
    plt.show()

def loadFromFile(nameFile):
    d = np.load('data/'+nameFile+name+'array.npy')
    print('Reading from' + 'data/'+nameFile+name+'array.npy')
    return d

if __name__ == "__main__":

    
    name = 'Com_Sensores'
    delay = loadFromFile('delay')
    acerto = loadFromFile('acerto')
    tempDec = loadFromFile('TempoDec')

    name = 'Sem_Sensores'
    delaySS = loadFromFile('delay')
    acertoSS = loadFromFile('acerto')
    tempDecSS = loadFromFile('TempoDec')

    name = 'Todos'
    
    if(name == 'Com_Sensores'):
        graphAcc(delay, acerto, name)
        graphPyTime(delay, tempDec, name)
    elif(name == 'Sem_Sensores'):
        graphAcc(delay, acertoSS, name)
        graphPyTime(delay, tempDecSS, name)
    else:
        graphAcc(delay, acerto, acertoSS)
        graphPyTime(delay, tempDec, tempDecSS)