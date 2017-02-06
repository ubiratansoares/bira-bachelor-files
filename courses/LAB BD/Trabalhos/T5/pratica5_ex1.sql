SET SERVEROUTPUT ON;

--Dar SELECT e contar o numero de matriculados. Se ZERO, entao a tabela esta vazia

DECLARE 
  CURSOR cteste IS SELECT * FROM lbd08_matricula;
  vteste cteste%ROWTYPE;
  fetch_count NUMBER;
  tupla_count NUMBER;
  excp_sem_nota EXCEPTION;
  excp_tabela_vazia EXCEPTION;
  
BEGIN

  SELECT COUNT(*) INTO tupla_count FROM lbd08_matricula;

  IF(tupla_count = 0) THEN RAISE excp_tabela_vazia;
  
  ELSE

    OPEN cteste;

    LOOP
  
      FETCH cteste INTO vteste;

      CASE 
        WHEN(vteste.nota >= 5) THEN dbms_output.put_line('ALUNO :  ' || vteste.nrousp || '  DISCIPLINA: ' || vteste.coddisc || '  APROVADO' );
        WHEN(vteste.nota BETWEEN 3 AND 4.9) THEN dbms_output.put_line('ALUNO :  ' || vteste.nrousp || '  DISCIPLINA: ' || vteste.coddisc || '  REC' );
        WHEN(vteste.nota < 3) THEN dbms_output.put_line('ALUNO :  ' || vteste.nrousp || '  DISCIPLINA: ' || vteste.coddisc || '  REPROVADO' );
        ELSE RAISE excp_sem_nota;
      END CASE;
    
      EXIT WHEN cteste%NOTFOUND;

    END LOOP;
  
    CLOSE cteste;
  END IF;
  
  EXCEPTION
    WHEN excp_sem_nota
      THEN dbms_output.put_line('ALUNO :  ' || vteste.nrousp || '  DISCIPLINA: ' || vteste.coddisc || '  SEM NOTA' );
  
    WHEN excp_tabela_vazia
      THEN dbms_output.put_line('TABELA MATRICULA VAZIA');
  
    WHEN OTHERS
        THEN dbms_output.put_line('Erro Nro: ' || SQLCODE || '.Mensagem: ' || SQLERRM);
  
  
END;    