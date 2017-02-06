#!/bin/sh

echo -n "Gerando gráfico... "
NOW=`date +%s`
gnuplot grafico-xor.pl
echo "$((`date +%s` - $NOW)) segundos." 

[ -f saidas.png ] && xv saidas.png
