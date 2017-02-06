INSERT INTO VinculoUsp VALUES (0231223,'Oziride Manzoli Neto',TO_DATE('JAN 15, 1973', 'MON DD, YYYY'),'Sim','1');
INSERT INTO VinculoUsp VALUES (8903321,'Jose Fernando Rodrigues Junior','10-JAN-10','Sim','1');
INSERT INTO VinculoUsp VALUES (7903321,'Gustavo Enrique De Almeida Prado Alves Batista','15-JAN-07','Sim','1');
INSERT INTO VinculoUsp VALUES (6903321,'Elaine Parros Machado De Sousa','13-JAN-00','Sim','1');
INSERT INTO VinculoUsp VALUES (3903321,'Raimundo Nonato Araújo Dos Santos',TO_DATE('JUN 16, 1987', 'MON DD, YYYY'),'Sim','1');
INSERT INTO VinculoUsp VALUES (0031220,'Joaquim Neves',TO_DATE('JAN 15, 1950', 'MON DD, YYYY'),'Nao','1');

INSERT INTO VinculoUsp VALUES (5634271,'Thiago Teixeira','03-02-06','Sim','2');
INSERT INTO VinculoUsp VALUES (5634292,'Ubiratan Francamar Soares','03-02-06','Sim','2');
INSERT INTO VinculoUsp VALUES (5634820,'Valter Moraes','03-02-06','Sim','2');
INSERT INTO VinculoUsp VALUES (6426399,'Luis Augusto Bin','03-02-07','Sim','2');
INSERT INTO VinculoUsp VALUES (7153334,'Camila Fernanda Amato Antunes','03-02-10','Sim','2');
INSERT INTO VinculoUsp VALUES (7634271,'Clara Maria Neves','03-02-10','Sim','2');
INSERT INTO VinculoUsp VALUES (7634275,'Paola Oliveira','03-02-10','Sim','2');
INSERT INTO VinculoUsp VALUES (6634271,'Uoshington Renato Flamigon','03-02-07','Sim','2');
INSERT INTO VinculoUsp VALUES (6634891,'Paula Cristhina Waltz','03-02-07','Sim','2');
INSERT INTO VinculoUsp VALUES (1634271,'Abigail Costa','03-02-00','Sim','2');
INSERT INTO VinculoUsp VALUES (1638571,'Robervaldo Wescley Junior','03-02-00','Sim','2');
INSERT INTO VinculoUsp VALUES (3634271,'Mirian Caio da Costa','03-02-03','Sim','2');
INSERT INTO VinculoUsp VALUES (1604271,'Joao Roberto Augusto','03-02-00','Nao','2');

INSERT INTO VinculoUsp VALUES (9636371,'Romero Andrade','15-JAN-04','Sim','3');
INSERT INTO VinculoUsp VALUES (973421,'Hong Ping Jo','15-JAN-03','Sim','3');
INSERT INTO VinculoUsp VALUES (9903321,'Joao Renato Souza Junior','01-JUN-01','Nao','3');
INSERT INTO VinculoUsp VALUES (2903321,'Maria Claudia da Silva',TO_DATE('MAR 23, 1997', 'MON DD, YYYY'),'Sim','3');
--#################################################################################################################################

INSERT INTO Disciplina VALUES ('SCC0241', 'Laboratorio de Bases de Dados',
'Linguagem SQL: comandos de definição de dados e de manipulação de dados. 
Integridade e segurança de banco de dados: conceitos e comandos SQL. 
Comandos analíticos em SQL. Visões, gatilhos (triggers) e procedimentos armazenados (stored procedures). 
Acesso multiusuário em bancos de dados. Projeto de sistemas usando a tecnologia cliente/servidor em 
bancos de dados.' );

INSERT INTO Disciplina VALUES ('SCC0240', 'Banco de Dados', 
'Introdução a banco de dados e sistemas gerenciadores de banco de dados. 
Arquitetura ANSI-X3-SPARC: nível físico, nível conceitual e nível externo. 
Modelagem de dados utilizando o modelo entidade-relacionamento. O modelo relacional: 
conceitos, álgebra relacional e normalização. Introdução a SQL: comandos de definição de 
dados e de manipulação de dados. Introdução ao processamento de transações: conceitos e teoria. 
Aspectos de implementação dos sistemas gerenciadores de banco de dados.' );

INSERT INTO Disciplina VALUES ('SMA0333', 'Calculo III',
'Seqüências numéricas. Séries numéricas. Critérios de convergência e divergência para 
séries de termos positivos. Séries absolutamente convergentes. Critérios de Cauchy e de Dirichlet.
Seqüências de funções. Séries de funções. Séries de potências. Séries de Fourier: Equação do calor e 
da onda como motivação para o estudo das Séries de Fourier. Séries de Fourier para funções pares e 
ímpares. Teorema de Fourier. Aplicações' );

INSERT INTO Disciplina VALUES ('SMA0180', 'Matematica Discreta I',
'1. Provas: Princípio da Indução Finita e Redução ao Absurdo;2. Problemas Recorrentes: 
Torres de Hanói, Problema de Josephus, Regiões do Plano;3. Somas: Notação, Somas e Recursões, 
Manipulação de Somas, Somas Telescópicas;Somas Múltiplas.4. Teoria dos Números: Divisibilidade, 
Primos, MDC e algoritmo de Euclides,Congruências, Teorema de Euler-Fermat, aplicações para a 
computação: algoritmo decriptografia RSA e geradores de números aleatórios;5. Grupos: grupos, 
subgrupos, grupos de permutação, teorema de Lagrange;6. Fundamentos: lógica sentencial, conjuntos, 
funções e relações, enumerabilidade,Diagonalização de Cantor e o Problema da Parada (Halting Problem)' );

INSERT INTO Disciplina VALUES ('SME0320', 'Estatistica I',
'Análise Exploratória de Dados (Estatística Descritiva). O espaço probabilístico. 
Modelos probabilísticos. Dependência e independência de eventos. Eventos condicionados. 
Variáveis aleatórias unidimensionais e n-dimensionais. Distribuições de probabilidade. 
Funções de variáveis aleatórias. Esperança matemática. Momentos. Covariância e correlação. 
Teorema do limite central. Estimação de parâmetros. Testes de hipóteses. Testes de aderência. 
Regressão linear.' );

INSERT INTO Disciplina VALUES ('SSC0124', 'Analise e Projeto Orientados a Objetos',
'Visão geral dos métodos para análise e projeto orientados a objetos, em particular o 
Processo Unificado. Comparação entre os principais métodos. Como modelar com objetos usando 
a notação UML: o modelo conceitual; o modelo comportamental: diagrama de seqüência, operações, 
contratos; o modelo de interação: casos de usos, colaboração entre objetos e diagramas de comunicação.
 Padrões para atribuição de responsabilidades e padrões de projeto. O problema de persistência. 
 Ferramentas de apoio.' );
 
INSERT INTO Disciplina VALUES ('SCC0203', 'Algoritmos e Estruturas de Dados II',
'Organização de arquivos. Algoritmos para classificação externa. Conceito de indexação de arquivos. 
Processamento cossequencial e ordenação de arquivos grandes. Árvores B e suas variações. 
Estruturas de dados para representação de grafos, e algoritmos clássicos sobre grafos.' );
--#################################################################################################################################
INSERT INTO Leciona VALUES (231223, 'SMA0333');
INSERT INTO Leciona VALUES (231223, 'SMA0180');
INSERT INTO Leciona VALUES (3903321, 'SMA0180');
INSERT INTO Leciona VALUES (3903321, 'SME0320');
INSERT INTO Leciona VALUES (8903321, 'SCC0241');
INSERT INTO Leciona VALUES (8903321, 'SCC0240');
INSERT INTO Leciona VALUES (6903321, 'SCC0241');
INSERT INTO Leciona VALUES (6903321, 'SCC0240');
INSERT INTO Leciona VALUES (7903321, 'SSC0124');
INSERT INTO Leciona VALUES (7903321, 'SCC0203');
--#################################################################################################################################
INSERT INTO Turma VALUES ('SCC0203', '2010-01');
INSERT INTO Turma VALUES ('SCC0240', '2010-01');
INSERT INTO Turma VALUES ('SCC0241', '2010-01');
INSERT INTO Turma VALUES ('SCC0203', '2010-02');
INSERT INTO Turma VALUES ('SCC0240', '2010-02');
INSERT INTO Turma VALUES ('SCC0241', '2010-02');
INSERT INTO Turma VALUES ('SSC0124', '2010-01');
INSERT INTO Turma VALUES ('SMA0333', '2010-01');
INSERT INTO Turma VALUES ('SMA0180', '2010-01');
INSERT INTO Turma VALUES ('SME0320', '2010-01');
--#################################################################################################################################
--Thiago
INSERT INTO Matricula_se VALUES (5634271,'SME0320', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634271,'SCC0241', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634271,'SMA0180', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634271,'SMA0333', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634271,'SCC0203', '2010-01',NULL);
--Ubiratan
INSERT INTO Matricula_se VALUES (5634292,'SME0320', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634292,'SCC0241', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634292,'SMA0180', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634292,'SMA0333', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634292,'SCC0203', '2010-01',NULL);
--Valter
INSERT INTO Matricula_se VALUES (5634820,'SME0320', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634820,'SCC0241', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634820,'SMA0180', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634820,'SMA0333', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (5634820,'SCC0203', '2010-01',NULL);
--Luis Augusto
INSERT INTO Matricula_se VALUES (6426399,'SCC0240', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (6426399,'SCC0241', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (6426399,'SCC0203', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (6426399,'SME0320', '2010-01',NULL);
--Uoshington
INSERT INTO Matricula_se VALUES (6634271,'SCC0240', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (6634271,'SCC0241', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (6634271,'SCC0203', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (6634271,'SME0320', '2010-01',NULL);
--Paula
INSERT INTO Matricula_se VALUES (6634891,'SCC0240', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (6634891,'SCC0241', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (6634891,'SCC0203', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (6634891,'SME0320', '2010-01',NULL);
--Camila
INSERT INTO Matricula_se VALUES (7153334,'SMA0333', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (7153334,'SMA0180', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (7153334,'SCC0203', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (7153334,'SCC0240', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (7153334,'SSC0124', '2010-01',NULL);
--Clara
INSERT INTO Matricula_se VALUES (7634271,'SMA0333', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (7634271,'SMA0180', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (7634271,'SCC0203', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (7634271,'SCC0240', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (7634271,'SSC0124', '2010-01',NULL);
--Paola
INSERT INTO Matricula_se VALUES (7634275,'SMA0333', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (7634275,'SMA0180', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (7634275,'SCC0203', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (7634275,'SCC0240', '2010-02',NULL);
INSERT INTO Matricula_se VALUES (7634275,'SSC0124', '2010-01',NULL);
--Abigail
INSERT INTO Matricula_se VALUES (1634271,'SMA0333', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (1634271,'SMA0180', '2010-01',NULL);
--Robervaldo
INSERT INTO Matricula_se VALUES (1638571,'SMA0333', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (1638571,'SMA0180', '2010-01',NULL);
--Mirian
INSERT INTO Matricula_se VALUES (3634271,'SMA0333', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (3634271,'SMA0180', '2010-01',NULL);
INSERT INTO Matricula_se VALUES (3634271,'SCC0240', '2010-01',NULL);
--#################################################################################################################################
INSERT INTO CursoEletronico VALUES (20321,'SCC0203','2010-01','Nao');
INSERT INTO CursoEletronico VALUES (20321,'SCC0203','2010-02','Sim');
INSERT INTO CursoEletronico VALUES (24021,'SCC0240','2010-01','Nao');
INSERT INTO CursoEletronico VALUES (24021,'SCC0240','2010-02','Sim');
INSERT INTO CursoEletronico VALUES (24121,'SCC0241','2010-01','Nao');
INSERT INTO CursoEletronico VALUES (24121,'SCC0241','2010-02','Sim');
INSERT INTO CursoEletronico VALUES (33321,'SMA0333','2010-01','Sim');
INSERT INTO CursoEletronico VALUES (18021,'SMA0180','2010-01','Sim');
INSERT INTO CursoEletronico VALUES (32021,'SME0320','2010-01','Sim');
INSERT INTO CursoEletronico VALUES (12421,'SSC0124','2010-01','Sim');
--#################################################################################################################################
--Romero funcionario
INSERT INTO Participante VALUES (9636371 ,20321,'SCC0203','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (9636371 ,24021,'SCC0240','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (9636371 ,24121,'SCC0241','2010-02',NULL,NULL);
--Alunos de SMA180
INSERT INTO Participante VALUES (5634271,18021,'SMA0180','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (5634292,18021,'SMA0180','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (5634820,18021,'SMA0180','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7153334,18021,'SMA0180','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7634271,18021,'SMA0180','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7634275,18021,'SMA0180','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (1634271,18021,'SMA0180','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (1638571,18021,'SMA0180','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (3634271,18021,'SMA0180','2010-01',NULL,NULL);
--Professores da disciplina
INSERT INTO Participante VALUES (3903321,18021,'SMA0180','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (231223,18021,'SMA0180','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (2903321,18021,'SMA0180','2010-01',NULL,NULL);--Funcionario
--Alunos de SMA0333
INSERT INTO Participante VALUES (231223,33321,'SMA0333','2010-01',NULL,NULL);--Professor
INSERT INTO Participante VALUES (5634271,33321,'SMA0333','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (5634292,33321,'SMA0333','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (5634820,33321,'SMA0333','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (1634271,33321,'SMA0333','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (1638571,33321,'SMA0333','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (3634271,33321,'SMA0333','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7153334,33321,'SMA0333','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7634271,33321,'SMA0333','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7634275,33321,'SMA0333','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (2903321,33321,'SMA0333','2010-01',NULL,NULL);--Funcionario
--Alunos SME0320
INSERT INTO Participante VALUES (5634271,32021,'SME0320','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (5634292,32021,'SME0320','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (5634820,32021,'SME0320','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (6426399,32021,'SME0320','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (6634271,32021,'SME0320','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (6634891,32021,'SME0320','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (3903321,32021,'SME0320','2010-01',NULL,NULL);--Professor
INSERT INTO Participante VALUES (2903321,32021,'SME0320','2010-01',NULL,NULL);--Funcionario
--Alunos SCC0203 turma 02
INSERT INTO Participante VALUES (6426399,20321,'SCC0203','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (6634271,20321,'SCC0203','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (6634891,20321,'SCC0203','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (7903321,20321,'SCC0203','2010-02',NULL,NULL);--Professor
--Alunos SCC0203 turma 01
INSERT INTO Participante VALUES (5634271,20321,'SCC0203','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (5634292,20321,'SCC0203','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (5634820,20321,'SCC0203','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7153334,20321,'SCC0203','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7634271,20321,'SCC0203','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7634275,20321,'SCC0203','2010-01',NULL,NULL);
--Alunos SCC0240 turma 01
INSERT INTO Participante VALUES (3634271,24021,'SCC0240','2010-01',NULL,NULL);
--Professores de SCC0240
INSERT INTO Participante VALUES (8903321,24021,'SCC0240','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (6903321,24021,'SCC0240','2010-02',NULL,NULL);
--Alunos SCC0240 turma 02
INSERT INTO Participante VALUES (6426399,24021,'SCC0240','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (6634891,24021,'SCC0240','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (6634271,24021,'SCC0240','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (7153334,24021,'SCC0240','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (7634271,24021,'SCC0240','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (7634275,24021,'SCC0240','2010-02',NULL,NULL);
--Professores de SCC0241
INSERT INTO Participante VALUES (8903321,24121,'SCC0241','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (6903321,24121,'SCC0241','2010-02',NULL,NULL);
--Alunos de SCC0241 turma 01
INSERT INTO Participante VALUES (5634271,24121,'SCC0241','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (5634292,24121,'SCC0241','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (5634820,24121,'SCC0241','2010-01',NULL,NULL);
--Alunos de SCC0241 turma 02
INSERT INTO Participante VALUES (6426399,24121,'SCC0241','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (6634891,24121,'SCC0241','2010-02',NULL,NULL);
INSERT INTO Participante VALUES (6634271,24121,'SCC0241','2010-02',NULL,NULL);
--Alunos de SSC0124
INSERT INTO Participante VALUES (7903321,12421,'SSC0124','2010-01',NULL,NULL);--Professor
INSERT INTO Participante VALUES (7153334,12421,'SSC0124','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7634271,12421,'SSC0124','2010-01',NULL,NULL);
INSERT INTO Participante VALUES (7634275,12421,'SSC0124','2010-01',NULL,NULL);
--#################################################################################################################################
--Grupos do SME320
INSERT INTO Grupo VALUES (32021,'SME0320','2010-01',1,'Gases Nobres');
INSERT INTO Grupo VALUES (32021,'SME0320','2010-01',2,'Metais');
--Grupos do SCC0203 t1
INSERT INTO Grupo VALUES (20321,'SCC0203','2010-01',1,'Merak');
INSERT INTO Grupo VALUES (20321,'SCC0203','2010-01',2,'Wyers');
INSERT INTO Grupo VALUES (20321,'SCC0203','2010-01',3,'Bwear');
--Grupos do SCC0203 t2
INSERT INTO Grupo VALUES (20321,'SCC0203','2010-02',1,'Hosde');
INSERT INTO Grupo VALUES (20321,'SCC0203','2010-02',2,'Mega');
--Grupos do SCC0240 t1
INSERT INTO Grupo VALUES (24021,'SCC0240','2010-01',1,'Cartogena');
--Grupos do SCC0240 t2
INSERT INTO Grupo VALUES (24021,'SCC0240','2010-02',1,'America');
INSERT INTO Grupo VALUES (24021,'SCC0240','2010-02',2,'Africa');
INSERT INTO Grupo VALUES (24021,'SCC0240','2010-02',3,'Asia');
--Grupos do SCC0241 t1
INSERT INTO Grupo VALUES (24121,'SCC0241','2010-01',1,'Omega');
INSERT INTO Grupo VALUES (24121,'SCC0241','2010-01',2,'Lambida');
--Grupos do SCC0241 t2
INSERT INTO Grupo VALUES (24121,'SCC0241','2010-02',1,'Teta');
INSERT INTO Grupo VALUES (24121,'SCC0241','2010-02',2,'Zeta');
--Grupos do SSC0124 
INSERT INTO Grupo VALUES (12421,'SSC0124','2010-01',1,'Alfa');
INSERT INTO Grupo VALUES (12421,'SSC0124','2010-01',2,'Beta');
--#################################################################################################################################
--SME0320
INSERT INTO Composicao VALUES (32021,'SME0320','2010-01',1,5634271);
INSERT INTO Composicao VALUES (32021,'SME0320','2010-01',1,5634292);
INSERT INTO Composicao VALUES (32021,'SME0320','2010-01',1,5634820);
INSERT INTO Composicao VALUES (32021,'SME0320','2010-01',1,6426399);
INSERT INTO Composicao VALUES (32021,'SME0320','2010-01',2,6634271);
INSERT INTO Composicao VALUES (32021,'SME0320','2010-01',2,6634891);
--SCC0203 t1
INSERT INTO Composicao VALUES (20321,'SCC0203','2010-01',1,5634271);
INSERT INTO Composicao VALUES (20321,'SCC0203','2010-01',1,5634292);
INSERT INTO Composicao VALUES (20321,'SCC0203','2010-01',2,5634820);
INSERT INTO Composicao VALUES (20321,'SCC0203','2010-01',2,7153334);
INSERT INTO Composicao VALUES (20321,'SCC0203','2010-01',3,7634271);
INSERT INTO Composicao VALUES (20321,'SCC0203','2010-01',3,7634275);
--SCC0203 t2
INSERT INTO Composicao VALUES (20321,'SCC0203','2010-02',1,6426399);
INSERT INTO Composicao VALUES (20321,'SCC0203','2010-02',1,6634271);
INSERT INTO Composicao VALUES (20321,'SCC0203','2010-02',2,6634891);
--SCC0240 t1
INSERT INTO Composicao VALUES (24021,'SCC0240','2010-01',1,3634271);
--SCC240 t2
INSERT INTO Composicao VALUES (24021,'SCC0240','2010-02',1,6426399);
INSERT INTO Composicao VALUES (24021,'SCC0240','2010-02',1,6634891);
INSERT INTO Composicao VALUES (24021,'SCC0240','2010-02',2,6634271);
INSERT INTO Composicao VALUES (24021,'SCC0240','2010-02',2,7153334);
INSERT INTO Composicao VALUES (24021,'SCC0240','2010-02',3,7634271);
INSERT INTO Composicao VALUES (24021,'SCC0240','2010-02',3,7634275);
--SCC0241 t1
INSERT INTO Composicao VALUES (24121,'SCC0241','2010-01',1,5634271);
INSERT INTO Composicao VALUES (24121,'SCC0241','2010-01',1,5634292);
INSERT INTO Composicao VALUES (24121,'SCC0241','2010-01',2,5634820);
--SCC0241 t2
INSERT INTO Composicao VALUES (24121,'SCC0241','2010-02',1,6426399);
INSERT INTO Composicao VALUES (24121,'SCC0241','2010-02',1,6634891);
INSERT INTO Composicao VALUES (24121,'SCC0241','2010-02',2,6634271);
--SSC0124
INSERT INTO Composicao VALUES (12421,'SSC0124','2010-01',1,7153334);
INSERT INTO Composicao VALUES (12421,'SSC0124','2010-01',1,7634271);
INSERT INTO Composicao VALUES (12421,'SSC0124','2010-01',2,7634275);
--#################################################################################################################################
INSERT INTO TipoRecurso VALUES (3321,'Video Aula',
'Principais trechos das explicações do professor disponibilizadas para o aluno, para que este possa ter melhor
entendimento do conteúdo explanado','Plug-in do flash instalado e caixas de som');
INSERT INTO TipoRecurso VALUES (3322,'Audio Aula','Disponibilização da aula editada pelo professor para que
o aluno posso ouví-la em casa','Plug-ins de audio e caixas de som');
INSERT INTO TipoRecurso VALUES (1223,'Slides','Slides das aulas do professor','Leitor de .pdf');
--#################################################################################################################################
--Slides
INSERT INTO RecursoEletronico VALUES (20321,'SCC0203','2010-02',1,1223,'Slides','Teoria das aulas em arquivo .pdf',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (24021,'SCC0240','2010-02',1,1223,'Slides','Teoria das aulas em arquivo .pdf',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (24121,'SCC0241','2010-02',1,1223,'Slides','Teoria das aulas em arquivo .pdf',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (33321,'SMA0333','2010-01',1,1223,'Slides','Teoria das aulas em arquivo .pdf',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (18021,'SMA0180','2010-01',1,1223,'Slides','Teoria das aulas em arquivo .pdf',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (32021,'SME0320','2010-01',1,1223,'Slides','Teoria das aulas em arquivo .pdf',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (12421,'SSC0124','2010-01',1,1223,'Slides','Teoria das aulas em arquivo .pdf',EMPTY_BLOB());
--Audio
INSERT INTO RecursoEletronico VALUES (20321,'SCC0203','2010-02',2,3321,'Audio Aulas','Arquivo sonoro da aula',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (18021,'SMA0180','2010-01',2,3321,'Audio Aulas','Arquivo sonoro da aula',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (32021,'SME0320','2010-01',2,3321,'Audio Aulas','Arquivo sonoro da aula',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (12421,'SSC0124','2010-01',2,3321,'Audio Aulas','Arquivo sonoro da aula',EMPTY_BLOB());
--Video
INSERT INTO RecursoEletronico VALUES (24021,'SCC0240','2010-02',3,3322,'Video Aulas','Video arquivo da aula',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (24121,'SCC0241','2010-02',3,3322,'Video Aulas','Video arquivo da aula',EMPTY_BLOB());
INSERT INTO RecursoEletronico VALUES (33321,'SMA0333','2010-01',3,3322,'Video Aulas','Video arquivo da aula',EMPTY_BLOB());
--#################################################################################################################################
INSERT INTO Trabalho VALUES (20321,'SCC0203','2010-02',1,'A aula de hoje consiste em fazer alguns programas que sejam capazes 
de ler arquivos organizados em campos e registros.',
'Existem três arquivos: 
•  fixo.dad é um arquivo com campos e registros de tamanho fixo; 
•  campo_var_reg_fixo.dad é um arquivo com campos de tamanho variável e 
registros de tamanho fixo; 
•  campo_var_reg_var.dad é um arquivo com campos e registros de tamanho 
variável. 
 
Todos os arquivos possuem a mesma informação: dez registros com os seguintes 
campos: 
1. Um nome com no máximo 15 caracteres incluindo o  caractere terminador de 
string; 
2. Um sobrenome com no máximo 15 caracteres incluindo o caractere terminador 
de string; 
3. Um nome de rua com no máximo 25 caracteres incluindo o caractere 
terminador de string; 
4. Um número (da casa) que é um inteiro com 4 bytes (int). 
 
O objetivo é fazer um programa para cada organização de arquivo descrita acima. 
Esse programa deve inicialmente imprimir os registros contidos no arquivo e logo em 
seguida pedir ao usuário um número entre 1 e 10 e imprimir o registro correspondente. 
Como material de apoio, utilize os slides da aula 1 de arquivos disponível na coteia. 
Nessa aula pode-se encontrar as principais funções  e procedimentos para 
manipulação de arquivos em C e Pascal.','18/08/2010 00:00:00');

INSERT INTO Trabalho VALUES (24121,'SCC0241','2010-02',1,
'A partir do diagrama, crie o projeto lógico para o sistema, isto é, faça o mapeamento do DER para o Modelo Relacional.',
'Descrição 
 
Sistemas de E-Learning, tais como Moodle e Tidia, têm tido crescente importância no ensino em 
diversos níveis, oferecendo suporte eletrônico de conteúdo e de atividades práticas (trabalhos) para 
disciplinas em milhares de instituições em todo o mundo. Neste trabalho deseja-se criar uma base de 
dados para armazenar informações sobre disciplinas, turmas, alunos, professores, funcionários, 
cursos eletrônicos, alunos participantes, recursos eletrônicos, trabalhos a serem feitos, e avaliação. 
Tais informações devem ser armazenadas respeitando uma semântica abrangente, mas com algumas 
restrições, de maneira que se possa ter um sistema de E-Learning funcionando sobre estes dados de 
maneira robusta. 
 
As informações do sistema são: 
 
-A entidade principal é o Vínculo USP, o que se refere a uma pessoa que pode ser ou um 
aluno, ou um professor ou um funcionário. 
-Cada professor é habilitado a lecionar um conjunto específico de disciplinas; cada disciplina é 
instanciada por uma ou mais turmas, sendo que cada turma tem um  único  professor  para 
ministrá-la. Cada turma possui um conjunto de alunos matriculados, os quais terão uma nota 
atribuída. Cada turma tem também um curso eletrônico criado imediatamente após a criação 
de uma dada turma.
-Um curso eletrônico possui participantes com Vínculo USP. A lista de participantes de um 
curso deve ser formada por todos os alunos de sua respectiva turma e, opcionalmente, por 
participantes externos com Vínculo USP  –  alunos,  professores ou funcionários  –  não 
matriculados na turma.  Também são aceitos cursos apenas com participantes externos; 
mesmo neste caso, o curso eletrônico deve ser vinculado a uma turma. 
-Cada curso eletrônico possui um conjunto de recursos eletrônicos, isto é, textos, vídeos, 
imagens, arquivos compactados, entre outros. Cada recurso é parte de um único curso, e só é 
exibido para seus respectivos participantes.  Os recursos são adicionados ao sistema pelo 
professor responsável pelo curso, que é o mesmo professor vinculado à turma. Cada vez que 
um participante acessa um recurso, um registro de log é armazenado no sistema. 
-Os cursos eletrônicos têm trabalhos que são propostos como atividades para os participantes. 
Para a realização destes trabalhos, os participantes devem se organizar em grupos com um ou 
mais  indivíduos. Os trabalhos devem ser entregues em um único arquivo até uma 
determinada data que faz parte da definição do trabalho. O upload de um trabalho é feito por 
um participante de um dado grupo. Cada trabalho entregue terá uma nota associada 
posteriormente pelo professor. 
-Cada curso também possui um fórum, onde os participantes podem postar mensagens 
referentes ao curso. Cada mensagem é composta por um título, por um texto, um autor 
(participante) e uma data de postagem.', '13/08/2010 14:20:00');
 
INSERT INTO Trabalho VALUES (24121,'SCC0241','2010-02',2,'SQL - DDL e DML',
'1.  Usando o SQL Developer 
a.  conecte-se com o seu usuário (veja padrão apresentado na aula Oracle); 
b.  altere sua senha (veja comando na aula Oracle) 
c.  desconecte-se;  
d.  conecte-se novamente com a nova senha. 
 
2.  Crie o esquema da base  de  dados usando o projeto lógico  do  sistema de E-Learning 
desenvolvido na Prática 1. Nesta prática, se a dupla tiver em mãos 2 projetos lógicos (1 de cada 
aluno, vindos de duplas diferentes na Prática 1), deve-se escolher o projeto mais correto e usar 
nesta prática. Se, durante a prática, forem identificados erros no projeto lógico, a dupla deve 
fazer as correções no projeto usado, e desenvolver a Prática 2 de acordo com as correções 
feitas.  Neste caso, o  projeto lógico  novo,  usado para a Prática 2,  deve ser entregue 
OBRIGATORIAMENTE, como parte do relatório. Se não  houver correção, apenas indique no 
relatório que foi usado o mesmo projeto da Prática 1. 
 
a.  (0.5) crie seqüências  e use-as  para atributos  numéricos sequenciais  de  seu projeto 
lógico – ex: Número de log de acesso 
 
b.  (5.0) tabelas – crie todas as tabelas do projeto lógico, incluindo: 
i.  restrições de integridade – defina as chaves primárias e secundárias, e as chaves 
estrangeiras adequadas para o esquema relacional, além das ações adequadas 
para remoção de tuplas relacionadas; ii.  restrições de valor nulo – as restrições devem atender aos requisitos colocados 
no DER, no projeto lógico e na descrição do sistema (cardinalidade, restrições 
de especialização, participação total, etc....) 
iii.  valores  default  –  usar valores apropriados de acordo com a semântica da 
aplicação  
iv.  restrições de verificação de validade dos dados (ex: intervalo válido ou conjunto 
de valores possíveis...) 
v.  Use os  tipo de dados  BLOB  e CLOB  para  atributos como Ementa, 
ArquivoRecurso, ArquivoEspec, e Arquivo (de trabalho entregue). 
 
 
3.  Usando o esquema relacional criado no exercício 2: 
a.  (2.5) Insira pelos menos 2 tuplas em cada tabela da base de dados. Requisitos:  
?  As seqüências criadas devem ser usadas na inserção dos dados.  
?  Insira valores default e valores null.  
?  Nos atributos de data e  hora devem ser usadas as funções de manipulação e 
formatação de data e hora dos  tipos  de dados  específicos  para data e hora (ex:. 
DATE, TIMESTAMP).   
?  Os dados  Large OBject  (tipos BLOB, CLOB) não devem  receber valores nulos, mas 
sim vazios (EMPTY_BLOB(), EMPTY_CLOB()).  b.  (0.5) Faça as seguintes alterações:  
?  Escolha uma tabela e faça uma alteração nos valores de 2 ou mais atributos de um 
conjunto de tuplas. Use uma condição de localização para encontrar as tuplas a 
serem modificadas. 
?  Escolha uma tabela e remova uma ou mais tuplas.  Use uma condição de localização 
para encontrar as tuplas a serem removidas. 
 
 
4.  Faça as seguintes alterações no esquema da base de dados: (OBS: faça escolhas que tenham 
sentido no domínio do problema...) 
a.  (0.5) Escolha uma tabela e insira um novo atributo com as restrições apropriadas. Use 
CHECK e valor default.  Explique o comportamento do SGBD com relação ao conteúdo 
da tabela: como  ficam  os valores deste novo atributo nas tuplas previamente 
existentes? Faça testes.  
 
b.  (0.5) Escolha uma tabela e insira uma nova restrição de tabela.  
 
c.  (0.5) Escolha uma tabela com chave primária  (PK) definida por apenas um atributo, e 
que seja referenciada por uma chave estrangeira (FK) de outra tabela. Tente remover a 
PK.  Teste o comando com e sem  CASCADE. Explique o que acontece. No caso do 
CASCADE, explique o que acontece na tabela em que havia a PK e na tabela em que 
havia a FK (use comando DESCRIBE).','20/08/2010 14:20:00');
--#################################################################################################################################
INSERT INTO Mensagem VALUES (6634271,24121,'SCC0241','2010-02',Msgs.NEXTVAL,'Duvida T2','Professor o trabalho deve ser entregue na coteia?', SYSDATE);
INSERT INTO Mensagem VALUES (8903321,24121,'SCC0241','2010-02',Msgs.NEXTVAL,'Re: Duvida T2','Sim, o trabalho deve ser entregue via coteia.', SYSDATE);
--#################################################################################################################################
INSERT INTO Entrega VALUES (24121,'SCC0241','2010-02',1,1,EMPTY_BLOB(),NULL,SYSDATE);
INSERT INTO Entrega VALUES (20321,'SCC0203','2010-02',1,1,EMPTY_BLOB(),NULL,SYSDATE);
--#################################################################################################################################
INSERT INTO LogAcesso VALUES (24121,'SCC0241','2010-02',1,1223,8903321,Logs.NEXTVAL,SYSDATE);
INSERT INTO LogAcesso VALUES (24021,'SCC0240','2010-02',1,1223,6903321,Logs.NEXTVAL,SYSDATE);
INSERT INTO LogAcesso VALUES (33321,'SMA0333','2010-01',1,1223,231223,Logs.NEXTVAL,SYSDATE);
--#################################################################################################################################
