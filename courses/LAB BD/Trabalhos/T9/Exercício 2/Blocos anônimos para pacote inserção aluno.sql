BEGIN

--insercao_aluno.insere_aluno(133,1,'thiago',23,'São Paulo','SP','15/02/2006','16/06/1987','y');--teste OK
insercao_aluno.insere_aluno(134,1,'camila','São Paulo','SP','15/02/2006','y','01/09/1990');
--insercao_aluno.insere_aluno(135,1,'ubiratan',26,'15/02/2006','16/06/1984','y', p_estado => 'SP');--teste OK
EXCEPTION
    when others then
    dbms_output.put_line('Erro: '||SQLERRM);

END;
/
BEGIN

insercao_aluno.insere_aluno(139,1,'ulisses',23,'15/02/2006','16/06/1984','y', p_estado => 'AS');
EXCEPTION
    when others then
    dbms_output.put_line('Erro: '||SQLERRM);

END;
/
rollback;