CLASSPATH=/usr/local/apache-tomcat-6.0.16/lib/servlet-api.jar:/usr/local/apache-tomcat-6.0.16/lib/jsp-api.jar:.:./src

all:
	rm -rf WEB-INF/classes
	mkdir WEB-INF/classes
	javac -classpath $(CLASSPATH) -d WEB-INF/classes src/db/*.java

clean:
	find -name "*~" | xargs rm -rf
