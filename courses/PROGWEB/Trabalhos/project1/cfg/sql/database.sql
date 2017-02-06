-- PostgreSQL

-- run as superuser!

-- create users and database

CREATE USER appuser WITH 
	CONNECTION LIMIT 10 
	ENCRYPTED PASSWORD '499u53r';

CREATE DATABASE agenda WITH
	OWNER appuser
	CONNECTION LIMIT 10;

-- drop database and users

-- (to be done)
