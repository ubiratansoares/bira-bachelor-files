SET SERVEROUTPUT ON;
/
BEGIN

insercao_funcionario.insere_funcionario(133,2,'thauner','Superior', 'EESC','15/02/2006','16/06/1987','y');
insercao_funcionario.insere_funcionario(80,2,'celia','IFSC','15/02/2006', '01/09/1980', 'y');
insercao_funcionario.insere_funcionario(135,2,'ubirajara','Basico','15/02/2006','16/06/1984','y');
EXCEPTION
    when others then
    dbms_output.put_line('Erro: '||SQLERRM);

END;
/
