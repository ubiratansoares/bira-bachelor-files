package org.populate;

import java.net.*;
import java.io.*;

public class GetStock {

	public static void main(String args[]) throws Exception {

		if (args.length != 7) {
			System.out.println("usage: java org.populate.GetStock quote endMonth endDay endYear startMonth startDay startYear");
		}

		String quote = args[0];
		String em = args[1];
		String ed = args[2];
		String ey = args[3];
		String sm = args[4];
		String sd = args[5];
		String sy = args[6];

		String url = "http://ichart.finance.yahoo.com/table.csv?s="+quote+"&d="+em+"&e="+ed+"&f="+ey+"&g=d&a="+sm+"&b="+sd+"&c="+sy+"&ignore=.csv";
		URL site = new URL(url);
		URLConnection yahoo = site.openConnection();

		BufferedReader in = new BufferedReader(new InputStreamReader(yahoo.getInputStream()));
		String inputLine;

		while ((inputLine = in.readLine()) != null)  {
			int i1 = inputLine.indexOf(",");
			String date = inputLine.substring(0, i1);

			inputLine = inputLine.substring(i1+1);
			i1 = inputLine.indexOf(",");
			String open = inputLine.substring(0, i1);

			inputLine = inputLine.substring(i1+1);
			i1 = inputLine.indexOf(",");
			String high = inputLine.substring(0, i1);

			inputLine = inputLine.substring(i1+1);
			i1 = inputLine.indexOf(",");
			String low = inputLine.substring(0, i1);

			inputLine = inputLine.substring(i1+1);
			i1 = inputLine.indexOf(",");
			String close = inputLine.substring(0, i1);

			inputLine = inputLine.substring(i1+1);
			i1 = inputLine.indexOf(",");
			String volume = inputLine.substring(0, i1);

			inputLine = inputLine.substring(i1+1);
			String adjclose = inputLine;
		}

		in.close();
	}
}

