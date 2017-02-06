SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE insere_professor( 
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
            RAISE_APPLICATION_ERROR(-20000,'Titulação Inválida');
        ELSE
            RAISE_APPLICATION_ERROR(-20105,SQLERRM);
        END CASE;
END;
/