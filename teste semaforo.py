from datetime import datetime
import threading
import random
import time

class Fila:

  def __init__(self, tamanho):
    self.max_tamanho = tamanho
    self.lista = []
    for i in range(0, self.max_tamanho):
      self.lista.append(random.randint(1, 100))

  def poe_elemento_no_fim_da_fila(self, novo):
    if (len(self.lista) < self.max_tamanho): 
      self.lista.append(novo)
    else:
      print("Fila cheia.")

  def get_fila(self):
    return self.lista

  def retira_primeiro_elemento_da_fila(self):
    if (len(self.lista) > 0):
      return self.lista.pop(0)
    else:
      print("Fila vazia.")
      return None

fila = Fila(10)   
semaforo = threading.Semaphore(1)

def thread_function(thread_number):
    
  while True:
    #print(f"{(datetime.now().strftime('%H:%M:%S'))} Thread {thread_number} solicitando acesso")
    semaforo.acquire()
    #print(f"{datetime.now().strftime('%H:%M:%S')} Thread {thread_number} conseguiu acesso")
    velho = fila.retira_primeiro_elemento_da_fila()
    novo = random.randint(0, 100)
    fila.poe_elemento_no_fim_da_fila(novo)
    print(f"{datetime.now().strftime('%H:%M:%S')} Thread {thread_number}: tira {velho}, bota {novo}, fila: {fila.get_fila()}")
    #print(f"{datetime.now().strftime('%H:%M:%S')} Thread {thread_number} liberando acesso.\n", '-'*50, "\n")
    time.sleep(5)
    semaforo.release()
    time.sleep(0.2)
    
t1 = threading.Thread(target=thread_function, args=(1,))
t2 = threading.Thread(target=thread_function, args=(2,))

t1.start()
t2.start()

t1.join()
t2.join()
