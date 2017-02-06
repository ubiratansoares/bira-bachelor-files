all:
	mkdir classes
	javac -classpath ./classes/ -d classes/ src/com/correios/utils/Rota.java
	javac -classpath ./classes/ -d classes/ src/com/correios/database/Rota.java
	javac -classpath ./classes/ -d classes/ src/com/correios/core/Correspondencia.java

clean:
	rm -rf classes

run:
	java -classpath ./classes com.correios.core.Correspondencia
