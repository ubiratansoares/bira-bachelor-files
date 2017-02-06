DECLARE

--cursor que pegara todos os dados de aluno, professor e funcionario
CURSOR meta IS
(((select v.nrousp, v.tipovinc, p.titulacao, p.predio,null, null,null,null from lbd01_vinculo_usp v join lbd02_professor p
on v.nrousp = p.nrousp) 

union

(select v.nrousp, v.tipovinc, null, null, al.idade, al.cidadenasc, null, null from lbd01_vinculo_usp v join lbd03_aluno al 
on v.nrousp = al.nrousp))

union

(select v.nrousp, v.tipovinc, null, null,null,null, f.nivel, f.lotacao from lbd01_vinculo_usp v join lbd04_funcionario f
on v.nrousp = f.nrousp));


TYPE metarecord IS RECORD(
    nrousp NUMBER,
    meta1 VARCHAR2(300),
    meta2 VARCHAR2(300)
	
);


TYPE todos_vinculados IS TABLE OF metarecord;

vinculados 	todos_vinculados := todos_vinculados();

BEGIN

    FOR cur IN meta LOOP

		vinculados.extend;
		

        IF (cur.tipovinc = 1) THEN --aluno
          vinculados.nrousp := cur.nrousp;
          vinculados.meta1 :=  TO_CHAR(cur.idade);
          vinculados.meta2 := TO_CHAR(cur.cidadenasc);
        
        ELSE IF (cur.tipovinc = 2) THEN --funcionario
                 vinculados.nrousp := cur.nrousp;
                 vinculados.meta1 :=  TO_CHAR(cur.nivel);
                 vinculados.meta2 := TO_CHAR(cur.lotacao);
                
        ELSE --professor
          vinculados.nrousp := cur.nrousp;
          vinculados.meta1 :=  TO_CHAR(cur.titulacao);
          vinculados.meta2 := TO_CHAR(cur.predio);
        END IF;
        END IF;
    END LOOP;

END;