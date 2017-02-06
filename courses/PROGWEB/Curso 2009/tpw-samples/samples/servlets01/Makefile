CLASSPATH=/usr/local/apache-tomcat-6.0.16/lib/servlet-api.jar:/usr/local/apache-tomcat-6.0.16/lib/jsp-api.jar:.:./src:./WEB-INF/lib/standard.jar:./WEB-INF/lib/jstl.jar

all:
	rm -rf WEB-INF/classes
	mkdir WEB-INF/classes
	javac -classpath $(CLASSPATH) -d WEB-INF/classes src/*.java

clean:
	find -name "*~" | xargs rm -rf
