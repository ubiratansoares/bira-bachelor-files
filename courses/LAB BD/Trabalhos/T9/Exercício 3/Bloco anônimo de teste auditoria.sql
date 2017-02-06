set serveroutput on;
/
DECLARE
g_time_granularity VARCHAR2(20);
    g_resource_type VARCHAR2(20);
BEGIN
		g_time_granularity := 'ANUAL';
    g_resource_type := '*';
    auditoria.report_on_curso(19,'ANUAL','*');
	auditoria.report_on_disciplina('SCC0722');
	auditoria.report_on_nrousp(1);	
END;

CODCURSO    CODTIPO       ANO     TOTAL
19            4           2009           4
19            7           2008           4

create global temporary table RELATORIO( METADADO VARCHAR2(7), 
    CODTIPO VARCHAR2(3),
    DIAACESSO VARCHAR2(100), 
    MESACESSO VARCHAR2(100), 
    ANOACESSO VARCHAR2(100), 
    TOTAL NUMBER);
	
	
	try
    {
        String lista = new String();
                
        while (consulta.resultset.next())
        {
             lista = lista +("  "+consulta.resultset.getString("codcurso"));
        }
                
		String mensagem = "Entre com um Curso da lista: \n" +lista;
        String curso = (String)JOptionPane.showInputDialog(mensagem);
				
    } catch(SQLException sqlex) {
        JOptionPane.showMessageDialog(null,"Não foi possível executar o comado"+sqlex);
    }