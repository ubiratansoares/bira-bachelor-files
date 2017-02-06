-- PostgreSQL

-- run as database owner!

-- creation

-- informações de autenticação de conta
CREATE TABLE account (
	id SERIAL PRIMARY KEY, --for referencing
	email VARCHAR(50) NOT NULL UNIQUE, --for locating
	password TEXT NOT NULL --for authentication
);

-- tarefa
CREATE TABLE task (
	id SERIAL PRIMARY KEY, 
	owner INTEGER NOT NULL REFERENCES account(id) ON DELETE CASCADE,
	short_desc VARCHAR(255) NOT NULL,
	long_desc TEXT, 
	busy BOOLEAN,
	location VARCHAR(255),
	created_on TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	start_time TIMESTAMP NOT NULL,
	end_time TIMESTAMP NOT NULL
);

-- tipo de tarefa
CREATE TABLE task_type (
	id SERIAL PRIMARY KEY,
	name VARCHAR(50) NOT NULL UNIQUE,
	description TEXT
);

-- tarefas possuem tipos de tarefas (m:n)
CREATE TABLE task_task_type (
	task_id INTEGER REFERENCES task(id) ON DELETE CASCADE,
	type_id INTEGER REFERENCES task_type(id) ON DELETE CASCADE,
	PRIMARY KEY(task_id, type_id)
);

-- privilégios disponíveis de contas
CREATE TABLE privilege (
	id SERIAL PRIMARY KEY,
	description TEXT UNIQUE 
);

-- contas possuem privilégios (m:n)
CREATE TABLE account_privilege (
	acc_id INTEGER REFERENCES account(id) ON DELETE CASCADE,
	priv_id INTEGER REFERENCES privilege(id) ON DELETE CASCADE,
	PRIMARY KEY(acc_id, priv_id)
);

-- informações adicionais de conta
CREATE TABLE account_info (
	id INTEGER PRIMARY KEY REFERENCES account(id) ON DELETE CASCADE,
	created_on TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	last_visit TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	first_name VARCHAR(30) NOT NULL,
	last_name VARCHAR(100)
);

-- dropping

/*
DROP TABLE account_privilege;
DROP TABLE account_info;
DROP TABLE privilege;
DROP TABLE task_task_type;
DROP TABLE task_type;
DROP TABLE task;
DROP TABLE account;
*/
