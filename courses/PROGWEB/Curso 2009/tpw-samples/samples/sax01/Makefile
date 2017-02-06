CLASSPATH=./libs/crimson.jar

all:
	find -name "*~" | xargs rm -rf
	rm -rf classes
	mkdir classes
	javac -classpath $(CLASSPATH) -d classes src/xmls/*.java

run:
	java -classpath $(CLASSPATH):./classes xmls.TestSAX samples/file01.xml

run1:
	java -classpath $(CLASSPATH):./classes xmls.TestSAX01 samples/file02.xml
