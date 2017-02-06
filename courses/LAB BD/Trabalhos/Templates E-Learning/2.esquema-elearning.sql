CREATE SEQUENCE LBD_SEQ_COD_CURSO
      INCREMENT BY 1
      START WITH 1
      MAXVALUE   999999
      NOCACHE
      NOCYCLE;

CREATE SEQUENCE LBD_SEQ_NRO_USP
      INCREMENT BY 1
      START WITH 1
      MAXVALUE   999999
      NOCACHE
      NOCYCLE;

CREATE SEQUENCE LBD_SEQ_LOG_ACESSO
      INCREMENT BY 1
      START WITH 1
      MAXVALUE   999999
      NOCACHE
      NOCYCLE;

--Criação das Tabelas      

--1.LBD01_VINCULO_USP
--tipo: 1 - aluno, 2 - funcionario, 3 - professor
CREATE TABLE LBD01_VINCULO_USP (
  NroUSP         NUMBER(7),

  TipoVinc       NUMBER(1)  NOT NULL,
  Nome           VARCHAR2(100) NOT NULL,
  DataIngresso   DATE DEFAULT SYSDATE NOT NULL,
  DataNascimento DATE,
  Ativo          CHAR(1) DEFAULT 'y',

  CONSTRAINT PK_VINCULO_USP PRIMARY KEY (NroUSP),

  CONSTRAINT CK_VINCULO_USP CHECK (TipoVinc IN (1, 2, 3)),
  CONSTRAINT CK_ATIVO CHECK (Ativo IN ('y','n'))
 );

--2.LBD02_PROFESSOR
CREATE TABLE LBD02_PROFESSOR (
  NroUSP        NUMBER(7),

  Titulacao     VARCHAR2(9) DEFAULT 'Doutorado' NOT NULL,
  Predio        NUMBER(2),
  Sala          NUMBER(2),
  
  CONSTRAINT PK_PROFESSOR PRIMARY KEY (NroUSP),
  CONSTRAINT FK_PROFESSOR_NROUSP FOREIGN KEY (NroUSP)
     REFERENCES LBD01_VINCULO_USP (NroUSP) ON DELETE CASCADE,
  CONSTRAINT CK_TITULACAO CHECK (Titulacao IN ('Mestrado','Doutorado'))
);

--3.LBD03_ALUNO
CREATE TABLE LBD03_ALUNO (
  NroUSP        NUMBER(7),

  Idade         NUMBER(2),
  CidadeNasc    VARCHAR2(50),
  Estado        VARCHAR(2),
  
  CONSTRAINT PK_ALUNO PRIMARY KEY (NroUSP),
  CONSTRAINT FK_ALUNO_NROUSP FOREIGN KEY (NroUSP)
     REFERENCES LBD01_VINCULO_USP (NroUSP) ON DELETE CASCADE,
  CONSTRAINT CK_IDADE CHECK (Idade > 10)
);

--4.LBD04_FUNCIONARIO
CREATE TABLE LBD04_FUNCIONARIO (
  NroUSP        NUMBER(7),

  Nivel         VARCHAR2(8) DEFAULT 'Basico' NOT NULL,
  Lotacao       VARCHAR2(5) DEFAULT 'ICMC' NOT NULL,
  
  CONSTRAINT PK_FUNCIONARIO PRIMARY KEY (NroUSP),
  CONSTRAINT FK_FUNCION_NROUSP FOREIGN KEY (NroUSP)
     REFERENCES LBD01_VINCULO_USP (NroUSP) ON DELETE CASCADE,
  CONSTRAINT CK_NIVEL CHECK (Nivel IN ('Basico', 'Tecnico', 'Superior')),
  CONSTRAINT CK_LOTACAO CHECK (Lotacao IN ('ICMC', 'EESC', 'IQSC', 'IFSC'))
);

--5.LBD05_DISCIPLINA
CREATE TABLE LBD05_DISCIPLINA (
  Cod	  VARCHAR2(7),

  Nome   VARCHAR2(100) NOT NULL,
  Ementa CLOB,

  CONSTRAINT PK_DISCIPLINA PRIMARY KEY (Cod)
);

--6.LBD06_LECIONA
--Relacionamento M-N
CREATE TABLE LBD06_LECIONA (
  NroUSPProf  NUMBER(7),
  CodDisc     VARCHAR2(7),

  CONSTRAINT PK_LECIONA PRIMARY KEY (NroUSPProf, CodDisc),
  --O vinculo tem que existir
  CONSTRAINT FK_LECIONA_PROF FOREIGN KEY (NroUSPProf)
  	REFERENCES LBD02_PROFESSOR(NroUSP) ON DELETE CASCADE,
  --A disciplina tem que existir
  CONSTRAINT FK_LECIONA_DISC FOREIGN KEY (CodDisc)
  	REFERENCES LBD05_DISCIPLINA(Cod) ON DELETE CASCADE
);

--7.LBD07_TURMA
--Relacionamento 1-N
--Entidade fraca, sua chave é formada pela chave de disciplina, o que facilita 
--saber a qual disciplina se refere uma turma
--Note que a unicidade de uma turma é dada por um par NroTurma-CodDisc
CREATE TABLE LBD07_TURMA (
  NroTurma   NUMBER(6),
  Ano        NUMBER(4),
  CodDisc    VARCHAR2(7),

  NroUSPProf  NUMBER(7) NOT NULL,

  CONSTRAINT PK_TURMA PRIMARY KEY (NroTurma, Ano, CodDisc),
  --A disciplina tem que existir
  CONSTRAINT FK_TURMA_DISC FOREIGN KEY (CodDisc)
  	REFERENCES LBD05_DISCIPLINA(Cod) ON DELETE CASCADE,
  --O vinculo tem que existir
  CONSTRAINT FK_TURMA_PROF FOREIGN KEY (NroUSPProf)
  	REFERENCES LBD02_PROFESSOR(NroUSP)
);

--8.LBD08_MATRICULA
--Relacionamento M-N
CREATE TABLE LBD08_MATRICULA (
  NroUSP      NUMBER(7),
  CodDisc     VARCHAR2(7),
  Ano         NUMBER(4) NOT NULL,

  NroTurma    NUMBER(6) NOT NULL,

  Nota        NUMBER(4,2),

  --Esta chave permite que um aluno se matricule apenas uma vez em
  --cada disciplina em cada ano, independentemente da turma
  CONSTRAINT PK_MATRICULA PRIMARY KEY (NroUSP, CodDisc, Ano),
  --O vinculo tem que existir
  CONSTRAINT FK_MATRICULA_VINCULADO FOREIGN KEY (NroUSP)
  	REFERENCES LBD01_VINCULO_USP(NroUSP),
  --A turma deve existir
  CONSTRAINT FK_MATRICULA_TURMA FOREIGN KEY (NroTurma,CodDisc, Ano)
  	REFERENCES LBD07_TURMA(NroTurma,CodDisc, Ano)
);

--9.LBD09_CURSO_ELETRONICO
--Relacionamento 1-1 com LBD07_TURMA
CREATE TABLE LBD09_CURSO_ELETRONICO (
  CodCurso        VARCHAR2(7),

  NroTurma        NUMBER(6) NOT NULL,
  Ano             NUMBER(4) NOT NULL,
  CodDisc         VARCHAR2(7) NOT NULL,

  AceitaExternos  CHAR(1) DEFAULT 'y' CHECK (AceitaExternos IN ( 'y', 'n' )),

  CONSTRAINT PK_CURSO_ELETRONICO PRIMARY KEY (CodCurso),
  --Esta restrição faz com que exista um único curso para uma dada turma
  CONSTRAINT UN_CURSO_ELETRONICO UNIQUE (NroTurma,CodDisc),
  --A turma tem que existir
  CONSTRAINT FK_CURSO_ELETRONICO_TURMA FOREIGN KEY (NroTurma,CodDisc,Ano)
  	REFERENCES LBD07_TURMA(NroTurma,CodDisc,Ano) ON DELETE CASCADE
);

--10.LBD10_PARTICIPANTE
--Relacionamento M-N entre LBD01_VINCULO_USP e LBD09_CURSO_ELETRONICO
--Este relacionamento determina uma entidade associativa
CREATE TABLE LBD10_PARTICIPANTE (
  CodCurso    VARCHAR2(7),
  NroUSP      NUMBER(7),

  Nota        NUMBER(4,2),
  NotaFinal   NUMBER(4,2),

  CONSTRAINT PK_PARTICIPANTE PRIMARY KEY (CodCurso, NroUSP),
  --O curso tem que existir
  CONSTRAINT FK_PARTICIPANTE_CURSO FOREIGN KEY (CodCurso)
     REFERENCES LBD09_CURSO_ELETRONICO (CodCurso),
  --O vinculo tem que existir
  CONSTRAINT FK_PARTICIPANTE_VINC FOREIGN KEY (NroUSP)
     REFERENCES LBD01_VINCULO_USP (NroUSP)
);

--11.LBD11_GRUPO
--Entidade fraca, sua chave é formada pela chave de LBD09_CURSO_ELETRONICO
--Relacionamento N-1 com LBD09_CURSO_ELETRONICO 
CREATE TABLE LBD11_GRUPO (
  NroGrupo     NUMBER(2),
  CodCurso     VARCHAR2(7),

  NomeGrupo    VARCHAR2(100),

  CONSTRAINT PK_GRUPO PRIMARY KEY (NroGrupo, CodCurso),
  --O curso tem que existir
  CONSTRAINT FK_GRUPO FOREIGN KEY (CodCurso)
  	REFERENCES LBD09_CURSO_ELETRONICO(CodCurso) ON DELETE CASCADE
);

--12.LBD12_COMPOE_GRUPO
--Esta relação considera o relacionamento LBD10_PARTICIPANTE como uma 
--entidade associativa, com a qual LBD11_GRUPO tem relacionamento M-N
CREATE TABLE LBD12_COMPOE_GRUPO (
  CodCurso    VARCHAR2(7),
  NroUSP      NUMBER(7),

  NroGrupo    NUMBER(2) NOT NULL,
  --A chave desta relação vem do fato de que um NroUSP só pode participar de um
  --único grupo por curso
  CONSTRAINT PK_COMPOE_GRUPO PRIMARY KEY(NroUSP, CodCurso),
  --O grupo tem que existir
  CONSTRAINT FK_COMPOE_GRUPO_GRUPO FOREIGN KEY(NroGrupo, CodCurso)
     REFERENCES LBD11_GRUPO(NroGrupo, CodCurso),
  --O participante tem que existir
  CONSTRAINT FK_COMPOE_GRUPO_PARTIC FOREIGN KEY(NroUSP, CodCurso)
     REFERENCES LBD10_PARTICIPANTE(NroUSP, CodCurso)
);

--13.LBD13_TRABALHO
--Entidade fraca, sua chave é formada pela chave de LBD09_CURSO_ELETRONICO
--Relacionamento N-1 com LBD09_CURSO_ELETRONICO 
CREATE TABLE LBD13_TRABALHO (
  NroTrabalho     NUMBER(2),
  CodCurso        VARCHAR2(7),

  Descricao       CLOB,
  ArquivoEspec    BLOB,
  DataHoraLimite  DATE,

  CONSTRAINT PK_TRABALHO PRIMARY KEY (NroTrabalho, CodCurso),
  --O curso eletrônico tem que existir
  CONSTRAINT FK_TRABALHO_CURSO FOREIGN KEY (CodCurso)
  	REFERENCES LBD09_CURSO_ELETRONICO(CodCurso) ON DELETE CASCADE
);

--14.LBD14_ENTREGA_TRABALHO
--Relacionamento M-N entre LBD11_GRUPO e LBD13_TRABALHO
CREATE TABLE LBD14_ENTREGA_TRABALHO (
  NroTrabalho         NUMBER(2),
  NroGrupo            NUMBER(2),
  CodCurso            VARCHAR2(7),

  DataHorarioEntrega  DATE NOT NULL,
  Arquivo             BLOB,
  Nota                NUMBER(4,2),

  CONSTRAINT PK_ENTREGA_TRABALHO PRIMARY KEY (NroTrabalho, NroGrupo, CodCurso),
  --O trabalho tem que existir
  CONSTRAINT FK_ENTREGA_TRABALHO_TRABALHO FOREIGN KEY (NroTrabalho, CodCurso)
  	REFERENCES LBD13_TRABALHO(NroTrabalho, CodCurso) ON DELETE CASCADE,
  --O grupo tem que existir
  CONSTRAINT FK_ENTREGA_TRABALHO_GRUPO FOREIGN KEY (NroGrupo, CodCurso)
  	REFERENCES LBD11_GRUPO(NroGrupo, CodCurso) ON DELETE CASCADE
);

--15.LBD15_TIPO_RECURSO
CREATE TABLE LBD15_TIPO_RECURSO (
  CodTipo      VARCHAR2(3),

  Nome         VARCHAR2(100) NOT NULL,

  Descricao    VARCHAR2(300),
  Requisitos   VARCHAR2(200),

  CONSTRAINT PK_TIPO_RECURSO PRIMARY KEY (CodTipo),
  CONSTRAINT UN_TIPO_RECURSO_NOME UNIQUE (Nome)
);

--16.LBD16_RECURSO_ELETRONICO
--Entidade fraca, sua chave é formada pela chave de LBD09_CURSO_ELETRONICO
--Relacionamento N-1 com LBD09_CURSO_ELETRONICO 
CREATE TABLE LBD16_RECURSO_ELETRONICO (
  NroRecurso      NUMBER(3),
  CodCurso        VARCHAR2(7),

  Tipo            VARCHAR2(3) NOT NULL,

  Nome            VARCHAR2(100) NOT NULL,
  Descricao       VARCHAR2(300),
  ArquivoRecurso  BLOB,

  CONSTRAINT PK_RECURSO_ELETRONICO PRIMARY KEY (NroRecurso, CodCurso),
  --Um nome de recurso pode se repitir, mas não para o mesmo curso
  CONSTRAINT UN_RECURSO_ELETRONICO_NOME UNIQUE (Nome,CodCurso),
  --O curso tem que existir
  CONSTRAINT FK_REC_ELETR_CURSO FOREIGN KEY (CodCurso)
  	REFERENCES LBD09_CURSO_ELETRONICO(CodCurso) ON DELETE CASCADE,
  --O tipo do recurso tem que existir
  CONSTRAINT FK_REC_ELETR_TIPO FOREIGN KEY (Tipo)
  	REFERENCES LBD15_TIPO_RECURSO(CodTipo)
);


--17.LBD17_LOG_ACESSO
--Relacionamento M-N entre LBD10_PARTICIPANTE e LBD16_RECURSO_ELETRONICO
--agregado como LBD17_LOG_ACESSO gerando os atributos DataHorarioAcesso e CodLog
CREATE TABLE LBD17_LOG_ACESSO (
  CodLog             NUMBER(6),

  DataHorarioAcesso  DATE NOT NULL,

  NroUSP             NUMBER(7) NOT NULL,
  CodCurso           VARCHAR2(7) NOT NULL,
  NroRecurso         NUMBER(3) NOT NULL,

  CONSTRAINT PK_LOG_ACESSO PRIMARY KEY (CodLog),
  --O recurso eletrônico tem que existir
  CONSTRAINT FK_LOG_ACESSO_RECURSO FOREIGN KEY (NroRecurso,CodCurso)
     REFERENCES LBD16_RECURSO_ELETRONICO(NroRecurso,CodCurso),
  --O participante tem que existir
  CONSTRAINT FK_LOG_ACESSO_VINCULADO FOREIGN KEY (NroUSP, CodCurso)
     REFERENCES LBD10_PARTICIPANTE(NroUSP, CodCurso)
);

--18.LBD18_MENSAGEM
----Entidade fraca, sua chave é formada pela chave de LBD10_PARTICIPANTE
CREATE TABLE LBD18_MENSAGEM (
  NroMensagem       NUMBER(4),
  CodCurso          VARCHAR2(7),
  NroUSP            NUMBER(7),

  Titulo            VARCHAR2(100) NOT NULL,
  Texto             VARCHAR2(500) NOT NULL,
  DataHoraPostagem  DATE NOT NULL,

  CONSTRAINT PK_MENSAGEM PRIMARY KEY (NroMensagem, CodCurso, NroUSP),
  --O participante tem que existir
  CONSTRAINT FK_MENSAGEM_PARTIC FOREIGN KEY (NroUSP, CodCurso)
     REFERENCES LBD10_PARTICIPANTE (NroUSP, CodCurso)
);