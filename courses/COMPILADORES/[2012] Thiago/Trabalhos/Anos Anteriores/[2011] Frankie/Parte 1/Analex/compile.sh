# Script para compilar o arquivos de especifacao do Analisador Lexico
# Assume-se que o diretor /javacc-x.y/bin esta na variavel PATH
 
# Autores : Ubiratan Soares, Humberto Yagi, Ulisses Soares
# Data : 30/03/2011


#!/bin/bash

javacc AnaLex.jj
javac AnaLex.java
jar cfm AnaLex.jar MANIFEST.MF *.class *.java AnaLex.jj

