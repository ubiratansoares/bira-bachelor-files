CLASSPATH=./build:./lib/postgresql-8.3-603.jdbc4.jar:./lib/mysql-connector-java-5.0.8-bin.jar

all:
	mkdir build
	javac -d build/ -classpath $(CLASSPATH) src/com/galpao/bd/Config.java
	javac -d build/ -classpath $(CLASSPATH) src/com/galpao/bd/DBConnection.java
	javac -d build/ -classpath $(CLASSPATH) src/com/galpao/app/Pessoa.java
	javac -d build/ -classpath $(CLASSPATH) src/com/galpao/app/Sistema.java
	cp -ax src/com/galpao/i18n build/com/galpao/

run:
	java -classpath $(CLASSPATH) com.galpao.app.Sistema

clean:
	rm -rf build
	find -name "*~" | xargs rm -rf
