# Laboratorios de redes
En este repo se encuentran enunciados, planteamiento, apuntes durante la realización y scripts para automatizarlos a la hora de rehacerlos.

## Index
### apuntes.txt
Aquí se encuentran los apuntes en orden de los comandos que se han ido ejecutando, las IPs escogidas, dónde se ha puesto cada cable, etc.
### Hito 1
![alt text](https://github.com/PaquitoelChocolatero/LaboRedes/blob/master/images/hito1.png)<br />
```
Uso: ./hito1.sh <nombre-de-la-primera-red> <numero-de-router> <nombre-de-la-segunda-red> | telnet 192.168.4.1
```
* La función del script es automatizar el cambio de IPs y tabla de rutas en cada ruter, por lo que se debe ejecutar en cada uno. <br />

* Todos los cambios que deban tener lugar en el pc físico deben hacerse manualmente. <br />
### Hito 2
![alt text](https://github.com/PaquitoelChocolatero/LaboRedes/blob/master/images/hito2.png)<br />
```
Uso: ./hito2-r1.sh | telnet 192.168.2.1
Uso: ./hito2-r2.sh | telnet 192.168.4.1
Uso: ./hito2-r3.sh | telnet 192.168.3.1
```
* La función del script es automatizar el cambio de IPs y tabla de rutas en cada ruter, por lo que se debe ejecutar en cada uno. <br />

* Enchufar cada cable donde se indica en apuntes.txt, es esencial para el buen funcionamiento del script.<br />

* El nombre de la red actual es la \*\*.91.\*\*, de querer cambiarlo hay que hacerlo a mano en los archivos.<br />

* Todos los cambios que deban tener lugar en el pc físico deben hacerse manualmente. <br />
### Hito 3
* Se hace exactamente igual que en el 2, los cambios son en el script
