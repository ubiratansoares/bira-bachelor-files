set serveroutput on;

DECLARE

  TYPE valor_recurso IS VARRAY(8) OF NUMBER; --array que guardará os pontos de cada recurso
--  TYPE tabela IS TABLE OF NUMBER;--tabela de inteiros
  TYPE X IS VARRAY(100) OF NUMBER;
  TYPE ntabela IS TABLE OF X;--tabela bi-dimensional
  
--  CURSOR sel IS SELECT nrousp, codcurso  FROM lbd10_participante order by nrousp;
--  CURSOR sel1 IS SELECT DISTINCT nrousp FROM lbd10_participante order by nrousp;
  CURSOR logA IS 
  SELECT l.nrousp, l.codcurso, l.nrorecurso, r.tipo 
    FROM lbd17_log_acesso l JOIN lbd16_recurso_eletronico r
    ON l.codcurso = r.codcurso and l.nrorecurso = r.nrorecurso;
  
  vr valor_recurso;
--  t_nusp tabela := tabela(); --construtor para tabela de numeros usp
--  t_codcurso tabela:= tabela();--construtor para tabela de codcursos
  bitable ntabela := ntabela(X(NULL), X(NULL));--tabela bidimensional
  
  i number;
  j number;
  
BEGIN
  -- a posiçao se refere ao tipo do recurso e o conteudo a sua pontuação
  vr := valor_recurso(10, 15, 5, 30, 20, 10, 5, 10); 

/*
--cursor para inserir dados na tabela t_codcurso, que será a coluna da tabela bi-dimensional  
  FOR cur_sel IN sel LOOP
    t_codcurso.EXTEND; --alocando espaço para cada tupla
    t_codcurso(t_codcurso.LAST) := cur_sel.codcurso;--pegando codcurso da tabela
  END LOOP;

--inserimos em uma tabela os numeros usp esses serão a linha da tabela bi-dimensional
  FOR cur_sel1 IN sel1 LOOP
    t_nusp.EXTEND; --alocando espaço para cada tupla
    t_nusp(t_nusp.LAST) := cur_sel1.nrousp;--pegando o numero usp da tabela
  END LOOP;
*/
  
--montagem da tabela bi-dimensional
  FOR cur_log IN logA LOOP
    bitable.EXTEND;
    bitable(cur_log.nrousp).EXTEND;
    dbms_output.put_line('nusp= ' || cur_log.nrousp || ' cod= ' || cur_log.codcurso || ' pontuacao: ' || vr(cur_log.tipo));
    IF ( bitable(cur_log.nrousp)(cur_log.codcurso)  IS NULL ) THEN --se for o primeiro, apenas recebe o valor
      bitable(cur_log.nrousp)(cur_log.codcurso) := vr(cur_log.tipo);
    ELSE --senao soma com o antrerior
      bitable(cur_log.nrousp)(cur_log.codcurso) := bitable(cur_log.nrousp)(cur_log.codcurso) + vr(cur_log.tipo);
      dbms_output.put_line('TOTAL= ' || bitable(cur_log.nrousp)(cur_log.codcurso) );
    END IF; 
  END LOOP;

/*
type tv1 is varray(10) of integer;
 type ntb2 is table of tv1; -- table of varray elements


--impressão da saida
FOR i IN 1..bitable.LAST LOOP
  FOR j IN 1..bitable.LAST LOOP
    dbms_output.put_line('Aluno ' || i || ', curso ' || j || ', pontuação: ' ||  bitable(i)(j)  );
  END LOOP;
END LOOP;
*/  

END;
/
