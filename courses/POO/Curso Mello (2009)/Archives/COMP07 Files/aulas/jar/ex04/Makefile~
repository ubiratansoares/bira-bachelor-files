CLASSPATH=./build

all:
	javac -classpath $(CLASSPATH) -d ./build src/br/usp/icmc/projeto01/*.java

jar:
	jar cvfm projeto.jar manifesto -C ./build/ .

run:
	java -jar ./projeto.jar
