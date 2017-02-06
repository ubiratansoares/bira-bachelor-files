CREATE OR REPLACE PACKAGE insercao_funcionario AS 

PROCEDURE insere_vinculo_usp( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y' 
);
/*
PROCEDURE insere_funcionario( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_nivel LBD04_FUNCIONARIO.NIVEL%TYPE, 
p_lotacao LBD04_FUNCIONARIO.LOTACAO%TYPE, 
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y'
); 
*/
PROCEDURE insere_funcionario( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE,  
p_lotacao LBD04_FUNCIONARIO.LOTACAO%TYPE, 
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y'
); 
/*
PROCEDURE insere_funcionario( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_nivel LBD04_FUNCIONARIO.NIVEL%TYPE, 
p_lotacao LBD04_FUNCIONARIO.LOTACAO%TYPE, 
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y'
); 
*/
END insercao_funcionario;
/

CREATE OR REPLACE PACKAGE BODY insercao_funcionario AS

PROCEDURE insere_vinculo_usp( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y' 
)
IS BEGIN
  
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
/*
--#########################  procedure normal #######################################
PROCEDURE insere_funcionario( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_nivel LBD04_FUNCIONARIO.NIVEL%TYPE, 
p_lotacao LBD04_FUNCIONARIO.LOTACAO%TYPE, 
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y'
)  
IS BEGIN

    insere_vinculo_usp(p_nrousp, p_tipovinc, p_nome, p_dataingresso, p_datanascimento, p_ativo);
  
    IF (p_tipovinc <> 2) then
      rollback;
      RAISE_APPLICATION_ERROR(-20105,'Vinculo incorreto');
    END IF;
    
  insert into lbd04_funcionario values (p_nrousp, p_nivel, p_lotacao);
  
  EXCEPTION --tratando o erros
    WHEN OTHERS THEN
      CASE
      --ordem correta para execução primeiro as fks dps a pk
        WHEN (SQLERRM LIKE '%FK_FUNCIONARIO_NROUSP%') THEN
           rollback;
           RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%PK_FUNCIONARIO%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%CK_NIVEL%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Nível inválido');
		WHEN (SQLERRM LIKE '%CK_LOTACAO%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Lotação inválida');
        ELSE
            RAISE_APPLICATION_ERROR(-20105,SQLERRM);
      END CASE;
END insere_funcionario;
--###########################################################################

*/

--####################### INICIO overload procedure sem NIVEL ###############################
PROCEDURE insere_funcionario( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE,  
p_lotacao LBD04_FUNCIONARIO.LOTACAO%TYPE, 
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y'
)  
IS 
idade NUMBER;
p_nivel VARCHAR2(8);
BEGIN

    insere_vinculo_usp(p_nrousp, p_tipovinc, p_nome, p_dataingresso, p_datanascimento, p_ativo);
  
    IF (p_tipovinc <> 2) then
      rollback;
      RAISE_APPLICATION_ERROR(-20105,'Vinculo incorreto');
    END IF;
    
  idade := (TRUNC((SYSDATE-p_datanascimento)/365)); --calcula a idade
  
  IF (idade > 40) THEN
    p_nivel := 'Superior';
  ELSIF (idade >=25) and (idade <= 39) THEN
    p_nivel := 'Tecnico';
  ELSIF (idade >= 10) and (idade <= 24) THEN
    p_nivel := 'Basico';
  END IF;
  
  insert into lbd04_funcionario values (p_nrousp, p_nivel, p_lotacao);
  
  EXCEPTION --tratando o erros
    WHEN OTHERS THEN
      CASE
      --ordem correta para execução primeiro as fks dps a pk
        WHEN (SQLERRM LIKE '%FK_FUNCIONARIO_NROUSP%') THEN
           rollback;
           RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%PK_FUNCIONARIO%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%CK_NIVEL%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Nível inválido');
		WHEN (SQLERRM LIKE '%CK_LOTACAO%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Lotação inválida');
        ELSE
            RAISE_APPLICATION_ERROR(-20105,SQLERRM);
      END CASE;
END insere_funcionario;
--##################### FIM overload procedure sem NIVEL ##############################
END insercao_funcionario;
/