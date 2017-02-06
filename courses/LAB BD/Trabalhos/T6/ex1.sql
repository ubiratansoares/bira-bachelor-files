SET SERVEROUTPUT ON;

DECLARE

CURSOR c1 IS SELECT nrousp, codcurso from LBD10_PARTICIPANTE;

TYPE recpart IS RECORD(
	nrousp lbd10_participante.nrousp%TYPE,
    codcurso lbd10_participante.codcurso%TYPE,
    nromensagens INTEGER
);                                   

TYPE participantes IS TABLE OF recpart;

TYPE auxtable IS VARRAY (10) OF VARCHAR(500);

valor_participante 	participantes := participantes();
auxiliar 		auxtable := auxtable();

rand_msg INTEGER;
rand_user INTEGER;
total_participantes INTEGER;
ultima_mensagem INTEGER;
i INTEGER;

BEGIN

SELECT count(*) INTO total_participantes FROM lbd10_participante;

auxiliar.extend(10);

auxiliar(1) := 'A prova estava dificil.';
auxiliar(2) := 'Novos exercicios online.';
auxiliar(3) := 'Novo horario de atendimento.';
auxiliar(4) := 'Alguem fez o exercicio 4?';
auxiliar(5) := 'Hoje havera aula de reposicao.';
auxiliar(6) := 'A partir da semana que vem, aulas no lab 7.';
auxiliar(7) := 'O prazo para entrega das praticas eh de 15 dias.';
auxiliar(8) := 'Quem ainda nao retificou sua matricula compareca na secretaria.';
auxiliar(9) := 'Nao havera aula durante a semana da computacao.';
auxiliar(10) := 'Atencao para as datas do projeto.';

i := 1;

FOR auxiliar IN c1 LOOP
	valor_participante.extend;
	valor_participante(i).nrousp := auxiliar.nrousp; 
	valor_participante(i).codcurso := auxiliar.codcurso;
	i := i+1;
END LOOP;

FOR i IN 1.. 1000 LOOP
    rand_msg := dbms_random.value(1, 10);
   	rand_user:= dbms_random.value(1, total_participantes);
   	
   	SELECT max(nromensagem) INTO ultima_mensagem FROM lbd18_mensagem WHERE codcurso = valor_participante(rand_user).codcurso AND nrousp = valor_participante(rand_user).nrousp;
   	
   	IF (ultima_mensagem is NULL) THEN
    	ultima_mensagem := 0;
    END IF;
    	
    ultima_mensagem := (ultima_mensagem + 1);
    	
    INSERT INTO lbd18_mensagem(nromensagem, codcurso, nrousp, titulo, texto, datahorapostagem) 
    	VALUES (ultima_mensagem, valor_participante(rand_user).codcurso, valor_participante(rand_user).nrousp, ('Mensagem ' || rand_msg), auxiliar(rand_msg), TO_DATE(SYSDATE, 'DD-MM-YYYY HH:MI:SS'));
				
END LOOP;  

EXCEPTION
    WHEN OTHERS
        THEN dbms_output.put_line('Erro Nro: ' || SQLCODE || '.Mensagem: ' || SQLERRM); 
END;

--rollback


