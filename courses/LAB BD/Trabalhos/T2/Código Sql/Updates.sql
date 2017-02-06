--Atualizacao de 4 tuplas
UPDATE RecursoEletronico 
SET Nome = 'Aulas de audio' , 
	Descricao = 'Este recurso permite que o aluno ouça novamente a aula ministrada pelo docente'
WHERE CodTipo = 3321;

--Delecao de 3 tuplas
DELETE FROM VinculoUsp
WHERE Ativo = 'Nao';

--4a
ALTER TABLE Disciplina
ADD NroCreditos NUMBER DEFAULT 4
	CHECK (NroCreditos IN (1,2,3,4,6));

ou
ALTER TABLE Disciplina
ADD NroCreditos NUMBER DEFAULT 4;

ALTER TABLE Disciplina
ADD CONSTRAINT NroCreditos CHECK (NroCreditos IN (1,2,3,4,6));
	
--4b
ALTER TABLE Matricula_se
ADD CONSTRAINT Nota CHECK (Nota >= 0);

--4c
--Drop sem cascade
 ALTER TABLE TipoRecurso
	DROP COLUMN   CodTipo;

	--Drop com cascade	
ALTER TABLE TipoRecurso
	DROP COLUMN   CodTipo  CASCADE CONSTRAINT;


--4di
ALTER TABLE Disciplina
DISABLE CONSTRAINT NroCreditos;

INSERT INTO Disciplina VALUES ('SSC0573','Projeto Supervisionado ou de Graduação I',
'Para os alunos que optarem por um estágio em uma empresa, realização de projeto em firma 
conveniada ou aprovada pelo ICMC, sob supervisão do professor responsável pela disciplina e 
engajado em algum projeto na empresa, sob a responsabilidade de um supervisor na empresa. 
Ao final deve se executar uma monografia pormenorizada do projeto. Para aqueles que optarem 
por um projeto de graduação, elaboração de um projeto em Ciências de Computação ou 
Sistemas de Informação, sob a orientação de docente especialista. O Projeto deve compreender as 
fases de especificação e implementação e, quando for o caso, levantamento bibliográfico deve ser 
feito. Ao final deve ser escrita uma monografia pormenorizada do projeto.',12);

ALTER TABLE Disciplina
ENABLE CONSTRAINT NroCreditos;