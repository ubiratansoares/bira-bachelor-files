SET SERVEROUTPUT ON;

DECLARE 
  ano NUMBER := 2009; 
  disc VARCHAR2(7) := 'SCC0261'; 
  prof NUMBER := 31; 
  nroturma NUMBER; 
BEGIN 
  --neste exemplo, o numero da turma será = 1
  nroturma := proximo_valor_geral('LBD07_TURMA','ANO',ano,'CODDISC',disc); 
  --aqui insere-se a a (1,2009,SCC0261,31);
  safe_insert_turma(nroturma,ano,disc,prof);   
 
  --aqui faz-se o update do professor da turma anterior 
  prof := 40; 
  safe_update_turma(nroturma,ano,disc,prof); 
 
  prof := 40; 
  safe_update_turma(nroturma,ano,disc,prof); 
    
  --aqui não conseguimos deletar o professor já que ele leciona aulas
  disc := 'SCC0261'; 
  prof := 40; 
  safe_delete_leciona(prof, disc); 
 
EXCEPTION 
   WHEN OTHERS 
     THEN dbms_output.put_line('Erro nro:  ' || SQLCODE || '. Mensagem: ' || SQLERRM ); 

END;