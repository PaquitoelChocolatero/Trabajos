#!/bin/bash
#Multiple choice
WHAT=$(dialog --backtitle $0 --output-fd 1 --checklist 'Select items to uncompress:\n\nSPACE: check/uncheck     TAB: changes focus     INTRO: accept\nMouse can also be used, if supported by your terminal emulator.' 16 76 26 README 'Read this before doing anything else' off ChangeLog 'History of changes' off usr/include/ 'C/C++ headers' off usr/share/backgrounds/ 'Backgrounds for X desktop environments' off usr/share/pixmaps/ 'X11 pixmaps' off usr/share/man/ 'Manual pages' off)
SIZE=$(stat --printf=%s $0)
#If at least one choice has been answered
if [ -n "$WHAT" ]; then
  #Input redirects to /tmp/input.$$ file
  dialog --inputbox "Enter a directory:" 8 60 2>/tmp/input.$$
  #The variable takes its value from reading the answer
  if [ "$(cat /tmp/input.$$)" == "." ]; then
    FILE="$(pwd)/Exercise5Files/"
  else
    FILE="$(pwd)/$(cat /tmp/input.$$)/Exercise5Files/"
  fi
  #Uncompressing the selected files
  mkdir -p $FILE
  cd
  cd $FILE
  (tail -n +25 $0 | pv -n -s $SIZE -i 0.25 | base64 -d | tar -xzvf - $WHAT) 2>&1 | dialog --gauge 'Uncompressing...' 8 46 
fi
#Deleting the temporary file created for the variable
rm -f /tmp/input.$$
exit 0;
#Buffer for the base64 command
H4sIAAAAAAAAA+3UwU6DQBAGYK7yFONJjbFhKVD11mg1Rr009e62jLBmZQkLqfTpXWptNMb0VJsm
/3dZAsMMC8yMR8Prx5G3VYGTRFG3ikEcfF+XRBx5oh/1RRwmQeLOi1BEiUfBdh/rU2NrWRF5r1Kb
khfZX3Gbru+pSa4sjZc/Ac2lJVXMdJNy6g6ozplMpTJVSE3P/M7VTFmOezY/ohel2QXImlrTUGrm
hTbS3dcjusplkfGDyZYJpbamy1Tx4a73Cr+tP9YWa2zq/1DE6/4fCOH6vx9EAv3/H254SmEgLi7p
jBZcqLpddm3FpZYzNwamLaXKbT2j48nT3Yl/QC5wNSTIvkmtqZRt1/vdxPg5JHx/WFZd9vMue2P5
q0JtqKzYclHT7eT+dFXB+rt+GQAAAAAAAAAAAAAAAAAAAAB76AOdzqHwACgAAA==
