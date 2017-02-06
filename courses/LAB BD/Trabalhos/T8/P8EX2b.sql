DECLARE 

CURSOR c1 IS SELECT * FROM lbd07_turma FOR UPDATE OF NroAlunos;

aux c1%ROWTYPE;

total NUMBER;

BEGIN
  
  OPEN c1;
  
  LOOP
  
    FETCH c1 INTO aux;
    
    SELECT COUNT(*) INTO total FROM lbd08_matricula M WHERE M.coddisc = aux.coddisc AND M.nroturma = aux.nroturma; 
  
    UPDATE lbd07_turma SET nroalunos = total WHERE CURRENT OF c1;
  
    EXIT WHEN c1%NOTFOUND; 
  
  END LOOP;

  EXCEPTION
    WHEN OTHERS
        THEN dbms_output.put_line('Erro Nro: ' || SQLCODE || '.Mensagem: ' || SQLERRM);

END;

--rollback