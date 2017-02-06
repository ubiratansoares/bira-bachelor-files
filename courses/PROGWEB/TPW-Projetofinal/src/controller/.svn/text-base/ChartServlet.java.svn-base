package controller;

import java.awt.Color;
import java.io.IOException;
import java.io.OutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.List;
import java.util.Locale;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import model.Company;
import model.Quotation;

import org.hibernate.Session;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartRenderingInfo;
import org.jfree.chart.ChartUtilities;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.entity.StandardEntityCollection;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.general.DatasetUtilities;

import util.HibernateUtil;

public class ChartServlet extends HttpServlet {

    private static final long serialVersionUID = 1L;

    private Calendar cal = Calendar.getInstance();

    private String start_day = "1";
    private String start_month = "1";
    private String start_year = "1900";
	
    //Data atual caso outra nao seja fornecida
    private String end_day = String.valueOf(cal.get(Calendar.DAY_OF_MONTH));
    private String end_month = String.valueOf(cal.get(Calendar.MONTH)+1);
    private String end_year = String.valueOf(cal.get(Calendar.YEAR));
    
    private String stock = new String();
    private String bank = new String();
    private String chart = new String();

    
	private List<Double> values = new ArrayList<Double>();
	//private List<String> dates = new ArrayList<String>();
    
    public void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
    	values.clear();
    	//dates.clear();
    	
    	String aux = new String();
    	
    	aux = request.getParameter("sday");
    	if (aux != null){
    		start_day = aux;
    		aux = null;
    	}
    	
    	aux = request.getParameter("smonth");
    	if (aux != null){
    		start_month = aux;
    		aux = null;
    	}
    	
    	aux = request.getParameter("syear");
    	if (aux != null){
    		start_year = aux;
    		aux = null;
    	}
    	
    	aux = request.getParameter("eday");
    	if (aux != null){
    		end_day = aux;
    		aux = null;
    	}
    	
    	aux = request.getParameter("emonth");
    	if (aux != null){
    		end_month = aux;
    		aux = null;
    	}
    	
    	aux = request.getParameter("eyear");
    	if (aux != null){
    		end_year = aux;
    		aux = null;
    	}
    	
    	aux = request.getParameter("stock");
    	if (aux != null){
    		stock = aux;
    		aux = null;
    	}else {
    		aux = null;
    	}
    	
    	aux = request.getParameter("bank");
    	if (aux != null){
    		bank = aux;
    		aux = null;
    	}
    	
    	aux = request.getParameter("chart");
    	if (aux != null){
    		chart = aux;
    		aux = null;
    	}    	

    	/*
    	 * TODO Gerar graficos
    	 */
    	response.setContentType("image/png");
        OutputStream out = response.getOutputStream();
        
        if (chart.equalsIgnoreCase("stk")){
            buildStockChart(out, start_day, start_month, start_year, end_day, end_month, end_year, stock);
        }else if (chart.equalsIgnoreCase("invest")){
        	buildInvestChart(out, start_day, start_month, start_year, end_day, end_month, end_year, 
        			Integer.parseInt(request.getSession().getId()));
        }else{
        	buildStockChart(out, start_day, start_month, start_year, end_day, end_month, end_year, "GOOG");
        }
       
    }
    
    /*
     * 
     */
    private void buildInvestChart(OutputStream out, String start_day,
			String start_month, String start_year, String end_day,
			String end_month, String end_year, int user) {
		// TODO Auto-generated method stub
    	setInvestValues(start_day, start_month, start_year, end_day, end_month, end_year, user);
    	
    	double[][] data = new double[1][values.size()];
    	
    	for (int i=0; i< values.size(); i++){
    		data[0][i] = values.get(i);
    	}
    	
        final CategoryDataset dataset = DatasetUtilities.createCategoryDataset("Row ", "Column", data);

        final JFreeChart chart = ChartFactory.createLineChart("Area Chart", "",
                "Value", dataset, PlotOrientation.VERTICAL, true, true, false);

        final CategoryPlot plot = chart.getCategoryPlot();
        plot.setForegroundAlpha(0.5f);
        
        chart.setBackgroundPaint(new Color(185, 211, 238));

        try {
            final ChartRenderingInfo info = new ChartRenderingInfo(new StandardEntityCollection());        
            ChartUtilities.writeChartAsPNG(out, chart, 600, 400, info);

        } catch (Exception e) {
            e.printStackTrace();
        }
		
	}

	private void setInvestValues(String start_day, String start_month,
			String start_year, String end_day, String end_month,
			String end_year, Integer user) {
		// TODO Auto-generated method stub
		SimpleDateFormat f = new SimpleDateFormat("dd-mm-yyyy");
    	Date start_date = new GregorianCalendar(Integer.parseInt(start_year), 
    			Integer.parseInt(start_month), Integer.parseInt(start_day)).getTime();
    	
    	Date end_date = new GregorianCalendar(Integer.parseInt(end_year), 
    			Integer.parseInt(end_month), Integer.parseInt(end_day)).getTime();
    	
    	Session session = HibernateUtil.getSession();
		session.beginTransaction();
		
		Company c = (Company) session.createQuery("FROM Account WHERE id = " +
				" :user").setParameter("user", user).uniqueResult();
		
    	List list = session.createQuery("FROM Quotation q " +
    			" WHERE company = :company AND (q.tstamp > :start_date) AND " +
    			" (q.tstamp < :end_date) ORDER BY q.tstamp ASC")
				.setParameter("company", c).setParameter("start_date", start_date).setParameter("end_date", end_date).list();    	
    	    	
    	for (Object o: list){
    		Quotation q = (Quotation) o;
    		this.values.add(q.getValue());
    		//this.dates.add(f.format(q.getTimestamp()));
    	}    	
		
    	session.getTransaction().commit(); 
	}

	/*
     * TODO Usar datas limite para gerar grafico
     */
    private void buildStockChart(OutputStream out, String start_day, String start_month, String start_year, 
    		String end_day, String end_month, String end_year, String stock){
    	
    	setStockValues(start_day, start_month, start_year, end_day, end_month, end_year, stock);
    	
    	double[][] data = new double[1][values.size()];
    	
    	for (int i=0; i< values.size(); i++){
    		data[0][i] = values.get(i);
    	}
    	
        final CategoryDataset dataset = DatasetUtilities.createCategoryDataset("Row ", "Column", data);

        final JFreeChart chart = ChartFactory.createLineChart("Area Chart", "",
                "Value", dataset, PlotOrientation.VERTICAL, true, true, false);

        final CategoryPlot plot = chart.getCategoryPlot();
        plot.setForegroundAlpha(0.5f);
        
        chart.setBackgroundPaint(new Color(185, 211, 238));

        try {
            final ChartRenderingInfo info = new ChartRenderingInfo(new StandardEntityCollection());        
            ChartUtilities.writeChartAsPNG(out, chart, 600, 400, info);

        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    /**
     * Obtem dados do yahoo finance
     * 
     * @return
     * valores a serem plotados
     */
    private void setStockValues(String start_day, String start_month, String start_year, 
    		String end_day, String end_month, String end_year, String stock){   		

    	SimpleDateFormat f = new SimpleDateFormat("dd-mm-yyyy");
    	Date start_date = new GregorianCalendar(Integer.parseInt(start_year), 
    			Integer.parseInt(start_month), Integer.parseInt(start_day)).getTime();
    	
    	Date end_date = new GregorianCalendar(Integer.parseInt(end_year), 
    			Integer.parseInt(end_month), Integer.parseInt(end_day)).getTime();
    	
    	Session session = HibernateUtil.getSession();
		session.beginTransaction();
    	
		Company c = (Company) session.createQuery("FROM Company WHERE code = " +
				" :code").setParameter("code", stock).uniqueResult();
		
    	List list = session.createQuery("FROM Quotation q " +
    			" WHERE company = :company AND (q.tstamp > :start_date) AND " +
    			" (q.tstamp < :end_date) ORDER BY q.tstamp ASC")
				.setParameter("company", c).setParameter("start_date", start_date).setParameter("end_date", end_date).list();    	
    	    	
    	for (Object o: list){
    		Quotation q = (Quotation) o;
    		this.values.add(q.getValue());
    		//this.dates.add(f.format(q.getTimestamp()));
    	}    	
		
    	session.getTransaction().commit();   	
    }
}
