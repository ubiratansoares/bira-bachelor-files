--#################### INICIO DO PROCEDURA PARA NROUSP ####################  
  PROCEDURE REPORT_ON_NROUSP(p_nrousp NUMBER) AS
    v_data_on_report_diario t_data_on_report_diario;  
    v_data_on_report_mensal t_data_on_report_mensal;  
    v_data_on_report_anual t_data_on_report_anual;  
    v_selection VARCHAR2(1000);
  BEGIN
    v_selection := 'SELECT nrousp, tipo ';  

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
                                      (SELECT l1.nrousp, l2.tipo, l1.datahorarioacesso
                                      FROM lbd17_log_acesso L1, lbd16_recurso_eletronico L2
                                      WHERE L1.nrorecurso = l2.nrorecurso ';
    
	--seleção do curso desejado, ou todos os cursos
      IF(p_curso <> '*')THEN
        v_selection := v_selection || ' AND l1.nrousp = TO_CHAR('||p_curso;
      END IF;
	  
	v_selection := v_selection || ')';
	
	--seleção do recurso desejado, ou todos os recursos
      IF(g_resource_type <> '*')THEN
        v_selection := v_selection || ' AND l2.tipo = '||g_resource_type;
      END IF;
      v_selection := v_selection || ')';
    v_selection := v_selection || ' 
    GROUP BY codcurso, tipo, ';    


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
      dbms_output.put_line('NROUSP    CODTIPO       ANO           MES       DIA       TOTAL');
      FOR i IN 1..v_data_on_report_diario.COUNT LOOP
        dbms_output.put_line(v_data_on_report_diario(i).METADADO||'            '||v_data_on_report_diario(i).CODTIPO||'           '||v_data_on_report_diario(i).ANOACESSO||'           '||v_data_on_report_diario(i).MESACESSO||'           '||v_data_on_report_diario(i).DIAACESSO||'           '||v_data_on_report_diario(i).TOTAL);
      END LOOP;            
    ELSIF(UPPER(g_time_granularity) = 'MENSAL') THEN
      EXECUTE IMMEDIATE v_selection BULK COLLECT INTO v_data_on_report_mensal;
      dbms_output.put_line('NROUSP    CODTIPO       ANO           MES       TOTAL');
      FOR i IN 1..v_data_on_report_mensal.COUNT LOOP
        dbms_output.put_line(v_data_on_report_mensal(i).METADADO||'            '||v_data_on_report_mensal(i).CODTIPO||'           '||v_data_on_report_mensal(i).ANOACESSO||'           '||v_data_on_report_mensal(i).MESACESSO||'           '||v_data_on_report_mensal(i).TOTAL);
      END LOOP;      
    ELSE
      EXECUTE IMMEDIATE v_selection BULK COLLECT INTO v_data_on_report_anual;
      dbms_output.put_line('NROUSP    CODTIPO       ANO     TOTAL');
      FOR i IN 1..v_data_on_report_anual.COUNT LOOP
        dbms_output.put_line(v_data_on_report_anual(i).METADADO||'            '||v_data_on_report_anual(i).CODTIPO||'           '||v_data_on_report_anual(i).ANOACESSO||'           '||v_data_on_report_anual(i).TOTAL);
      END LOOP;
    END IF;
        dbms_output.put_line(v_selection);
  END REPORT_ON_NROUSP;
--##################### FIM DO PROCEDURE  PARA NROUSP #####################