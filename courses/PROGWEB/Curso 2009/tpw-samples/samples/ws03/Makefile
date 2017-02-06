CLASSPATH=./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar:/usr/local/axis-1_4/lib/axis-ant.jar:/usr/local/axis-1_4/lib/axis.jar:/usr/local/axis-1_4/lib/commons-discovery-0.2.jar:/usr/local/axis-1_4/lib/commons-logging-1.0.4.jar:/usr/local/axis-1_4/lib/jaxrpc.jar:/usr/local/axis-1_4/lib/log4j-1.2.8.jar:/usr/local/axis-1_4/lib/log4j.properties:/usr/local/axis-1_4/lib/saaj.jar:/usr/local/axis-1_4/lib/wsdl4j-1.5.1.jar:/usr/local/apache-tomcat-6.0.16/lib/activation.jar:/usr/local/apache-tomcat-6.0.16/lib/mailapi.jar:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/src:./libs/postgresql-8.0-311.jdbc2.jar

all:
	find -name "*~" | xargs rm -rf
	rm -rf classes
	mkdir classes
	javac -classpath $(CLASSPATH) -d classes src/robot/*.java

register:
	java -classpath $(CLASSPATH) org.apache.axis.client.AdminClient -llocal:///AdminService deploy.wsdd

run:
	java -classpath $(CLASSPATH) robot.Cliente -llocal:///
