-- PostgreSQL

-- run as superuser!

-- create users and database

CREATE USER appadmin WITH 
	CONNECTION LIMIT 5 
	ENCRYPTED PASSWORD '4994dm1n';

CREATE USER appuser WITH 
	CONNECTION LIMIT 10 
	ENCRYPTED PASSWORD '499u53r';

CREATE DATABASE sandbox WITH
	OWNER appadmin
	CONNECTION LIMIT 10;

GRANT CONNECT ON DATABASE sandbox TO appuser;

-- drop database and users

-- (to be done)
