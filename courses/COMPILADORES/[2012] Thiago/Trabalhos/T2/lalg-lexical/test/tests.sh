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

for i in $(find . -name "*.pas");
do
 	echo "--> Analysing file $i "

	java -jar ../bin/Compiler.jar  < "samples/`basename $i`" > ./results/`basename $i .pas`_analysed.txt
	
	echo "--> Generated `basename $i .pas`_analysed.txt"
    echo " "			 
done
