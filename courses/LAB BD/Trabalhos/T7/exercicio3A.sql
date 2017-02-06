SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE count_leciona
( pnrousp lbd02_professor.nrousp%TYPE, dcod lbd05_disciplina.cod%TYPE)

IS
 sql_text VARCHAR2(400);
 
 total NUMBER;
BEGIN
  sql_text := 'Select count(*) from lbd06_leciona where nrouspprof = '|| pnrousp || ' and coddisc = ''' || dcod || '''';
 
  EXECUTE IMMEDIATE sql_text into total;
   
  IF ( total = 0 ) THEN
    RAISE_APPLICATION_ERROR(-20101,'O professor ' || pnrousp || ' não está habilitado a ministrar a disciplina ' || dcod );
  ELSE
    NULL;
  END IF;
END;
/
DECLARE
BEGIN
  
  --count_leciona(&pnrousp, '&dcod'); --espera a entrada digitada pelo usuário
  count_leciona(31, 'SCC0262');
  
 EXCEPTION
    when others then
    dbms_output.put_line(SQLERRM);
END;
/