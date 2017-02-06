-- PostgreSQL

-- run as owner!

-- creation

CREATE TABLE user_auth (
	id SERIAL PRIMARY KEY, 
	email VARCHAR(50) UNIQUE,
	password TEXT NOT NULL
);

GRANT ALL ON user_auth TO appuser;
GRANT ALL ON user_auth_id_seq TO appuser;

-- dropping

/*
DROP TABLE user_auth;
*/
