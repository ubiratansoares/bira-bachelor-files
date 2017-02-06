all:
	javac -d WEB-INF/classes/ src/org/stockmarket/db/*.java
	javac -d WEB-INF/classes/ -classpath WEB-INF/classes/ src/org/stockmarket/system/*.java

run:
	java -classpath WEB-INF/classes:WEB-INF/libs/postgresql-8.0-311.jdbc2.jar org.stockmarket.system.Stock
