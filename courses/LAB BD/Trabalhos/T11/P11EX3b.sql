SELECT * FROM P11_LECIONA WHERE DEREF(CODDISCREF).Cod = 'SSC0124';
SELECT * FROM P11_LECIONA WHERE DEREF(NROUSPPROFREF).NroUSP = 31;

SET SERVEROUTPUT ON;
DECLARE 
   vinculado P11_VINCULOUSP_TYPE;
   aluno P11_ALUNO_TYPE;
   professor P11_PROF_TYPE;

BEGIN

  SELECT VALUE(v) INTO vinculado
         FROM P11_VINCULOUSP v 
         WHERE v.NroUSP = 6;
         
  dbms_output.put_line('Tempo Vinculo do NroUSP ' || vinculado.NroUSP || '  = '|| vinculado.tempo_vinculo());


  SELECT VALUE(p) INTO aluno
         FROM P11_ALUNO p 
         WHERE p.NroUSP = 36;

  aluno.corrige_idade();

  dbms_output.put_line('Idade Atualizada do Aluno ' || aluno.Nome || '= ' || aluno.Idade);

  SELECT VALUE(u) INTO professor
         FROM P11_PROFESSOR u 
         WHERE u.NroUSP = 31;  
  dbms_output.put_line('Localizacao do Professor ' || professor.Nome || '  (Predio - Sala) = '|| professor.localizacao());

END;