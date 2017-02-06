SET SERVEROUTPUT ON;

DECLARE

  TYPE msg_tuple IS RECORD(
    num NUMBER,
    text VARCHAR2 (300)
  );

  TYPE msg_table IS TABLE OF msg_tuple;
  
  msgs msg_table := msg_table();
  
  i NUMBER;

  --exept_no_entry EXCEPTION;
  
  -- Declarando o tipo de dado tabela equivalente ao esquema da tabela lbd18
  TYPE sent_msgs IS TABLE OF lbd18_mensagem%ROWTYPE;
  
  -- Declarando a variavel para as mensagens geradas
  sents sent_msgs := sent_msgs();
  
BEGIN


  --dbms_output.put_line('OFFSET: '||offset);
  --diff := numpart - offset;
  --chosed := ROUND(dbms_random.value(1, diff));
 
  -- Construindo a tabela de mensagens que serao escolhidas ao acaso 
  FOR i IN 1..10 LOOP
  
      msgs.EXTEND;
      msgs(msgs.LAST).num := i;
      
      CASE i 
      
        WHEN(1) THEN msgs(msgs.LAST).text := 'A prova estava difícil.';
        WHEN(2) THEN msgs(msgs.LAST).text := 'Novos exercícios online.';
        WHEN(3) THEN msgs(msgs.LAST).text := 'Novo horário de atendimento';
        WHEN(4) THEN msgs(msgs.LAST).text := 'Alguém fez o exercício 4?';
        WHEN(5) THEN msgs(msgs.LAST).text := 'Hoje haverá aula de reposição.';
        WHEN(6) THEN msgs(msgs.LAST).text := 'A partir da semana que vem, aulas no lab 7.';
        WHEN(7) THEN msgs(msgs.LAST).text := 'O prazo para entrega das práticas é de 15 dias.';
        WHEN(8) THEN msgs(msgs.LAST).text := 'Quem ainda não retificou sua matrícula compareça à secretaria.';
        WHEN(9) THEN msgs(msgs.LAST).text := 'Não haverá aula durante a semana da computação';
        WHEN(10) THEN msgs(msgs.LAST).text := 'Atenção para as datas do projeto.';
        
        --ELSE RAISE OTHERS;
      END CASE;

  END LOOP;
  
  -- Construindo a tabela com todas as 1000 mensagens a serem enviadas, escolhidas
  
    --sents();
  
  --OPEN cpart;
  --CLOSE cpart;
  
  FOR i IN 1..msgs.LAST LOOP
  
    IF (msgs.EXISTS(i)) THEN dbms_output.put_line (msgs(i).num || '  ' || msgs(i).text);
    END IF;

  END LOOP;
  
  --sents();
  sents.EXTEND;
  sents(1).nromensagem := 1; dbms_output.put_line(sents(1).nromensagem);
  sents(1).codcurso := 'cod1'; dbms_output.put_line(sents(1).codcurso);
  sents(1).nrousp := 1; dbms_output.put_line(sents(1).nrousp);
  sents(1).titulo := 'titulo1'; dbms_output.put_line(sents(1).titulo);
  sents(1).texto := 'texto'; dbms_output.put_line(sents(1).texto);
  sents(1).datahorapostagem := SYSDATE; dbms_output.put_line(sents(1).datahorapostagem);
  
  EXCEPTION
    WHEN OTHERS
        THEN dbms_output.put_line('Erro Nro: ' || SQLCODE || '.Mensagem: ' || SQLERRM);
   
  
END;    