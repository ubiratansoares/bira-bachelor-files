-- some initial data to make the system useable

INSERT INTO privilege(description) 
VALUES('admin');

INSERT INTO privilege(description) 
VALUES('user');

INSERT INTO account(email, password) 
VALUES('root@default.org', md5('r007'));

INSERT INTO account_info(id, first_name)
VALUES((
	SELECT id FROM account 
	WHERE email = 'root@default.org'
), 'Default Admin');

INSERT INTO account_privilege(acc_id, priv_id)
VALUES((
	SELECT id FROM account 
	WHERE email = 'root@default.org'
), (
	SELECT id FROM privilege
	WHERE description = 'admin'
));
