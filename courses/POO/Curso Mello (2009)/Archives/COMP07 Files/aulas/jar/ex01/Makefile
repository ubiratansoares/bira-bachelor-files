CLASSPATH=./build

all:
	javac -classpath $(CLASSPATH) -d ./build src/com/empresa/framework/Calculo.java
	javac -classpath $(CLASSPATH) -d ./build src/com/empresa/framework/core/Teste.java
	javac -classpath $(CLASSPATH) -d ./build src/com/empresa/framework/core/Principal.java

jar:
	jar cvf minhaapp.jar -C ./build/ .

clean:
	rm -rf ./build/*
	rm -rf minhaapp.jar
