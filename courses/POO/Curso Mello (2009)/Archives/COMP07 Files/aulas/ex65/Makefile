all:
	javac src/org/aplicacao/teste/Calculo.java src/org/aplicacao/hello/Hello.java -d build/

run:
	java -classpath ./build org.aplicacao.hello.Hello teste

jar:
	jar cvf aplicacao.jar -C build/ .

jarm:
	jar cvfm aplicacao.jar manifesto -C build/ .

runm:
	java -jar aplicacao.jar teste
