CLASSPATH=/usr/local/axis-1_4/lib/axis.jar:/usr/local/axis-1_4/lib/jaxrpc.jar:/usr/local/axis-1_4/lib/log4j-1.2.8.jar:/usr/local/axis-1_4/lib/commons-logging-1.0.4.jar:/usr/local/axis-1_4/lib/axis-ant.jar:/usr/local/axis-1_4/lib/saaj.jar:/usr/local/axis-1_4/lib/wsdl4j-1.5.1.jar:/usr/local/axis-1_4/lib/commons-discovery-0.2.jar:/usr/local/apache-tomcat-6.0.16/lib/activation.jar:/usr/local/apache-tomcat-6.0.16/lib/mailapi.jar:./libs/postgresql-8.0-311.jdbc2.jar

all:
	rm -rf classes
	mkdir classes
	javac -classpath $(CLASSPATH) -d classes src/servico/*.java
	cp src/servico/Servico02.java ../axis/Servico02.jws

run:
	java -classpath $(CLASSPATH):./classes servico.Cliente
