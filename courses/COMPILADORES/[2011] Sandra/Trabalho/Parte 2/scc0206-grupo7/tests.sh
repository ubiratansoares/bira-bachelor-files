# Script para gerar os resultados do Analisador Lexico
# Autores : Ubiratan Soares, Humberto Yagi, Ulisses Soares
# Data : 30/03/2011


#!/bin/sh

dir="./Results"

if [[ ! -d $dir ]]; then mkdir $dir
fi	

echo "---------------------------------------------------------------------------"
echo "                ALL RESULT FILES WILL BE AT ./Results"
echo "---------------------------------------------------------------------------"

for i in $(find . -name "*.frankie");
do
 	echo "--> Parsing file $i "

	java -jar bin/parser.jar  < "corretos/`basename $i`" > ./Results/`basename $i .frankie`.txt
	
	echo "--> Generated `basename $i .frankie`.txt"
			 
done
