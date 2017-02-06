CLASSPATH=./libs/crimson.jar

all:
	find -name "*~" | xargs rm -rf
	rm -rf classes
	mkdir classes
	javac -classpath $(CLASSPATH) -d classes src/xmls/*.java

run:
	java -classpath $(CLASSPATH):./classes xmls.TestDOMlibrary samples/file01.xml

run1:
	java -classpath $(CLASSPATH):./classes xmls.TestDOMlibrary01 samples/file01.xml

run2:
	java -classpath $(CLASSPATH):./classes xmls.TestDOMlibrary02 samples/file01.xml

run3:
	java -classpath $(CLASSPATH):./classes xmls.TestDOMlibrary03 samples/file01.xml

run4:
	java -classpath $(CLASSPATH):./classes xmls.TestDOMlibrary04 samples/file01.xml
