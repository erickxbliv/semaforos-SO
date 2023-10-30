import threading
import time
import random

numero_de_caixas = 2
clientes = 10
semaforo = threading.Semaphore(numero_de_caixas)
lock = threading.Lock()

teste = [False,False]
numeros = [1,84,67,23,89,64,10,28,90,15]
execucoes = [0,0]
atendimento = [10]

def recurso():

    semaforo.acquire()

    lock.acquire()

    guardar = 0;

    for i in range(2):
      if teste[i] == False:
        teste[i] = True
        guardar = i
        break

    print(numeros[guardar] + " atendeu: ", threading.current_thread().name)
    execucoes[i] = threading.current_thread().ident

    remove(numeros[0])
    numeros.append(rand % 100)

    lock.release()

    #print(threading.current_thread().name)
    time.sleep(random.randint(3,10))

    for i in range(2):
      if execucoes[i] == threading.current_thread().ident:
        teste[i] = False
        break
 
    semaforo.release()

if __name__=="__main__":

    linhas = []

    for i in range(10):
      thread = threading.Thread(target=recurso)
      linhas.append(thread)


    for thread in linhas:
      thread.start()

    for thread in linhas:
      thread.join()
