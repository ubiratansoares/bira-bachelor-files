create or replace
PROCEDURE safe_insert_turma
( nturma lbd07_turma.nroturma%TYPE, ano lbd07_turma.ano%TYPE,disc lbd07_turma.coddisc%TYPE, prof lbd07_turma.nrouspprof%TYPE)
IS
BEGIN
  count_leciona(prof,disc);
  insert into lbd07_turma values (nturma, ano, disc, prof);
  
EXCEPTION --tratando o erros
  WHEN OTHERS THEN
    CASE
      -- SQLERRM = variavel global q o oracle preenche no exception, preenchida com o valor da exceção
      WHEN (SQLERRM LIKE '%FK_TURMA_DISC%') THEN --se for fk referente a disciplina
        RAISE_APPLICATION_ERROR(-20000,'Erro, a disciplina não existe');
      WHEN (SQLERRM LIKE '%FK_TURMA_PROF%') THEN --se for fk referente a professor
        RAISE_APPLICATION_ERROR(-20000,'Erro, professor não existe');
      WHEN (SQLERRM LIKE '%PK_TURMA%') THEN
        RAISE_APPLICATION_ERROR(-20000,'Relacionamento já inserido');
      ELSE
         RAISE_APPLICATION_ERROR(-20101,SQLERRM);
    END CASE;

END;