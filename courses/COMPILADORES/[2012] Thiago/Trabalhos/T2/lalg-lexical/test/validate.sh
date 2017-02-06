# Script para automatizar os testes do Analisador Lexico

# Autores : Ubiratan Soares, Ulisses Soares, Rafael Pillon
# Data : 06/04/2012


#!/bin/sh

dir="./results"

if [[ ! -d $dir ]]; then mkdir $dir
fi	

echo "---------------------------------------------------------------------------"
echo "                ALL RESULT FILES WILL BE AT ./results FOLDER"
echo "---------------------------------------------------------------------------"

for i in $(find . -name "*.pas" -type f);
do
 	echo "--> Analysing file $i "

	gpc "samples/`basename $i`" 2> ./results/`basename $i .pas`_validated.txt
	echo "--> Generated `basename $i .pas`_validated.txt"
    echo " "			 
done