CREATE OR REPLACE PROCEDURE insere_vinculo_usp( 
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
      WHEN (SQLERRM LIKE '%PK_VINCULO_USP%') THEN
        RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
      END CASE;
END;