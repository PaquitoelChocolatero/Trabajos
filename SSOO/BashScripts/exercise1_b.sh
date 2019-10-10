#!/bin/bash
#Hacemos la misma comprobacion que en el apartado anterior
[[ "$1" =~ : ]] && echo "GIDs cannot contain ':'" && continue
#Generamos el output, en este caso buscar el gid introducido en /etc/group y separar los usuarios del grupo por líneas nuevas en vez de comas
output="$(grep ":$1:" "/etc/group"|cut -d: -f 4|tr , "\n")"
if [[ "$output" ]]; then
  #Si la variable existe hay usuarios, los printeamos
  echo "$output"
else
  #Si no existe o bien no existe el grupo o no tiene usuarios, printeamos un mensaje de error
  echo "Group with GID $1: doesn't exist or contains no users"
fi
