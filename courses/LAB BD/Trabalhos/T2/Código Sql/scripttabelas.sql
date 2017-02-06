CREATE TABLE  VinculoUSP 
   (  NroUsp NUMBER NOT NULL, 
      Nome VARCHAR2(60) NOT NULL, 
      DataIngresso DATE, 
      Ativo CHAR(3),
	  TipoVinculo CHAR NOT NULL,
     
	  CONSTRAINT VinculoUSP_PK PRIMARY KEY (NroUsp),
	  
	  CONSTRAINT Check_Ativo CHECK (Ativo IN ('Sim','Nao')),
	  CONSTRAINT Check_TipoVinculo CHECK (TipoVinculo IN ('1','2','3'))   --1.Professor;2.Aluno;3.Funcionario
   );
   
CREATE TABLE  Disciplina 
   (  Cod VARCHAR2(7) NOT NULL, 
      Nome VARCHAR2(40) NOT NULL, 
      Ementa CLOB, 
		
      CONSTRAINT Disciplina_PK PRIMARY KEY (Cod) 
   );

CREATE TABLE  Leciona 
   (  NroUSP NUMBER NOT NULL , 
      Cod VARCHAR2(7) NOT NULL , 
	 
      CONSTRAINT Leciona_PK PRIMARY KEY (NroUSP, Cod) , 
	 
      CONSTRAINT Leciona_FK_VinculoUSP FOREIGN KEY (NroUSP)
        REFERENCES  VinculoUSP (NroUSP) ON DELETE CASCADE , 
      CONSTRAINT Leciona_FK_Disciplina FOREIGN KEY (Cod)
        REFERENCES  Disciplina (Cod) ON DELETE CASCADE 
   );

CREATE TABLE  Turma 
   (  Cod VARCHAR2(7) NOT NULL , 
      NroTurma VARCHAR2(7) NOT NULL , 
	 
      CONSTRAINT Turma_PK PRIMARY KEY (Cod, NroTurma) , 
		
      CONSTRAINT Turma_FK_Disciplina FOREIGN KEY (Cod)
        REFERENCES  Disciplina (Cod) 
   );


CREATE TABLE  Matricula_se 
   (  NroUSP NUMBER NOT NULL , 
      Cod VARCHAR2(7) NOT NULL , 
      NroTurma VARCHAR2(7) NOT NULL , 
      Nota BINARY_FLOAT , 
	 
      CONSTRAINT Matriculase_PK PRIMARY KEY (NroUSP, Cod, NroTurma) , 
	 
      CONSTRAINT Matriculase_FK_VinculoUSP FOREIGN KEY (NroUSP)
        REFERENCES  VinculoUSP (NroUsp) ON DELETE CASCADE ,
      CONSTRAINT Matriculase_FK_Turma FOREIGN KEY (Cod, NroTurma)
        REFERENCES  Turma (Cod, NroTurma) ON DELETE CASCADE 
   );
   
CREATE TABLE  CursoEletronico 
   (  CodCurso NUMBER NOT NULL , 
      Cod VARCHAR2(7) NOT NULL , 
      NroTurma VARCHAR2(7) NOT NULL , 
      AceitaExternos CHAR(3)  DEFAULT ('Nao'),
	 
      CONSTRAINT CursoEletronico_PK PRIMARY KEY (CodCurso, Cod,NroTurma) , 
	 
      CONSTRAINT CursoEletronico_FK_Turma FOREIGN KEY (Cod, NroTurma)
        REFERENCES  Turma (Cod, NroTurma) ON DELETE CASCADE,

	  CONSTRAINT Check_Aceita CHECK (AceitaExternos IN ('Sim','Nao'))
   );
   
CREATE TABLE Participante
   (  NroUSP NUMBER NOT NULL , 
      CodCurso NUMBER NOT NULL , 
      Cod VARCHAR2(7) NOT NULL,
      NroTurma VARCHAR2(7) NOT NULL , 
      Nota BINARY_FLOAT ,
      NotaFinal BINARY_FLOAT ,
	 
      CONSTRAINT Participante_PK PRIMARY KEY (NroUSP, CodCurso, Cod, NroTurma) , 
	 
      CONSTRAINT Participante_FK_Matricula FOREIGN KEY (NroUSP)
        REFERENCES  VinculoUsp(NroUsp) ON DELETE CASCADE ,
      CONSTRAINT Participante_FK_CursoE FOREIGN KEY (CodCurso, Cod, NroTurma)
        REFERENCES  CursoEletronico (CodCurso, Cod, NroTurma) ON DELETE CASCADE 
   );
   
CREATE TABLE Mensagem
   (  NroUSP NUMBER NOT NULL , 
      CodCurso NUMBER NOT NULL , 
      Cod VARCHAR2(7) NOT NULL,
      NroTurma VARCHAR2(7) NOT NULL , 
      NroMsg NUMBER NOT NULL ,
      Título VARCHAR2 (50) ,
      Texto VARCHAR2 (140) NOT NULL,
      DataHoraPostagem TIMESTAMP WITH LOCAL TIME ZONE,
	 
      CONSTRAINT Mensagem_PK PRIMARY KEY (NroUSP, CodCurso, Cod, NroTurma, NroMsg) , 
	 
      CONSTRAINT Mensagem_FK_Participante FOREIGN KEY (NroUSP, CodCurso, Cod, NroTurma)
        REFERENCES  Participante (NroUsp, CodCurso, Cod, NroTurma) ON DELETE CASCADE 
   );

CREATE TABLE Grupo
   (  CodCurso NUMBER NOT NULL , 
      Cod VARCHAR2(7) NOT NULL,
      NroTurma VARCHAR2(7) NOT NULL , 
      NroGrupo NUMBER NOT NULL ,
      NomeGrupo VARCHAR2 (20) ,
      
      CONSTRAINT Grupo_PK PRIMARY KEY (CodCurso, Cod, NroTurma, NroGrupo) , 
	 
      CONSTRAINT Grupo_FK_CE FOREIGN KEY (CodCurso, Cod, NroTurma)
        REFERENCES  CursoEletronico (CodCurso, Cod, NroTurma) ON DELETE CASCADE 
   );

CREATE TABLE Trabalho
   (  CodCurso NUMBER NOT NULL , 
      Cod VARCHAR2(7) NOT NULL,
      NroTurma VARCHAR2(7) NOT NULL , 
      NroTrabalho NUMBER NOT NULL ,
      Descricao VARCHAR2 (400) ,
      ArqEspec CLOB DEFAULT (EMPTY_CLOB()),
      DataHoraLimite VARCHAR2 (19),
      
      CONSTRAINT Trabalho_PK PRIMARY KEY (CodCurso, Cod, NroTurma, NroTrabalho) , 
	 
      CONSTRAINT Trabalho_FK_CE FOREIGN KEY (CodCurso, Cod, NroTurma)
        REFERENCES  CursoEletronico (CodCurso, Cod, NroTurma) ON DELETE CASCADE 
   );

CREATE TABLE Composicao
   (  CodCurso NUMBER NOT NULL , 
      Cod VARCHAR2(7) NOT NULL,
      NroTurma VARCHAR2(7) NOT NULL , 
      NroGrupo NUMBER NOT NULL ,
      NroUsp NUMBER NOT NULL,
      
      
      CONSTRAINT Composicao_PK PRIMARY KEY (CodCurso, Cod, NroTurma, NroGrupo, NroUsp) , 
	 
      CONSTRAINT Composicao_FK_Grupo FOREIGN KEY (CodCurso, Cod, NroTurma, NroGrupo)
        REFERENCES  Grupo  (CodCurso, Cod, NroTurma, NroGrupo) ON DELETE CASCADE,
      CONSTRAINT Composicao_FK_Participante FOREIGN KEY (NroUsp, CodCurso, Cod, NroTurma)
        REFERENCES  Participante  (NroUsp, CodCurso, Cod, NroTurma) ON DELETE CASCADE 
   );
   
CREATE TABLE Entrega
   (  CodCurso NUMBER NOT NULL , 
      Cod VARCHAR2(7) NOT NULL,
      NroTurma VARCHAR2(7) NOT NULL , 
      NroTrabalho NUMBER NOT NULL ,
      NroGrupo NUMBER NOT NULL ,
      Arquivo BLOB DEFAULT (EMPTY_BLOB()),
      Nota BINARY_FLOAT,
      DataHoraEntrega TIMESTAMP WITH LOCAL TIME ZONE,
      
      CONSTRAINT Entrega_PK PRIMARY KEY (CodCurso, Cod, NroTurma, NroTrabalho, NroGrupo) , 
	 
      CONSTRAINT Entrega_FK_Trabalho FOREIGN KEY (CodCurso, Cod, NroTurma, NroTrabalho)
        REFERENCES Trabalho (CodCurso, Cod, NroTurma,NroTrabalho) ON DELETE CASCADE,
      CONSTRAINT Entrega_FK_Grupo FOREIGN KEY (CodCurso, Cod, NroTurma, NroGrupo)
        REFERENCES Grupo (CodCurso, Cod, NroTurma,NroGrupo) ON DELETE CASCADE
   );
   
CREATE TABLE TipoRecurso
   (  CodTipo NUMBER NOT NULL , 
      Nome VARCHAR2 (30),
      Descricao VARCHAR2 (200),
      Requisitos VARCHAR2 (200),
      
      CONSTRAINT TipoRecurso_PK PRIMARY KEY (CodTipo)
   );
   
CREATE TABLE RecursoEletronico
   (  CodCurso NUMBER NOT NULL, 
      Cod VARCHAR2(7) NOT NULL,
      NroTurma VARCHAR2(7) NOT NULL , 
      NroRecurso NUMBER NOT NULL,
      CodTipo NUMBER NOT NULL,
      Nome VARCHAR2 (30),
      Descricao VARCHAR2 (200),
      ArquivoRecurso BLOB DEFAULT (EMPTY_BLOB()),
      
      CONSTRAINT RecursoEletronico_PK PRIMARY KEY (CodCurso, Cod, NroTurma, NroRecurso,CodTipo) , 
	 
      CONSTRAINT RecursoE_FK_CE FOREIGN KEY (CodCurso, Cod, NroTurma)
        REFERENCES CursoEletronico (CodCurso,Cod,NroTurma) ON DELETE CASCADE,
      CONSTRAINT RecursoE_FK_TipoRecurso FOREIGN KEY (CodTipo)
        REFERENCES  TipoRecurso (CodTipo) ON DELETE CASCADE
   );

CREATE TABLE LogAcesso
   (  CodCurso NUMBER NOT NULL, 
      Cod VARCHAR2(7) NOT NULL,
      NroTurma VARCHAR2(7) NOT NULL , 
      NroRecurso NUMBER NOT NULL,
      CodTipo NUMBER NOT NULL,
      NroUsp NUMBER NOT NULL,
	  CodLog NUMBER NOT NULL,
      DataHoraAcesso TIMESTAMP WITH LOCAL TIME ZONE,
      
      CONSTRAINT LogAcesso_PK PRIMARY KEY (CodCurso, Cod, NroTurma, NroRecurso,CodTipo, NroUsp, CodLog) , 
	 
      CONSTRAINT LogAcesso_FK_RE FOREIGN KEY (CodCurso, Cod, NroTurma, NroRecurso,CodTipo)
        REFERENCES RecursoEletronico (CodCurso, Cod, NroTurma, NroRecurso,CodTipo) ON DELETE CASCADE,
      CONSTRAINT LogAcesso_FK_Participante FOREIGN KEY (CodCurso, Cod, NroTurma, NroUsp)
        REFERENCES  Participante (CodCurso, Cod, NroTurma, NroUsp) ON DELETE CASCADE
   );

CREATE SEQUENCE Logs 
INCREMENT BY 1
START WITH 1
MAXVALUE 1000000000000000000000
NOCACHE 
CYCLE;

CREATE SEQUENCE Msgs
INCREMENT BY 1
START WITH 1
MAXVALUE 10000000
NOCACHE
CYCLE;