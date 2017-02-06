-- PostgreSQL

-- run as superuser!

-- create users and database

CREATE USER appuser WITH 
	CONNECTION LIMIT 10 
	ENCRYPTED PASSWORD 'senha';

CREATE DATABASE ecommerce WITH
	OWNER appuser
	CONNECTION LIMIT 10;

-- drop database and users

-- (to be done)
