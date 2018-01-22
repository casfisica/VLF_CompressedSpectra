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
    
    echo "*********************************************** "
    echo "File: $line $2"
    # The place in Gfif is /home/rsiii/SSSFDM/VLF/Signal/signal
    path="\/home\/rsiii\/SSSFDM\/VLF\/Signal\/signal\/$line"
    #echo $path
    sed 's/chain->Add();/'"chain->Add(\"$path\");"'/' ./CutsByCarlos.C > ./CutsByCarlosRun.C
    #echo "root -l -x -q 'CutsByCarlosRun.C'"

    
done < "$1" #Entrega el archivo de texto como entrada.
