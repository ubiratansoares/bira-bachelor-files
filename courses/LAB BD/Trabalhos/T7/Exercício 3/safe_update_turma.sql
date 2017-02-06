create or replace
PROCEDURE safe_update_turma
( nroturmaIN lbd07_turma.nroturma%TYPE, anoIN lbd07_turma.ano%TYPE,disc lbd07_turma.coddisc%TYPE, prof lbd07_turma.nrouspprof%TYPE)
IS
total number;
BEGIN
  count_leciona(prof,disc);
  select count(*) into total from lbd07_turma where nroturma = nroturmaIN and ano = anoIN and coddisc = disc;
  
  if (total = 0) then
    RAISE_APPLICATION_ERROR(-20102,'Não é possível atualizar a turma, pois ela não existe');
  else
    update lbd07_turma set nrouspprof = prof where nroturma = nroturmaIN  and ano = anoIN and coddisc = disc;
  end if;
  
END;