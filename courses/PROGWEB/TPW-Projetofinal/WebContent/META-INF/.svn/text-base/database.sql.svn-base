-- run as superuser!



-- Role: "app"

-- DROP ROLE app;

CREATE ROLE app LOGIN
  ENCRYPTED PASSWORD 'md5627420172c6c5b7a8348f5cb2cdf500c' -- 'senha'
  NOSUPERUSER NOINHERIT CREATEDB NOCREATEROLE CONNECTION LIMIT 5;



-- Database: invest

-- DROP DATABASE invest;

CREATE DATABASE invest
  WITH OWNER = app
       ENCODING = 'UTF8'
       LC_COLLATE = 'C'
       LC_CTYPE = 'Portuguese_Brazil.1252'
       CONNECTION LIMIT = -1;
