create or replace
PACKAGE Item6 AS
  g_time_granularity VARCHAR2(50);
  g_resource_type VARCHAR2(3);
  PROCEDURE REPORT_ON_CURSO(p_curso VARCHAR2);
  --PROCEDURE REPORT_ON_DISCIPLINA(p_disciplina VARCHAR2);
  --PROCEDURE REPORT_ON_VINCULO(p_vinculo VARCHAR2);
END Item6;
/


create or replace
PACKAGE BODY Item6 AS
--record para reportar acesso diario
  TYPE data_on_report_diario IS RECORD(
    CODCURSO lbd17_log_acesso.codcurso%TYPE,
    CODTIPO lbd15_tipo_recurso.codtipo%TYPE,
    DIAACESSO VARCHAR2(100),
    MESACESSO VARCHAR2(100),
    ANOACESSO VARCHAR2(100),
    TOTAL NUMBER
  );
  TYPE t_data_on_report_diario IS TABLE OF data_on_report_diario;

--record para reportar acesso mensal
  TYPE data_on_report_mensal IS RECORD(
    CODCURSO lbd17_log_acesso.codcurso%TYPE,
    CODTIPO lbd15_tipo_recurso.codtipo%TYPE,
    MESACESSO VARCHAR2(100),
    ANOACESSO VARCHAR2(100),
    TOTAL NUMBER
  );
  TYPE t_data_on_report_mensal IS TABLE OF data_on_report_mensal;

--record para reportal acesso anual
  TYPE data_on_report_anual IS RECORD(
    CODCURSO lbd17_log_acesso.codcurso%TYPE,
    CODTIPO lbd15_tipo_recurso.codtipo%TYPE,
    ANOACESSO VARCHAR2(100),
    TOTAL NUMBER
  );
  TYPE t_data_on_report_anual IS TABLE OF data_on_report_anual;

--#################### INICIO DO PROCEDURA PARA CURSO ####################  
  PROCEDURE REPORT_ON_CURSO(p_curso VARCHAR2) AS
    v_data_on_report_diario t_data_on_report_diario;  
    v_data_on_report_mensal t_data_on_report_mensal;  
    v_data_on_report_anual t_data_on_report_anual;  
    v_selection VARCHAR2(1000);
  BEGIN
    v_selection := 'SELECT codcurso, tipo ';  

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
                                      (SELECT l1.codcurso, l2.tipo, l1.datahorarioacesso
                                      FROM lbd17_log_acesso L1, lbd16_recurso_eletronico L2
                                      WHERE L1.nrorecurso = l2.nrorecurso ';
    
	--seleção do curso desejado, ou todos os cursos
      IF(p_curso <> '*')THEN
        v_selection := v_selection || ' AND l1.codcurso = '||p_curso;
      END IF;
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
      dbms_output.put_line('CODCURSO    CODTIPO       ANO           MES       DIA       TOTAL');
      FOR i IN 1..v_data_on_report_diario.COUNT LOOP
        dbms_output.put_line(v_data_on_report_diario(i).CODCURSO||'            '||v_data_on_report_diario(i).CODTIPO||'           '||v_data_on_report_diario(i).ANOACESSO||'           '||v_data_on_report_diario(i).MESACESSO||'           '||v_data_on_report_diario(i).DIAACESSO||'           '||v_data_on_report_diario(i).TOTAL);
      END LOOP;            
    ELSIF(UPPER(g_time_granularity) = 'MENSAL') THEN
      EXECUTE IMMEDIATE v_selection BULK COLLECT INTO v_data_on_report_mensal;
      dbms_output.put_line('CODCURSO    CODTIPO       ANO           MES       TOTAL');
      FOR i IN 1..v_data_on_report_mensal.COUNT LOOP
        dbms_output.put_line(v_data_on_report_mensal(i).CODCURSO||'            '||v_data_on_report_mensal(i).CODTIPO||'           '||v_data_on_report_mensal(i).ANOACESSO||'           '||v_data_on_report_mensal(i).MESACESSO||'           '||v_data_on_report_mensal(i).TOTAL);
      END LOOP;      
    ELSE
      EXECUTE IMMEDIATE v_selection BULK COLLECT INTO v_data_on_report_anual;
      dbms_output.put_line('CODCURSO    CODTIPO       ANO     TOTAL');
      FOR i IN 1..v_data_on_report_anual.COUNT LOOP
        dbms_output.put_line(v_data_on_report_anual(i).CODCURSO||'            '||v_data_on_report_anual(i).CODTIPO||'           '||v_data_on_report_anual(i).ANOACESSO||'           '||v_data_on_report_anual(i).TOTAL);
      END LOOP;
    END IF;
        dbms_output.put_line(v_selection);
  END REPORT_ON_CURSO;
END Item6;
BEGIN
		g_time_granularity := 'ANUAL';
    g_resource_type := '*';  
END;