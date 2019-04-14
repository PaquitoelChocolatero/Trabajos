#!/bin/bash
#Comprobamos que se introduzca un directorio
if [[ "$1" ]]; then
  #Comprobamos si existe
  if [[ -e $1 ]]; then
    #Recorremos todos los JPG del directorio introducido
    for filename in $1*.jpg; do
      #Guardamos en una variable el tamaño del archivo
      filesize="$(wc -c $filename | cut -d " " -f 1)"
      #Comprobamos que la imagen sea más pesada que 1 MB
      if [[ $filesize -gt 1000000 ]]; then
        #Modificamos el archivo
        mogrify -resize 720x720 $filename
        #Imprimimos el archivo modificado
        echo "$filename"
      fi
    done
  else
    #Mensaje de error de archivo inexistente
    echo "Directory $1 doesn't exist"
  fi
else
  #Mensaje de error de uso
  echo "Usage: ./ejercicio4.sh FILE"
fi
