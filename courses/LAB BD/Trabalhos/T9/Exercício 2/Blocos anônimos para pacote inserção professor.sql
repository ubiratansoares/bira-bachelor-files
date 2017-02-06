SET SERVEROUTPUT ON;
/
Begin 

insercao_professor.insere_professor(199, 3, 'alberto',	'29/07/10',	'09/09/79',	'y', 'Mestrado', p_predio => 5 );
insercao_professor.insere_professor(193, 3, 'alberto',	'29/07/10',	'09/09/79',	'y', 'Mestrado', p_sala => 9);
insercao_professor.insere_professor(183, 3, 'alberto', 5,10, 	'29/07/10',	'09/09/79',	'y', 'Mestrado');
EXCEPTION
    when others then
    dbms_output.put_line('Erro: '||SQLERRM);

end;

/
Begin 
--exception numero de predio
insercao_professor.insere_professor(191, 3, 'alberto',	'29/07/10',	'09/09/79',	'y', 'Mestrado', p_predio => 6 );
EXCEPTION
    when others then
    dbms_output.put_line('Erro: '||SQLERRM);

end;

/
Begin 
--exception número de sala
insercao_professor.insere_professor(182, 3, 'alberto', 5, -10, 	'29/07/10',	'09/09/79',	'y', 'Mestrado');
EXCEPTION
    when others then
    dbms_output.put_line('Erro: '||SQLERRM);

end;
/
