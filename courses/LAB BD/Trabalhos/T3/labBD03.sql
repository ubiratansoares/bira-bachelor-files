SELECT * FROM lbd01_vinculo_usp
WHERE 
      EXTRACT(YEAR FROM TO_DATE(datanascimento, 'DD-MM-RR')) BETWEEN 1985 AND 1993
      AND EXTRACT(MONTH FROM TO_DATE(datanascimento, 'DD-MM-RR')) Between  09 AND 12;

SELECT  DISTINCT V1.NROUSP, T1.NROTRABALHO, T1.CODCURSO
FROM lbd13_trabalho T1 JOIN lbd10_participante P1 ON T1.codcurso = P1.codcurso, 
	           lbd10_participante P1 JOIN lbd01_vinculo_usp V1 ON p1.nrousp = v1.nrousp
ORDER BY v1.nrousp;