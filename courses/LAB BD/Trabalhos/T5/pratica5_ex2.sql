SET SERVEROUTPUT ON;

DECLARE 
  CURSOR cpart IS SELECT * FROM lbd10_participante P ORDER BY p.notafinal DESC;
  vpart cpart%ROWTYPE;
  vtipovinc lbd01_vinculo_usp%ROWTYPE;
  numpart NUMBER;
  offset NUMBER;
  diff NUMBER;
  chosed NUMBER;
  i NUMBER;
  
BEGIN

  SELECT COUNT(*) INTO numpart FROM lbd10_participante;
  dbms_output.put_line('TOTAL PARTICIPANTES: '||numpart);
  
  offset := ROUND(numpart * 0.8);
  dbms_output.put_line('OFFSET: '||offset);
  
  diff := numpart - offset;
  
  chosed := ROUND(dbms_random.value(1, diff));
    dbms_output.put_line('CHOSED : '||chosed);
  
  --i:=0;
  
  OPEN cpart;
  
  FOR i IN 1..chosed LOOP
  
    FETCH cpart INTO vpart;
    --dbms_output.put_line('i = ' || i);
  END LOOP;
  
  -- FALTA DAR O SELECT NA TABELA VINCULO USP PARA PEGAR OS CAMPOS NECESSARIOS
  
  SELECT * INTO vtipovinc FROM lbd01_vinculo_usp WHERE nrousp = vpart.nrousp;
 
  dbms_output.put_line('Ganhador: '|| vtipovinc.nome ||', Nro usp '|| vpart.nrousp ||', participante do curso ' ||vpart.codcurso || ', tipo vinculo ' || vtipovinc.tipovinc || ' e nascido(a) em ' || vtipovinc.datanascimento || ' - Nota final : ' || vpart.notafinal);
  
  CLOSE cpart;
  EXCEPTION
    WHEN OTHERS
        THEN dbms_output.put_line('Erro Nro: ' || SQLCODE || '.Mensagem: ' || SQLERRM);
  
  
END;    