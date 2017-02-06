CREATE OR REPLACE PACKAGE insercao_professor AS 
PROCEDURE insere_vinculo_usp( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y' 
);

PROCEDURE insere_professor( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_predio LBD02_PROFESSOR.PREDIO%TYPE, 
p_sala LBD02_PROFESSOR.SALA%TYPE, 
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y', 
p_titulacao LBD02_PROFESSOR.TITULACAO%TYPE DEFAULT 'Doutorado' 
); 

PROCEDURE insere_professor( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE, 
p_titulacao LBD02_PROFESSOR.TITULACAO%TYPE,
p_sala LBD02_PROFESSOR.SALA%TYPE
);

PROCEDURE insere_professor( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE, 
p_titulacao LBD02_PROFESSOR.TITULACAO%TYPE,
p_predio LBD02_PROFESSOR.PREDIO%TYPE 
);

END insercao_professor;
/
CREATE OR REPLACE PACKAGE BODY insercao_professor AS
PROCEDURE insere_vinculo_usp( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y' 
)
IS
BEGIN
  
  insert into lbd01_vinculo_usp values (p_nrousp, p_tipovinc, p_nome, p_dataingresso, p_datanascimento, p_ativo);
  
EXCEPTION --tratando o erros
  WHEN OTHERS THEN
    CASE
      -- SQLERRM = variavel global q o oracle preenche no exception, preenchida com o valor da exceção
      WHEN (SQLERRM LIKE '%CK_VINCULO_USP%') THEN
        RAISE_APPLICATION_ERROR(-20000,'Vinculo inválido');
      WHEN (SQLERRM LIKE '%CK_ATIVO%') THEN
        RAISE_APPLICATION_ERROR(-20000,'Caracter inválido');
      WHEN (SQLERRM LIKE '%PK_VINCULO_USP%') THEN
        RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
      ELSE
        RAISE_APPLICATION_ERROR(-20000,SQLERRM);
      END CASE;
      
END insere_vinculo_usp;

--#########################  procedure normal #######################################
PROCEDURE insere_professor( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_predio LBD02_PROFESSOR.PREDIO%TYPE, 
p_sala LBD02_PROFESSOR.SALA%TYPE,

p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y', 
p_titulacao LBD02_PROFESSOR.TITULACAO%TYPE DEFAULT 'Doutorado'
) 
IS
BEGIN

    insere_vinculo_usp(p_nrousp, p_tipovinc, p_nome, p_dataingresso, p_datanascimento, p_ativo);
  
    IF (p_tipovinc <> 3) then
      rollback;
      RAISE_APPLICATION_ERROR(-20105,'Vinculo incorreto');
    END IF;
    
  insert into lbd02_professor values (p_nrousp, p_titulacao, p_predio, p_sala);
  
  IF (p_sala <= 0) THEN
    rollback;
    RAISE_APPLICATION_ERROR(-20105,'Número de sala incorreto');
  ELSIF (p_predio <= 0) THEN
    rollback;
    RAISE_APPLICATION_ERROR(-20105,'Número de prédio incorreto');
  END IF;
  
  EXCEPTION --tratando o erros
    WHEN OTHERS THEN
      CASE
      --ordem correta para execução primeiro as fks dps a pk
        WHEN (SQLERRM LIKE '%FK_PROFESSOR_NROUSP%') THEN
           rollback;
           RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%PK_PROFESSOR%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%CK_TITULACAO%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Titulação inválida');
        ELSE
            RAISE_APPLICATION_ERROR(-20105,SQLERRM);
      END CASE;
END insere_professor;
--###########################################################################

--####################### overload procedure sem predio ###############################
PROCEDURE insere_professor( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE, 
p_titulacao LBD02_PROFESSOR.TITULACAO%TYPE,
p_sala LBD02_PROFESSOR.SALA%TYPE
) 
AS
p_predio NUMBER;
BEGIN
 
    insere_vinculo_usp(p_nrousp, p_tipovinc, p_nome, p_dataingresso, p_datanascimento, p_ativo);
  
    IF (p_tipovinc <> 3) then
      rollback;
      RAISE_APPLICATION_ERROR(-20105,'Vinculo incorreto');
    END IF;
    
    IF (p_sala <= 0) OR (p_sala > 100 ) THEN
      rollback;
      RAISE_APPLICATION_ERROR(-20105,'Número de sala incorreto');
    ELSIF (p_sala <= 70) AND (p_sala >= 61) THEN
        p_predio := 1;
    ELSIF (p_sala <= 60) AND (p_sala >= 51) THEN
        p_predio := 2;
    ELSIF (p_sala <= 50) AND (p_sala >= 25) THEN
        p_predio := 3;
    ELSIF (p_sala <= 24) AND (p_sala >= 1) THEN
        p_predio := 4;
    ELSIF (p_sala <= 71) AND (p_sala >= 100) THEN
        p_predio := 5;
    END IF;
    
  insert into lbd02_professor (nrousp, titulacao, predio, sala) values (p_nrousp, p_titulacao, p_predio, p_sala);
  EXCEPTION --tratando o erros
    WHEN OTHERS THEN
      CASE
      --ordem correta para execução primeiro as fks dps a pk
        WHEN (SQLERRM LIKE '%FK_PROFESSOR_NROUSP%') THEN
           rollback;
           RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%PK_PROFESSOR%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%CK_TITULACAO%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Titulação inválida');
        ELSE
            RAISE_APPLICATION_ERROR(-20105,SQLERRM);
        END CASE;
END insere_professor;
--##################### FIM overload procedure sem predio ##############################


--##################### INICIO overload procedure sem sala ##############################
PROCEDURE insere_professor( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE, 
p_titulacao LBD02_PROFESSOR.TITULACAO%TYPE,
p_predio LBD02_PROFESSOR.PREDIO%TYPE
) 
AS
p_sala NUMBER;
BEGIN
 
    insere_vinculo_usp(p_nrousp, p_tipovinc, p_nome, p_dataingresso, p_datanascimento, p_ativo);
  
    IF (p_tipovinc <> 3) then
      rollback;
      RAISE_APPLICATION_ERROR(-20105,'Vinculo incorreto');
    END IF;
  
  IF (p_predio <= 0) OR (p_predio > 5) THEN
    rollback;
    RAISE_APPLICATION_ERROR(-20105,'Número de prédio incorreto');
  ELSIF (p_predio = 5) THEN
    p_sala := dbms_random.value(71, 100);
  ELSIF (p_predio = 4) THEN
    p_sala := dbms_random.value(1, 24);
  ELSIF (p_predio = 3) THEN
    p_sala := dbms_random.value(25, 50);
  ELSIF (p_predio = 2) THEN
    p_sala := dbms_random.value(51, 60);
  ELSIF (p_predio = 1) THEN
    p_sala := dbms_random.value(61, 70);
  END IF;
    
  insert into lbd02_professor (nrousp, titulacao, sala, predio) values (p_nrousp, p_titulacao, p_sala, p_predio);
  
  EXCEPTION --tratando o erros
    WHEN OTHERS THEN
      CASE
      --ordem correta para execução primeiro as fks dps a pk
        WHEN (SQLERRM LIKE '%FK_PROFESSOR_NROUSP%') THEN
           rollback;
           RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%PK_PROFESSOR%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%CK_TITULACAO%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Titulação inválida');
        ELSE
            RAISE_APPLICATION_ERROR(-20105,SQLERRM);
        END CASE;
END insere_professor;
--################### procedure sem sala #######################

END insercao_professor;
/