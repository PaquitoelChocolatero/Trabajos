#!/bin/bash
#Comprobamos que esté bien definido el intervalo
if [[ "$1" ]] && [[ "$2" ]]; then
  #Recorremos todos los enteros dentro del intervalo
  for i in $(seq $1 $2); do
    '''
    Hacemos una variable que sea el primer factor primo del número, de tal forma que si el
    número es primo será ese y si no es primo no lo será, esto facilita la comparación
    '''
    factor="$(factor $i | cut -d " " -f 2)"
    #Para el resto de casos comprobamos que la salida de factor sea la misma que el número
    if [[ $i -eq $factor ]]; then
      echo "$i"
    fi
  done
else
  #Mensaje de error de uso
  echo "Usage: ./exercise3.sh A B"
fi
