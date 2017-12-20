#!/bin/bash


fecha=$(date +"%d-%m-%y_%T")

function Error {
    echo ""
    echo "usage:" $0  "ListOfFiles.cvs [phath_to_the_files]"
    echo ""
    exit 0
 }

if [ -z $1 ]; then #Para fijarse que el argumento no esté vacio
    echo "No 1st argument supplied"
    Error
fi


while IFS='' read -r line || [[ -n "$line" ]]; do

    echo "Text read from file: $line $2"
    
done < "$1" #Entrega el archivo de texto como entrada.
