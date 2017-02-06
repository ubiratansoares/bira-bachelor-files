SET SERVEROUTPUT ON;

create or replace
PROCEDURE safe_delete_leciona
( p_nrousp lbd02_professor.nrousp%TYPE, d_cod lbd05_disciplina.cod%TYPE)
IS
total number;
BEGIN
  count_leciona(p_nrousp,d_cod);
  select count(*) into total from lbd07_turma where coddisc = d_cod and nrouspprof = p_nrousp;
  
  if (total > 0) then
    RAISE_APPLICATION_ERROR(-20103,'Não é possível remover o cadastro do professor ' || p_nrousp || ' para a disciplina ' || d_cod || ' , pois há turmas desta disciplina atribuídas a ele');
  else    
    delete from lbd06_leciona where coddisc = d_cod and nrouspprof  = p_nrousp;
  end if;
  
END;