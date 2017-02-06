all:
	find -name "*~" | xargs rm -rf
	rm -rf classes
	mkdir classes
	javac -d classes src/db/*.java
	javac -d classes src/user/*.java
	javac -classpath /usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar -d classes src/org/usp/news/*.java
	javac -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar:/usr/local/axis-1_4/lib/axis-ant.jar:/usr/local/axis-1_4/lib/axis.jar:/usr/local/axis-1_4/lib/commons-discovery-0.2.jar:/usr/local/axis-1_4/lib/commons-logging-1.0.4.jar:/usr/local/axis-1_4/lib/jaxrpc.jar:/usr/local/axis-1_4/lib/log4j-1.2.8.jar:/usr/local/axis-1_4/lib/log4j.properties:/usr/local/axis-1_4/lib/saaj.jar:/usr/local/axis-1_4/lib/wsdl4j-1.5.1.jar:/usr/local/apache-tomcat-6.0.16/lib/activation.jar:/usr/local/apache-tomcat-6.0.16/lib/mailapi.jar -d classes src/org/usp/exemplo01ws/Cliente.java
	javac -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar:/usr/local/axis-1_4/lib/axis-ant.jar:/usr/local/axis-1_4/lib/axis.jar:/usr/local/axis-1_4/lib/commons-discovery-0.2.jar:/usr/local/axis-1_4/lib/commons-logging-1.0.4.jar:/usr/local/axis-1_4/lib/jaxrpc.jar:/usr/local/axis-1_4/lib/log4j-1.2.8.jar:/usr/local/axis-1_4/lib/log4j.properties:/usr/local/axis-1_4/lib/saaj.jar:/usr/local/axis-1_4/lib/wsdl4j-1.5.1.jar:/usr/local/apache-tomcat-6.0.16/lib/activation.jar:/usr/local/apache-tomcat-6.0.16/lib/mailapi.jar -d classes src/org/usp/math/Cliente.java
	javac -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar:/usr/local/axis-1_4/lib/axis-ant.jar:/usr/local/axis-1_4/lib/axis.jar:/usr/local/axis-1_4/lib/commons-discovery-0.2.jar:/usr/local/axis-1_4/lib/commons-logging-1.0.4.jar:/usr/local/axis-1_4/lib/jaxrpc.jar:/usr/local/axis-1_4/lib/log4j-1.2.8.jar:/usr/local/axis-1_4/lib/log4j.properties:/usr/local/axis-1_4/lib/saaj.jar:/usr/local/axis-1_4/lib/wsdl4j-1.5.1.jar:/usr/local/apache-tomcat-6.0.16/lib/activation.jar:/usr/local/apache-tomcat-6.0.16/lib/mailapi.jar -d classes src/org/usp/robotws/*.java
	javac -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar:/usr/local/axis-1_4/lib/axis-ant.jar:/usr/local/axis-1_4/lib/axis.jar:/usr/local/axis-1_4/lib/commons-discovery-0.2.jar:/usr/local/axis-1_4/lib/commons-logging-1.0.4.jar:/usr/local/axis-1_4/lib/jaxrpc.jar:/usr/local/axis-1_4/lib/log4j-1.2.8.jar:/usr/local/axis-1_4/lib/log4j.properties:/usr/local/axis-1_4/lib/saaj.jar:/usr/local/axis-1_4/lib/wsdl4j-1.5.1.jar:/usr/local/apache-tomcat-6.0.16/lib/activation.jar:/usr/local/apache-tomcat-6.0.16/lib/mailapi.jar -d classes src/org/usp/robotwsmapped/*.java
	javac -classpath /usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar -d classes src/xmls/*.java

run:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/postgresql-8.0-311.jdbc2.jar org.usp.news.RSS

runcliente:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar:/usr/local/axis-1_4/lib/axis-ant.jar:/usr/local/axis-1_4/lib/axis.jar:/usr/local/axis-1_4/lib/commons-discovery-0.2.jar:/usr/local/axis-1_4/lib/commons-logging-1.0.4.jar:/usr/local/axis-1_4/lib/jaxrpc.jar:/usr/local/axis-1_4/lib/log4j-1.2.8.jar:/usr/local/axis-1_4/lib/log4j.properties:/usr/local/axis-1_4/lib/saaj.jar:/usr/local/axis-1_4/lib/wsdl4j-1.5.1.jar:/usr/local/apache-tomcat-6.0.16/lib/activation.jar:/usr/local/apache-tomcat-6.0.16/lib/mailapi.jar:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/postgresql-8.0-311.jdbc2.jar org.usp.robotws.Cliente

runclientemapped:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar:/usr/local/axis-1_4/lib/axis-ant.jar:/usr/local/axis-1_4/lib/axis.jar:/usr/local/axis-1_4/lib/commons-discovery-0.2.jar:/usr/local/axis-1_4/lib/commons-logging-1.0.4.jar:/usr/local/axis-1_4/lib/jaxrpc.jar:/usr/local/axis-1_4/lib/log4j-1.2.8.jar:/usr/local/axis-1_4/lib/log4j.properties:/usr/local/axis-1_4/lib/saaj.jar:/usr/local/axis-1_4/lib/wsdl4j-1.5.1.jar:/usr/local/apache-tomcat-6.0.16/lib/activation.jar:/usr/local/apache-tomcat-6.0.16/lib/mailapi.jar:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/postgresql-8.0-311.jdbc2.jar org.usp.robotwsmapped.Cliente

runmath:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar:/usr/local/axis-1_4/lib/axis-ant.jar:/usr/local/axis-1_4/lib/axis.jar:/usr/local/axis-1_4/lib/commons-discovery-0.2.jar:/usr/local/axis-1_4/lib/commons-logging-1.0.4.jar:/usr/local/axis-1_4/lib/jaxrpc.jar:/usr/local/axis-1_4/lib/log4j-1.2.8.jar:/usr/local/axis-1_4/lib/log4j.properties:/usr/local/axis-1_4/lib/saaj.jar:/usr/local/axis-1_4/lib/wsdl4j-1.5.1.jar:/usr/local/apache-tomcat-6.0.16/lib/activation.jar:/usr/local/apache-tomcat-6.0.16/lib/mailapi.jar:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/postgresql-8.0-311.jdbc2.jar org.usp.math.Cliente

runexemplo01ws:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar:/usr/local/axis-1_4/lib/axis-ant.jar:/usr/local/axis-1_4/lib/axis.jar:/usr/local/axis-1_4/lib/commons-discovery-0.2.jar:/usr/local/axis-1_4/lib/commons-logging-1.0.4.jar:/usr/local/axis-1_4/lib/jaxrpc.jar:/usr/local/axis-1_4/lib/log4j-1.2.8.jar:/usr/local/axis-1_4/lib/log4j.properties:/usr/local/axis-1_4/lib/saaj.jar:/usr/local/axis-1_4/lib/wsdl4j-1.5.1.jar:/usr/local/apache-tomcat-6.0.16/lib/activation.jar:/usr/local/apache-tomcat-6.0.16/lib/mailapi.jar:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/postgresql-8.0-311.jdbc2.jar org.usp.exemplo01ws.Cliente

testdom01:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar xmls.TestDOMlibrary /usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/src/xmls/teste.xml

testdom02:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar xmls.TestDOMlibrary01 /usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/src/xmls/teste.xml

testdom03:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar xmls.TestDOMlibrary02 /usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/src/xmls/teste.xml

testdom04:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar xmls.TestDOMlibrary03 /usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/src/xmls/teste.xml

sax01:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar xmls.TestSAX /usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/samples/test01.xml

sax02:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar xmls.TestSAX /usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/samples/test02.xml

sax03:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar xmls.TestSAX /usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/samples/test03.xml

dom01:
	java -classpath ./classes:/usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/libs/crimson.jar xmls.TestDOM /usr/local/apache-tomcat-6.0.16/hipermidia/ROOT/base/samples/test03.xml




