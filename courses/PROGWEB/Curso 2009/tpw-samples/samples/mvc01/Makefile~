CLASSPATH=/usr/local/apache-tomcat-6.0.16/lib/servlet-api.jar:/usr/local/apache-tomcat-6.0.16/lib/jsp-api.jar:.:./src

all:
	rm -rf WEB-INF/classes
	mkdir -p WEB-INF/classes
	javac -classpath $(CLASSPATH) -d WEB-INF/classes src/sistema/*.java
	javac -classpath $(CLASSPATH) -d WEB-INF/classes src/servlets/*.java

clean:
	find -name "*~" | xargs rm -rf
