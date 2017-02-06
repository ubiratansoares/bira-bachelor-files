package util;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.StringTokenizer;

import model.Company;
import model.Quotation;

public class YFinanceBot {
	
	//FIXME *trunk* Usar isso para inserir dados do csv no banco
    public static void populateStocks(String start_day, String start_month, String start_year, 
    		String end_day, String end_month, String end_year, Company company) {    	
    	
        try {
            String strUrl = "http://ichart.finance.yahoo.com/table.csv?s="+company.getCode()+"&d="+end_month+
            		"&e="+end_day+"&f="+end_year+"&g=d&a="+start_month+"&b="+start_day+"&c="+start_year+"&ignore=.csv";
            URL url = new URL(strUrl);
            BufferedReader in = new BufferedReader(new InputStreamReader(url.openStream()));
            DateFormat df = new SimpleDateFormat("y-M-d");

            String inputLine;
            in.readLine();
            while ((inputLine = in.readLine()) != null) {
                StringTokenizer st = new StringTokenizer(inputLine, ",");

                Date date       = df.parse( st.nextToken() );
                double open     = Double.parseDouble( st.nextToken() );
                double high     = Double.parseDouble( st.nextToken() );
                double low      = Double.parseDouble( st.nextToken() );
                double close    = Double.parseDouble( st.nextToken() );
                double volume   = Double.parseDouble( st.nextToken() );
                double adjClose = Double.parseDouble( st.nextToken() );

                //TODO adicionar ao banco
                Quotation quo = new Quotation();
                quo.setCompany(company);
                quo.setTimestamp(date);
                quo.setValue(close);
                HibernateUtil.getSession().save(quo);
                
            }
            in.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}
