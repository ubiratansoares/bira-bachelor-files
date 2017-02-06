all:
	javac -d ./build src/teste/Teste.java

run:
	java -cp ./build:./lib/hsqldb.jar teste.Teste

hsqldb:
	java -cp ./lib/hsqldb.jar org.hsqldb.Server -database.0 file:mydb -dbname.0 xdb
