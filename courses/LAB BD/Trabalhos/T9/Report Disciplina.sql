create or replace
PACKAGE BODY AUDITORIA AS
--O METADADO IRÁ RECEBER OS VALORES DE CODCURSO, CODDISC OU NROUSP
--record para reportar acesso diario
  TYPE data_on_report_diario IS RECORD(
    METADADO VARCHAR2(7),
    CODTIPO lbd15_tipo_recurso.codtipo%TYPE,
    DIAACESSO VARCHAR2(100),
    MESACESSO VARCHAR2(100),
    ANOACESSO VARCHAR2(100),
    TOTAL NUMBER
  );
  TYPE t_data_on_report_diario IS TABLE OF data_on_report_diario;

--record para reportar acesso mensal
  TYPE data_on_report_mensal IS RECORD(
    METADADO VARCHAR2(7),
    CODTIPO lbd15_tipo_recurso.codtipo%TYPE,
    MESACESSO VARCHAR2(100),
    ANOACESSO VARCHAR2(100),
    TOTAL NUMBER
  );
  TYPE t_data_on_report_mensal IS TABLE OF data_on_report_mensal;

--record para reportal acesso anual
  TYPE data_on_report_anual IS RECORD(
    METADADO VARCHAR2(7),
    CODTIPO lbd15_tipo_recurso.codtipo%TYPE,
    ANOACESSO VARCHAR2(100),
    TOTAL NUMBER
  );
  TYPE t_data_on_report_anual IS TABLE OF data_on_report_anual;

  
--#################### INICIO DO PROCEDURE PARA DISCIPLINA  ####################  
  PROCEDURE REPORT_ON_DISCIPLINA(p_disciplina lbd09_curso_eletronico.coddisc%TYPE) AS
    v_data_on_report_diario t_data_on_report_diario;  
    v_data_on_report_mensal t_data_on_report_mensal;  
    v_data_on_report_anual t_data_on_report_anual;  
    v_selection VARCHAR2(1000);
  BEGIN
  
  dbms_output.put_line('AQUI !!!!!!!!!!!!!!'||p_disciplina);
    v_selection := 'SELECT coddisc, tipo ';  

--** Se pedir relatório diário **

    IF(UPPER(g_time_granularity) = 'DIARIO') THEN
      v_selection := v_selection ||
             ', EXTRACT(DAY FROM datahorarioacesso),
             EXTRACT(MONTH FROM datahorarioacesso),
             EXTRACT(YEAR FROM datahorarioacesso)';

--** Se pedir relatório mensal **
			 
    ELSIF(UPPER(g_time_granularity) = 'MENSAL') THEN
      v_selection := v_selection ||
             ', EXTRACT(MONTH FROM datahorarioacesso),
             EXTRACT(YEAR FROM datahorarioacesso)';
			 
--** Se pedir relatório anual **			 
			 
    ELSE
      v_selection := v_selection ||
             ', EXTRACT(YEAR FROM datahorarioacesso)';
    END IF;
    v_selection := v_selection || ',
                                    COUNT(*) "TOTAL" ';
    
    v_selection := v_selection || 'FROM
                                      (SELECT l3.coddisc, l2.tipo, l1.datahorarioacesso
									  FROM lbd09_curso_eletronico l3 join lbd16_recurso_eletronico L2
									  ON l3.codcurso = l2.codcurso';
	
	--seleção da disciplina desejada, ou todas as disciplinas
      IF(p_disciplina <> '*')THEN
        v_selection := v_selection || ' AND l3.coddisc = '||p_disciplina;
      END IF;

	v_selection := v_selection || ' JOIN lbd17_log_acesso L1
                                    ON L1.nrorecurso = l2.nrorecurso ';
    	
	--seleção do recurso desejado, ou todos os recursos
      IF(g_resource_type <> '*')THEN
        v_selection := v_selection || ' AND l2.tipo = '||g_resource_type;
      END IF;
      dbms_output.put_line(v_selection);
      v_selection := v_selection || ')';
    v_selection := v_selection || ' 
    GROUP BY coddisc, tipo, ';    


    IF(UPPER(g_time_granularity) = 'DIARIO') THEN
      v_selection := v_selection ||
             'EXTRACT(DAY FROM datahorarioacesso),
             EXTRACT(MONTH FROM datahorarioacesso),
             EXTRACT(YEAR FROM datahorarioacesso)';             
    ELSIF(UPPER(g_time_granularity) = 'MENSAL') THEN
      v_selection := v_selection ||
             'EXTRACT(MONTH FROM datahorarioacesso),
             EXTRACT(YEAR FROM datahorarioacesso)';
    ELSE
      v_selection := v_selection ||   
             'EXTRACT(YEAR FROM datahorarioacesso)';
    END IF;
    v_selection := v_selection || ' ORDER BY TOTAL DESC';


    IF(UPPER(g_time_granularity) = 'DIARIO') THEN
      EXECUTE IMMEDIATE v_selection BULK COLLECT INTO v_data_on_report_diario;
      dbms_output.put_line('CODDISC    CODTIPO       ANO           MES       DIA       TOTAL');
      FOR i IN 1..v_data_on_report_diario.COUNT LOOP
        dbms_output.put_line(v_data_on_report_diario(i).METADADO||'            '||v_data_on_report_diario(i).CODTIPO||'           '||v_data_on_report_diario(i).ANOACESSO||'           '||v_data_on_report_diario(i).MESACESSO||'           '||v_data_on_report_diario(i).DIAACESSO||'           '||v_data_on_report_diario(i).TOTAL);
      END LOOP;            
    ELSIF(UPPER(g_time_granularity) = 'MENSAL') THEN
      EXECUTE IMMEDIATE v_selection BULK COLLECT INTO v_data_on_report_mensal;
      dbms_output.put_line('CODDISC    CODTIPO       ANO           MES       TOTAL');
      FOR i IN 1..v_data_on_report_mensal.COUNT LOOP
        dbms_output.put_line(v_data_on_report_mensal(i).METADADO||'            '||v_data_on_report_mensal(i).CODTIPO||'           '||v_data_on_report_mensal(i).ANOACESSO||'           '||v_data_on_report_mensal(i).MESACESSO||'           '||v_data_on_report_mensal(i).TOTAL);
      END LOOP;      
    ELSE
      EXECUTE IMMEDIATE v_selection BULK COLLECT INTO v_data_on_report_anual;
      dbms_output.put_line('CODDISC    CODTIPO       ANO     TOTAL');
      FOR i IN 1..v_data_on_report_anual.COUNT LOOP
        dbms_output.put_line(v_data_on_report_anual(i).METADADO||'            '||v_data_on_report_anual(i).CODTIPO||'           '||v_data_on_report_anual(i).ANOACESSO||'           '||v_data_on_report_anual(i).TOTAL);
      END LOOP;
    END IF;
        dbms_output.put_line(v_selection);
  END REPORT_ON_DISCIPLINA;
 
  
  
END AUDITORIA;