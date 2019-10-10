#!/bin/bash
while [[ "$1" ]]; do
	#Comprobamos que los usuarios introducidos no contengan : para usarlo en la expresión regular
	[[ "$1" =~ : ]] && echo "Users cannot contain ':'" && continue
	#Variable que recoge el interprete de cada usuario utilizando el regexp: empieza por el nombre del usuario y seguido de :
	shell="$(grep  "^$1:" "/etc/passwd"|cut -d:  -f 7)"
	#Si existe la variable, es decir, existe el usuario imprime con el formato deseado
	if [[ "$shell" ]]; then
		echo "$1: $shell"
	#Si no existe imprimimos un mensaje de error
	else
		echo "-ERROR- $1: no such user"
	fi
	#Desplazamos los usuarios a la izquierda
	shift
done

