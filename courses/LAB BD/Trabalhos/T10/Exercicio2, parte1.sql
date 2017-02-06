SET SERVEROUTPUT ON;
/
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;

BEGIN

SAVEPOINT inicio;

insercao_aluno.insere_aluno(100,1,'Gabriel','São Carlos','SP',SYSDATE,'y',p_datanascimento => '14/09/1991');
insercao_aluno.insere_aluno(102,1,'Gabriela','São Carlos','SP',SYSDATE,'y',p_datanascimento => '26/12/1990');
insercao_aluno.insere_aluno(103,1,'Carlos','São Carlos','SP',SYSDATE,'y',p_datanascimento => '04/04/1991');
insercao_aluno.insere_aluno(104,1,'Carla','São Carlos','SP',SYSDATE,'y',p_datanascimento => '30/11/1991');
insercao_aluno.insere_aluno(105,1,'Renato','São Carlos','SP',SYSDATE,'y',p_datanascimento => '14/06/1989');
insercao_aluno.insere_aluno(106,1,'Renata','São Carlos','SP',SYSDATE,'y',p_datanascimento => '02/01/1994');

SAVEPOINT insere2;

insercao_professor.insere_professor(299, 3, 'alberto',	'29/07/10',	'09/09/79',	'y', 'Mestrado', p_predio => 5 );
insercao_professor.insere_professor(293, 3, 'roberto',	'29/07/10',	'09/09/79',	'y', 'Mestrado', p_sala => 9);
insercao_professor.insere_professor(293, 3, 'humberto', 5,10, 	'29/07/10',	'09/09/79',	'y', 'Mestrado');

EXCEPTION --tratando o erros
    WHEN OTHERS THEN
      CASE
      --ordem correta para execução primeiro as fks dps a pk
        WHEN (SQLERRM LIKE '%FK_ALUNO_NROUSP%') THEN
           rollback to inicio;
           COMMIT;
           RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%PK_ALUNO%') THEN
            rollback to inicio;
            COMMIT;
            RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%CK_IDADE%') THEN
            rollback to inicio;
            COMMIT;
            RAISE_APPLICATION_ERROR(-20000,'Idade inválida');
        WHEN (SQLERRM LIKE '%FK_PROFESSOR_NROUSP%') THEN
           rollback to insere2;
           COMMIT;
           RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%PK_PROFESSOR%') THEN
            rollback to insere2;
            COMMIT;
            RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%CK_TITULACAO%') THEN
            rollback to insere2;
            COMMIT;
            RAISE_APPLICATION_ERROR(-20000,'Titulação inválida');
        ELSE
            rollback to inicio;
            COMMIT;
            RAISE_APPLICATION_ERROR(-20105,SQLERRM);
      END CASE;
END;
