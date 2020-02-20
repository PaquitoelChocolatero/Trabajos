import sys
import copy
from time import time

## AUTOBUS ######################################################################################################################################################################################################

class Bus():
    def __init__(self, parada, nMax, alumnos):
        self.parada=parada
        self.alumnosMax=nMax
        self.alumnosBus=[]
        self.alumnosRestantes=alumnos
        self.f=0
        self.g=0
        self.padre=None

    #---Heurísticas---#
    def h(self):
        if int(sys.argv[2])==0:
            return 0
        elif int(sys.argv[2])==1:
            return len(self.alumnosBus)+2*len(self.alumnosRestantes)
        elif int(sys.argv[2])==2:
            return len(self.alumnosRestantes)*self.alumnosMax
        else:
            return -1

    #---Operadores---#
    def mover(self, destino, ind):
        self.g+=int(self.parada.adyacentes[ind-1])
        self.parada=destino
        self.f=self.g+self.h()

    def coger(self, alumno):
        self.alumnosBus.append(alumno[0])
        self.alumnosRestantes.pop(self.alumnosRestantes.index(alumno))
        self.g+=1
        self.f=self.g+self.h()

    def dejar(self, alumno):
        self.alumnosBus.pop(self.alumnosBus.index(alumno))
        self.g+=1
        self.f=self.g+self.h()

## PARADA ######################################################################################################################################################################################################

class Parada():
    def __init__(self, nombre, adyacentes, colegio):
        self.nombre=nombre
        self.adyacentes=adyacentes
        self.colegio=colegio

## BUSQUEDA ######################################################################################################################################################################################################

class Simulador():

    #---Control de uso---#
    if len(sys.argv) != 3 or sys.argv[1][-5:] != ".prob" or not sys.argv[2].isdigit() or int(sys.argv[2]) not in [0, 1, 2]:
        print("Correct use: ./BusRouting.sh *.prob n")
        print("Where n:\nn=0 -> No heuristic, breadth first\nn=1 -> Number of misplaced kids\nn=2 -> Remaining kids * Seats in the bus")

    #---Ejecucion---#
    else:

        flag=0
        flag2=0

        adjacents={}
        schools={}
        currentStop=0

        tiempo_inicial = time()

        #---Parseo del archivo---#
        fprob=open(sys.argv[1], "r")
        if fprob.mode == 'r':
            problema = fprob.readlines()[1:] #Eliminamos la primera fila
            for line in problema:
                myLine=line.split()

                #Parsea las paradas y sus adyacencias
                if myLine[0][0]=='P' and flag==0:
                    nombre=myLine[0]
                    adyacentes=[]
                    for i in range(len(myLine)):
                        if i!=0:
                            if myLine[i]=='--':
                                adyacentes.append(-1)
                            else:
                                adyacentes.append(int(myLine[i]))
                        adjacents[nombre]=adyacentes

                #Parsea los colegios y sus paradas
                elif myLine[0][0]=='C' and flag2==0:
                    flag=1
                    for item in range(len(myLine)):
                        myLine[item]=myLine[item].replace(':', '')
                        myLine[item]=myLine[item].replace(';', '')
                    for i in range(1, len(myLine), 2):
                        schools[myLine[i]]=myLine[i-1]

                #Parsea los niños en cada parada
                elif myLine[0][0]=='P':
                    flag2=1
                    for item in range(len(myLine)):
                        myLine[item]=myLine[item].replace(':', '')
                        myLine[item]=myLine[item].replace(';', '')
                        myLine[item]=myLine[item].replace(',', '')

                    kidsList=[] #Lista con todos los alumnos

                    for item in range(len(myLine)):
                        if myLine[item][0]=='P':
                            currentStop=myLine[item]
                        elif myLine[item][0]=='C':
                            Clist=[] #Lista con todos los alumnos de un mismo colegio en una parada
                            #Añadimos colegios a una lista temporal hasta que no queden (por si hubiera más de uno)
                            while int(myLine[item-1])>0:
                                Clist.append(myLine[item])
                                myLine[item-1]=str(int(myLine[item-1])-1)
                            for i in Clist:
                                kidsList.append((i, currentStop))
                elif myLine[0][0]=='B':
                    currentStop=myLine[1]
                    nMax=myLine[2]

        keys=adjacents.keys()
        for stop in keys:
            globals()[stop] = Parada(stop, adjacents[stop], schools[stop] if stop in schools.keys() else None)

        #Inicializamos el bus en su estado inicial
        bus = Bus(eval(currentStop), int(nMax), kidsList)

        #Inicializamos el nodo temporal
        node=None

        #Inicializamos las listas
        openList=[]
        closedList=[]

        #Metemos el nodo inicial
        closedList.append(bus)

        not_meta=True
        paradas_visitadas=0
        nodos_expandidos=0
        prueba = 0

        #---Bucle de simulación---#
        while not_meta:
            nodos_expandidos+=1

            #Generamos un nodo movimiento por cada parada adyacente a la actual
            it=1
            for stop in bus.parada.adyacentes:
                if stop != -1:
                    node=copy.deepcopy(bus)
                    node.padre=bus
                    node.mover(eval('P'+str(it)), it)
                    included=False
                    for x in closedList:    #Comprobamos que ese nodo no haya sido ya expandido
                        if x.parada.nombre == node.parada.nombre and x.alumnosBus == node.alumnosBus and x.alumnosRestantes == node.alumnosRestantes:
                            included=True
                            break
                    if not included:
                        included=False
                        for x in openList:
                            if x.parada.nombre == node.parada.nombre and x.alumnosBus == node.alumnosBus and x.alumnosRestantes == node.alumnosRestantes and x.f > node.f:    #Sólo lo añadimos si mejora un nodo igual ya existente
                                included=True
                                openList.remove(x)
                                openList.append(node)
                                break
                        if not included:
                            openList.append(node)
                it+=1

            #Generamos un nodo coger niño, es irrelevante tener más, ya que no cambia nada y posiblemente las concatene
            if len(node.alumnosBus) < node.alumnosMax:
                for niño in bus.alumnosRestantes:
                    if niño[1] == bus.parada.nombre:
                        node=copy.deepcopy(bus)
                        node.padre=bus
                        node.coger(niño)
                        included=False
                        for x in closedList:
                            if x.parada.nombre == node.parada.nombre and x.alumnosBus == node.alumnosBus and x.alumnosRestantes == node.alumnosRestantes:
                                included=True
                                break
                        if not included:
                            included=False
                            for x in openList:
                                if x.parada.nombre == node.parada.nombre and x.alumnosBus == node.alumnosBus and x.alumnosRestantes == node.alumnosRestantes and x.f > node.f:
                                    included=True
                                    openList.remove(x)
                                    openList.append(node)
                                    break
                            if not included:
                                openList.append(node)
                        break

            #Generamos un nodo dejar niño
            if bus.parada.colegio != None:
                for niño in bus.alumnosBus:
                    if niño == bus.parada.colegio:
                        node=copy.deepcopy(bus)
                        node.padre=bus
                        node.dejar(niño)
                        included=False
                        for x in closedList:
                            if x.parada.nombre == node.parada.nombre and x.alumnosBus == node.alumnosBus and x.alumnosRestantes == node.alumnosRestantes:
                                included=True
                                break
                        if not included:
                            included=False
                            for x in openList:
                                if x.parada.nombre == node.parada.nombre and x.alumnosBus == node.alumnosBus and x.alumnosRestantes == node.alumnosRestantes and x.f > node.f:
                                    included=True
                                    openList.remove(x)
                                    openList.append(node)
                                    break
                            if not included:
                                openList.append(node)
                        break

            openList.sort(key=lambda state: state.f) #Ordenamos la lista abierta por el valor de su funcion

            bus=openList[0] #Sustituimos el estado

            openList=openList[1:] #Sacamos el nodo elegido de la lista abierta

            closedList.append(bus) #Lo metemos en la lista cerrada

            #Comprobamos si es el estado meta
            if len(bus.alumnosBus)+len(bus.alumnosRestantes)==0 and bus.parada.nombre==currentStop:
                not_meta=False

        #---Reconstruimos la solucion---#
        solucion=[]
        while bus.padre != None:
            solucion.insert(0, bus)
            if bus.parada.nombre != bus.padre.parada.nombre:
                paradas_visitadas+=1
            bus=bus.padre
        solucion.insert(0, bus)

        #Paramos el reloj
        tiempo_final = time()
        tiempo_total = tiempo_final - tiempo_inicial

        #---Escritura del resultado---#
        file = open(sys.argv[1] + ".output" , "w")
        file.write("Mapa inicial:" + "\n")
        fprob=open(sys.argv[1], "r")
        if fprob.mode == 'r':
            problema = fprob.readlines()
            for line in problema:
                file.write(line)
        fprob.close()

        file.write("\n")

        file.write("Solución:" + '\n' )
        for node in range(len(solucion)):
            if node==0:
                file.write("El autobús empieza en " + solucion[node].parada.nombre + "\n")
            else:
                if solucion[node].parada.nombre != solucion[node-1].parada.nombre:
                    file.write("El autobús se mueve a " + solucion[node].parada.nombre + "\n")
                elif len(solucion[node].alumnosBus) > len(solucion[node-1].alumnosBus):
                    file.write("En la parada " + solucion[node].parada.nombre + " sube un alumno del colegio " + solucion[node].alumnosBus[-1] + "\n")
                elif len(solucion[node].alumnosBus) < len(solucion[node-1].alumnosBus):
                    file.write("El autobús deja a un niño en el colegio " + solucion[node].parada.colegio + " en la parada " + solucion[node].parada.nombre + "\n")
            #file.write("Parada: " + node.parada.nombre + " | Alumnos subidos: " + str(len(node.alumnosBus)) + " | Alumnos Restantes: " + str(len(node.alumnosRestantes)) + " | Función: " + str(node.f) + '\n')
        file.close()

        #---Escritura de las estadisticas de la ejecucion---#
        file = open(sys.argv[1] + ".statistics" , "w")
        file.write("Tiempo total: " + str(tiempo_total) + '\n')
        file.write("Coste total: " + str(solucion[node].f) + '\n')
        file.write("Paradas Visitadas: " + str(paradas_visitadas) + '\n')
        file.write("Nodos Expandidos: " + str(nodos_expandidos))
        file.close()
