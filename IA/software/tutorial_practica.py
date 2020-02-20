#!/usr/bin/env python
# -*- coding: utf-8 -*-


'''
    Busqueda no informada

    Para cualquier problema hay que implementar los siguientes metodos:
      actions: LISTA de acciones posibles en cierto estado: (accion1,accion2,...)
      result: LISTA (estado,accion)
      is_goal: TRUE si es un estado final
      cost: Coste de ir de un estado a otro mediante cierta accion
      heuristic: Valor de h(estado)

    Con WebViewer(), hay que conectarse al servidor para visualizar la ejecucion:
      http://localhost:8000/#

    Problema 1:
      Estado inicial: A
      Acciones: mover a un estado conectado
      Objetivo: H
      Heuristic: no
      Algorithm: breadth-first

    Problema 2:
      Estado inicial: A
      Acciones: mover a un estado conectado
      Objetivo: H
      Heuristic: no
      Algorithm: depth-first

'''
import os
import sys

sys.path.append(os.path.abspath("simpleai-0.8.1"))

# Clase básica
from simpleai.search import SearchProblem

# Los visores sirven para permitir la generación de estadisticas y visualización
from simpleai.search.viewers import BaseViewer,ConsoleViewer,WebViewer

# Estos son los algoritmos de búsqueda que se usan en el tutorial:
from simpleai.search import breadth_first,depth_first,astar,greedy

#
# DECLARACION DE LA CLASE MapProblem
#
# Se etiqueta con class y se pone la clase de la que deriva entre paréntesis
# Los métodos son funciones declaradas dentro
# No es necesario, pero puede crearse un constructor __init__ (consultar)
# Se llama a los métodos y miembros de la clase usando explicitamente el objeto self
#
class MapProblem(SearchProblem):
    # En esta sección inicializamos si queremos los atributos del objeto
    # Se accede a ellos con el prefijo self
    mapaProblema=None
    estado_final=None
    # initial_state: atributo heredado de SearchProblem. Se asigna en el constructor

    # --------------- Metodos Comunes a todo problema SearchProblem -----------------

    def actions(self, state):
        # ESTE METODO DEBE DEVOLVER UNA LISTA DE LAS ACCIONES APLICABLES EN state
        return []

    def result(self, state, action):
        # ESTE METODO DEBE DEVOLVER EL ESTADO RESULTADO DE APLICAR action EN state
        return None

    def is_goal(self, state):
        # ESTE METODO DEBE DEVOLVER UN BOOLEANO True CUANDO state ES UN ESTADO META
        return False

    def cost(self, state, action, state2):
        # ESTE METODO DEBE DEVOLVER UN VALOR NUMERICO Coste(estado,acción,estado_2)
        return 1

    def heuristic(self, state):
        # ESTE METODO DEBE DEVOLVER UN VALOR NUMERICO RESULTADO DE CALCULAR LA HEURÍSTICA PARA state: h(state)
        return 0


# --------------- Metodos FUERA DE LA CLASE MapProblem -----------------

def ejercicioMapa(problem,algorithm,use_viewer=None):

    result = algorithm(problem,graph_search=True,viewer=use_viewer)

    if result:
        print("Estado final:" + result.state)
        # La llamada devuelve el camino hasta dicho estado.
        # Aquí usamos format para presentar información en los tokens del string
        print("Camino: {0}".format(result.path()))
        print("Coste: {0}".format(getTotalCost(problem,result)))

    else:
        print("WARNING: resultado de algoritmo de búsqueda vacío!!!")


    # Ejemplo de creación de una lista de pares {name,value}
    # No es necesario poner if use_viewer is not None
    if use_viewer:
        stats = [{'name': stat.replace('_', ' '), 'value': value}
                         for stat, value in list(use_viewer.stats.items())]

        # Ejemplo de bucle sobre elementos de una lista
        for s in stats:
            print ('{0}: {1}'.format(s['name'],s['value']))

    return result

def getTotalCost (problem,result):
    originState = problem.initial_state
    totalCost = 0
    for action,endingState in result.path():
        if action is not None:
            totalCost += problem.cost(originState,action,endingState)
            originState = endingState
    return totalCost

# FIN de ejercicioMapa

# ------------  Aquí empieza el código que se ejecuta al cargar el script -----
# -------------------------  RESOLUCIÓN DE LOS PROBLEMAS ----------------------

# RESOLUCION DEL PROBLEMA 1
estado_inicial="A"
estado_final="H"
sucesoresA= {'accion-1':'B', 'accion-2':'C'}
sucesoresB= {'accion-1':'D', 'accion-2':'E'}
sucesoresC= {'accion-1':'E'}
sucesoresD= {'accion-1':'F', 'accion-2':'G'}
sucesoresE= {'accion-1':'G', 'accion-2':'H'}
sucesoresF= {}
sucesoresG= {'accion-1':'H'}
sucesoresH= {}
mapa = { "A" : sucesoresA, "B" : sucesoresB, "C" : sucesoresC, "D" : sucesoresD, "E" : sucesoresE, "F" : sucesoresF, "G" : sucesoresG, "H" : sucesoresH}

problem = MapProblem(estado_inicial)
problem.mapaProblema = mapa
problem.estado_final = estado_final

# NOTA: ejercicioMapa es una funcion de este modulo, no un metodo
# Aqui es donde podemos seleccionar WebViewer,ConsoleViewer o BaseViewer
#   BaseViewer() simplemente ejecuta y muestra las trazas y estadisticas
#   ConsoleViewer() permite ejecutar paso a paso por pantalla
#      NOTA: Para usar esto, hay que poner la opcion entre comillas
#   WebViewer() usa el interfaz web
#
# ejercicioMapa(problem,algorithm=breadth_first,use_viewer=WebViewer())
ejercicioMapa(problem,algorithm=breadth_first,use_viewer=BaseViewer())
#ejercicioMapa(problem,algorithm=breadth_first,use_viewer=ConsoleViewer())



