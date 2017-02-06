package ecommerce.template;

import java.util.*;
import java.util.regex.*;

/**
 * Fragmento que escapa caracteres especiais da sintaxe HTML ou XML.
 */
public class TextFragment extends PageFragment {
	
	private static final Pattern ENTITY_REF =
		Pattern.compile("&(\\w+|#\\d+|#x[0-9a-f]+);", 
		Pattern.CASE_INSENSITIVE);
	
	/**
	 * Escapa caracteres especiais do HTML e do XML (são os mesmos).
	 * Os caracteres escapados são: aspas simples (&apos;), aspas
	 * duplas (&quot;), menor (&lt;), maior (&gt;) e "'e' comercial"
	 * (&amp;).
	 */
	protected String escape(String text) {
		if(text == null) return null;
		String result;
		
		//escapar ampersand (mas não escapar entity references)
		String[] pieces = text.split("&");
		if(pieces.length > 1) {
			StringBuilder sb = new StringBuilder();
			sb.append(pieces[0]);
			
			for(int i = 1; i < pieces.length; i++) {
				if(ENTITY_REF.matcher("&" + pieces[i]).find())
					sb.append("&" + pieces[i]);
				else
					sb.append("&amp;" + pieces[i]);
			}
			
			result = sb.toString();
		} else
			result = text;
		
		//escapar demais caracteres
		return result.replace("'", "&apos;").replace("\"", "&quot;")
				.replace("<", "&lt;").replace(">", "&gt;");
	}
	
	protected TextFragment(PageTemplate template) {
		super(template);
	}
	
	protected TextFragment(PageTemplate template, Locale locale) {
		super(template, locale);
	}
	
	protected TextFragment(PageTemplate template, 
			Locale locale, String bundle) {
		super(template, locale, bundle);
	}
	
	/* == testes == */
	
	public static void main(String[] args) throws Exception {
		java.io.File file = new java.io.File("test.html");
		java.net.URL url = file.toURI().toURL();
		TextFragment t = new TextFragment(new PageTemplate(url));
		System.out.println(t.escape(t.get()));
	}
	
}
