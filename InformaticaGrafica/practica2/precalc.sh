#!/bin/sh

panic() {
	echo 'Uso: [sh] ./precalc.sh [ciudad|terreno]'
}

case $1 in
	ciudad|terreno)
		cwd=$(pwd)
		cd $1
		if ! povray precalc.pov +GDheight.py; then
			exit 1
		fi
		rm precalc.png
		cd $cwd
		;;
	*)
		panic && exit 1
		;;
esac
	
python genfield.py $1/height.py fields/$1.png
