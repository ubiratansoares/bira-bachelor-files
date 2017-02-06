CLASSPATH=./build:./lib/postgresql-8.3-603.jdbc4.jar

all:
	javac -d build/ -classpath $(CLASSPATH) src/br/usp/icmc/app01/Config.java
	javac -d build/ -classpath $(CLASSPATH) src/br/usp/icmc/app01/Projeto.java
	javac -d build/ -classpath $(CLASSPATH) src/br/usp/icmc/app01/Aplicacao.java

run:
	java -classpath $(CLASSPATH) br.usp.icmc.app01.Aplicacao

