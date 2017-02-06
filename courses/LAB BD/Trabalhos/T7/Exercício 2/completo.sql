SET SERVEROUTPUT ON;
/* para as tabelas lbd11_grupo e lbd13_trabalho só precisamos de 3 entradas (tabela, codcurso, valor)
    para as tabelas lbd07_turma e lbd18_mensagem precisamos de 5 entradas 
    (tabela, ano, valor1, coddisc, valor2) ou (tabela, codcurso, valor1, nrousp, valor2), respectivamente */
CREATE OR REPLACE FUNCTION proximo_valor_geral 
(p_table VARCHAR2, att1 VARCHAR2 default null, entrada1 VARCHAR2 default null, 
  att2 VARCHAR2 default null, entrada2 VARCHAR2 default null)
RETURN NUMBER IS --tipo que ele retorna
  sql_text VARCHAR2(400);
  total number;
BEGIN 
  sql_text := 'Select count(*) from ' || p_table;
  
  if (att1 IS NOT NULL) AND (entrada1 IS NOT NULL) AND (att2 IS NOT NULL) AND (entrada2 IS NOT NULL) then
    sql_text := sql_text || ' Where ' || att1 || ' = ''' || entrada1 || '''' || ' and ' || att2 || ' = ''' || entrada2 || '''';--com as aspas podemos pegar tanto valores char qto numbers
  elsif  (att1 IS NOT NULL) AND (entrada1 IS NOT NULL) AND ( att2 IS NULL ) AND ( entrada2 IS NULL ) then
    sql_text := sql_text || ' Where ' || att1 || ' = ''' || entrada1 || '''';
  end if;
  
  dbms_output.put_line(sql_text);
  
  EXECUTE IMMEDIATE sql_text into total; --executando o comando

  return (total+1);
end;
/
Declare
Begin
--funçao nao pode ser chamada como procedimento repare:
  dbms_output.put_line(proximo_valor_geral('LBD07_Turma', 'ano', '2008','coddisc','SCC0261'));
  dbms_output.put_line(proximo_valor_geral('LBD11_Grupo', 'codcurso', '2'));
  dbms_output.put_line(proximo_valor_geral('LBD13_Trabalho', 'codcurso', '4'));
  dbms_output.put_line(proximo_valor_geral('LBD18_Mensagem','codcurso','19','nrousp','2'));
    
  EXCEPTION 
   WHEN OTHERS 
     THEN dbms_output.put_line('Erro nro:  ' || SQLCODE || '. Mensagem: ' || SQLERRM ); 
end;
/