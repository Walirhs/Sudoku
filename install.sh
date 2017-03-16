#!/bin/bash
## INSTALLATION DES BIBLIOTHeQUES GRAPHIQUES ##

echo -n "L'installation de la SDL est nécessaire pour faire fonctionner le projet "
echo "Voulez vous l'installer [O/n]"
read reps
if [ $reps = "O" ]  || [ $reps = "o" ] # || [ $reps = "enter" ]
then
    echo -n "installation"    
    sudo apt-get install libsdl1.2-dev
    sudo apt-get install libsdl-ttf2.0-dev
    sudo apt-get install libsdl-image1.2 libsdl-image1.2-dev
    echo -n "fin de l'insatllation"
else
    echo -n "pas d'installation. Exécution du code en cours"
fi
gcc sudoku.c -o sudoku -lSDL -lSDL_image -lSDL_ttf
./sudoku
exit 0
