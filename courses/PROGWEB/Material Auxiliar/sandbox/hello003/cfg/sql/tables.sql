-- PostgreSQL

-- run as owner!

-- creation

CREATE TABLE user_auth (
	username VARCHAR(20) PRIMARY KEY,
	password TEXT NOT NULL,
	email VARCHAR(50) NOT NULL
);

GRANT ALL ON user_auth TO appuser;

-- dropping

-- (to be done)
