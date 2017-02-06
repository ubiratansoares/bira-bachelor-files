# Script para gerar os resultados do Analisador Lexico
# Autores : Ubiratan Soares, Humberto Yagi, Ulisses Soares
# Data : 06/06/2011


#!/bin/sh

dir="./results"

if [[ ! -d $dir ]]; then mkdir $dir
fi	

echo "---------------------------------------------------------------------------"
echo "                ALL RESULT FILES WILL BE AT ./results"
echo "---------------------------------------------------------------------------"

for i in $(find ./tests -name "*.txt");
do
 	echo "--> Compiling file $i "

	java -jar Compiler.jar  < "$i" > ./results/`basename $i .txt`.txt
	
	echo "--> Generated `basename $i .txt`.txt"
			 
done
