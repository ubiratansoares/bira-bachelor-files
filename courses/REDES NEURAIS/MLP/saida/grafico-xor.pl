
#set encoding iso_8859_1
set terminal png
set output "saidas.png"

set xlabel "Tempo"
set ylabel "Saida"
set title "Saidas da MLP"

plot 'saida.txt' using 1:2 title '(0, 0)' with lines, \
     'saida.txt' using 1:3 title '(0, 1)' with lines, \
     'saida.txt' using 1:4 title '(1, 0)' with lines, \
     'saida.txt' using 1:5 title '(1, 1)' with lines, \
     'saida.txt' using 1:6 title 'Erro' with lines
