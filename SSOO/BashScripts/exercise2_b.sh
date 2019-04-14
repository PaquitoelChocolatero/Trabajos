#!/bin/bash
#Comprobamos si ha introducido la ruta a un archivo
if [[ "$1" ]] && [[ "$2" ]]; then
  #Adicionalmente comprobamos si el archivo introducido existe
  if [[ -e $1 ]]; then
    #Código proporcionado
    tr -c [:alnum:] [\\n\*] < $1 | sort | uniq -c | sort -nr | head -$2  
  else
    #Mensaje de error de archivo inexistente
    echo "File $1 doesn't exist"
  fi
else
  #Mensaje de error de uso
  echo "Usage: ./exercise2_b.sh FILE NUM"
fi
