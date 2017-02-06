-- PostgreSQL

-- run as owner!

-- create first retailer

INSERT INTO account (
	id, addr_name, city, country, postal_code,
	birth_date, created_on, email, locale, 
	first_name, last_name, password, status
) VALUES (
	0, 'nowhere', 'aether', 'beyond', '00000',
	'1970-01-01', '1970-01-01', 'root@tree.org',
	'en', 'SysAdmin', 'Almighty', 'root', 'ACTIVE'
);

INSERT INTO retailer (title, id) 
VALUES ('System Administrator', 0);
