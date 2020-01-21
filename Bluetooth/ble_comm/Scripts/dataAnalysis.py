
import matplotlib
import matplotlib.pyplot as plt
import numpy as np

def loadFromFile(nameFile):
    d = np.load('../data/'+nameFile+name+'array.npy')
    print('Reading from' + '../data/'+nameFile+name+'array.npy')
    return d

def diff(x, y, y2, name):
    newY = abs(y - y2)
    fig = plt.figure()
    ax = plt.subplot(111)

    ax.plot(x, newY, 'r.', label=name)
    plt.xlabel('Delay(us)')
    plt.ylabel('Diferença de Dados')
    plt.legend()

    plt.show()


if __name__ == "__main__":    
    name = 'Com_Sensores'
    delay = loadFromFile('delay')
    acerto = loadFromFile('acerto')
    tempDec = loadFromFile('TempoDec')

    name = 'Sem_Sensores'
    acertoSS = loadFromFile('acerto')
    tempDecSS = loadFromFile('TempoDec')

    diff(delay, acerto, acertoSS, 'Acerto')
    diff(delay, tempDec, tempDecSS, 'Tempo de Aquisição Python')