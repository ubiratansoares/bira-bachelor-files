SET SERVEROUTPUT ON;

DECLARE
  
    TYPE valor_recurso IS VARRAY(8) OF NUMBER; --array que guardará os pontos de cada recurso
  
    TYPE number_array IS TABLE OF NUMBER INDEX BY BINARY_INTEGER;
        
    TYPE record_type IS RECORD ( inner_array number_array);
    
    TYPE number_multi IS TABLE OF record_type INDEX BY BINARY_INTEGER;
    
    CURSOR logA IS SELECT l.nrousp, l.codcurso, l.nrorecurso, r.tipo 
    FROM lbd17_log_acesso l JOIN lbd16_recurso_eletronico r
    ON l.codcurso = r.codcurso and l.nrorecurso = r.nrorecurso;
    
    
    outer_array number_multi;
    
    v_outer NUMBER;
    v_inner NUMBER;
    vr valor_recurso;
    i number; j number;
    
BEGIN

  -- a posiçao se refere ao tipo do recurso e o conteudo a sua pontuação
  vr := valor_recurso(10, 15, 5, 30, 20, 10, 5, 10); 
  
  --inicializaçao da matriz
   FOR cur_log IN logA LOOP
        outer_array(cur_log.nrousp).inner_array(cur_log.codcurso) := 0;
  END LOOP;
  
  --inserção de dados na matriz
  FOR cur_log IN logA LOOP
        outer_array(cur_log.nrousp).inner_array(cur_log.codcurso) := outer_array(cur_log.nrousp).inner_array(cur_log.codcurso)+ vr(cur_log.tipo);
  END LOOP;
  
  
--impressão da saida
  v_outer := outer_array.FIRST;
  WHILE v_outer IS NOT NULL LOOP
  
    v_inner := outer_array(v_outer).inner_array.FIRST;
    WHILE v_inner IS NOT NULL LOOP
    
        dbms_output.put_line('Aluno ' || v_outer || ', curso ' || v_inner || ', pontuação: ' ||  outer_array(v_outer).inner_array(v_inner)  );
        v_inner := outer_array(v_outer).inner_array.NEXT(v_inner);
        
    END LOOP;
    
    v_outer := outer_array.NEXT(v_outer);
    
  END LOOP;

END;