CREATE OR REPLACE PACKAGE insercao_aluno AS 

PROCEDURE insere_vinculo_usp( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y' 
);

PROCEDURE insere_aluno( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_idade LBD03_ALUNO.IDADE%TYPE,
p_cidadenasc LBD03_ALUNO.CIDADENASC%TYPE,
p_estado LBD03_ALUNO.ESTADO%TYPE,
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y' 
);

PROCEDURE insere_aluno( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE,
p_cidadenasc LBD03_ALUNO.CIDADENASC%TYPE,
p_estado LBD03_ALUNO.ESTADO%TYPE,
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y',
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL
);


PROCEDURE insere_aluno(
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE,
p_idade LBD03_ALUNO.IDADE%TYPE,
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL,
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y',
p_estado LBD03_ALUNO.ESTADO%TYPE
);

END insercao_aluno;
/

CREATE OR REPLACE PACKAGE BODY insercao_aluno AS

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

--#########################  procedure normal #######################################
PROCEDURE insere_aluno( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE, 
p_idade LBD03_ALUNO.IDADE%TYPE,
p_cidadenasc LBD03_ALUNO.CIDADENASC%TYPE,
p_estado LBD03_ALUNO.ESTADO%TYPE,
                                              
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y' 
) 
IS BEGIN

  insere_vinculo_usp(p_nrousp, p_tipovinc, p_nome, p_dataingresso, p_datanascimento, p_ativo);
  
    IF (p_tipovinc <> 1) then
      rollback;
      RAISE_APPLICATION_ERROR(-20105,'Vinculo incorreto');
    END IF;
    
  insert into lbd03_aluno values (p_nrousp, p_idade, p_cidadenasc, p_estado);
  
  
  EXCEPTION --tratando o erros
    WHEN OTHERS THEN
      CASE
      --ordem correta para execução primeiro as fks dps a pk
        WHEN (SQLERRM LIKE '%FK_ALUNO_NROUSP%') THEN
           rollback;
           RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%PK_ALUNO%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%CK_IDADE%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Idade inválida');
        ELSE
            RAISE_APPLICATION_ERROR(-20105,SQLERRM);
      END CASE;
END insere_aluno;
--###################################################################################

--####################### INICIO overload procedure sem IDADE #######################
PROCEDURE insere_aluno( 
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE,
p_cidadenasc LBD03_ALUNO.CIDADENASC%TYPE,
p_estado LBD03_ALUNO.ESTADO%TYPE,
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y',
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL
)
IS 
p_idade number;
BEGIN
    
	insere_vinculo_usp(p_nrousp, p_tipovinc, p_nome, p_dataingresso, p_datanascimento, p_ativo);
  
    IF (p_tipovinc <> 1) then
      rollback;
      RAISE_APPLICATION_ERROR(-20105,'Vinculo incorreto');
    END IF;
  
  p_idade := (TRUNC((SYSDATE-p_datanascimento)/365)); --calcula a idade
  
  insert into lbd03_aluno values (p_nrousp, p_idade, p_cidadenasc, p_estado);
    
  EXCEPTION --tratando o erros
    WHEN OTHERS THEN
      CASE
      --ordem correta para execução primeiro as fks dps a pk
        WHEN (SQLERRM LIKE '%FK_ALUNO_NROUSP%') THEN
           rollback;
           RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%PK_ALUNO%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%CK_IDADE%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Idade inválida');
        ELSE
            RAISE_APPLICATION_ERROR(-20105,SQLERRM);
      END CASE;
END insere_aluno;
--##################### FIM overload procedure sem IDADE ############################

--###################### INICIO overload procedure sem CIDADENASC #####################
PROCEDURE insere_aluno(
p_nrousp LBD01_VINCULO_USP.NROUSP%TYPE, 
p_tipovinc LBD01_VINCULO_USP.TIPOVINC%TYPE, 
p_nome LBD01_VINCULO_USP.NOME%TYPE,
p_idade LBD03_ALUNO.IDADE%TYPE,
p_dataingresso LBD01_VINCULO_USP.DATAINGRESSO%TYPE DEFAULT SYSDATE, 
p_datanascimento LBD01_VINCULO_USP.DATANASCIMENTO%TYPE DEFAULT NULL,
p_ativo LBD01_VINCULO_USP.ATIVO%TYPE DEFAULT 'y',
p_estado LBD03_ALUNO.ESTADO%TYPE
)
IS 
p_cidadenasc VARCHAR2(20); 
BEGIN
  
  insere_vinculo_usp(p_nrousp, p_tipovinc, p_nome, p_dataingresso, p_datanascimento, p_ativo);
  
  IF (p_tipovinc <> 1) then
    rollback;
    RAISE_APPLICATION_ERROR(-20105,'Vinculo incorreto');
  END IF;
  
  IF (p_estado = 'AC') THEN
    p_cidadenasc := 'Rio Branco';
  ELSIF (p_estado = 'AL') THEN
    p_cidadenasc := 'Maceió';
  ELSIF (p_estado = 'AP') THEN
    p_cidadenasc := 'Macapá';
  ELSIF (p_estado = 'AM') THEN
    p_cidadenasc := 'Manaus';
  ELSIF (p_estado = 'BA') THEN
    p_cidadenasc := 'Salvador';
  ELSIF (p_estado = 'CE') THEN
    p_cidadenasc := 'Fortaleza';
  ELSIF (p_estado = 'DF') THEN
    p_cidadenasc := 'Brasília';
  ELSIF (p_estado = 'ES') THEN
    p_cidadenasc := 'Vitória';
  ELSIF (p_estado = 'GO') THEN
    p_cidadenasc := 'Goiânia';
  ELSIF (p_estado = 'MA') THEN
    p_cidadenasc := 'São Luis';
  ELSIF (p_estado = 'MT') THEN
    p_cidadenasc := 'Cuiabá';
  ELSIF (p_estado = 'MS') THEN
    p_cidadenasc := 'Campo Grande';
  ELSIF (p_estado = 'MG') THEN
    p_cidadenasc := 'Belo Horizonte';
  ELSIF (p_estado = 'PA') THEN
    p_cidadenasc := 'Belém';
  ELSIF (p_estado = 'PB') THEN
    p_cidadenasc := 'João Pessoa';
  ELSIF (p_estado = 'PR') THEN
    p_cidadenasc := 'Curitiba';
  ELSIF (p_estado = 'PE') THEN
    p_cidadenasc := 'Recife';
  ELSIF (p_estado = 'PI') THEN
    p_cidadenasc := 'Teresina';
  ELSIF (p_estado = 'RJ') THEN
    p_cidadenasc := 'Rio de Janeiro';
  ELSIF (p_estado = 'RN') THEN
    p_cidadenasc := 'Natal';
  ELSIF (p_estado = 'RS') THEN
    p_cidadenasc := 'Porto Alegre';
  ELSIF (p_estado = 'RO') THEN
    p_cidadenasc := 'Porto Velho';
  ELSIF (p_estado = 'RR') THEN
    p_cidadenasc := 'Boa Vista';
  ELSIF (p_estado = 'SC') THEN
    p_cidadenasc := 'Florianopolis';
  ELSIF (p_estado = 'SP') THEN
    p_cidadenasc := 'São Paulo';
  ELSIF (p_estado = 'SE') THEN
    p_cidadenasc := 'Aracaju';
  ELSIF (p_estado = 'TO') THEN
    p_cidadenasc := 'Palmas';
  ELSE
    rollback;
    RAISE_APPLICATION_ERROR(-20105,'Estado incorreto');
  END IF;
  insert into lbd03_aluno values (p_nrousp, p_idade, p_cidadenasc, p_estado);
    
  EXCEPTION --tratando o erros
    WHEN OTHERS THEN
      CASE
      --ordem correta para execução primeiro as fks dps a pk
        WHEN (SQLERRM LIKE '%FK_ALUNO_NROUSP%') THEN
           rollback;
           RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%PK_ALUNO%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Usuário já inserido');
        WHEN (SQLERRM LIKE '%CK_IDADE%') THEN
            rollback;
            RAISE_APPLICATION_ERROR(-20000,'Idade inválida');
        ELSE
            RAISE_APPLICATION_ERROR(-20105,SQLERRM);
      END CASE;
END insere_aluno;
--###################### FIM overload procedure sem CIDADENASC #####################

END insercao_aluno;
/
