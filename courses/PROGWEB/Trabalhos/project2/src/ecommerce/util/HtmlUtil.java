package ecommerce.util;

import java.text.*;
import java.util.*;
import java.util.regex.*;

import javax.servlet.http.*;

public class HtmlUtil {
	
	private static final Pattern ENTITY_REF =
		Pattern.compile("&(\\w+|#\\d+|#x[0-9a-f]+);", 
		Pattern.CASE_INSENSITIVE);
	
	/**
	 * Transforma o objeto em texto com caracteres especiais escapados.
	 * {@code null} é transformado numa string vazia.
	 */
	public static String cdata(Object o) {
		if(o == null) return "";
		String text = o.toString(), result;
		
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
	
	/**
	 * Transforma a data em texto usando o formato especificado.
	 * {@code null} é transformado numa string vazia.
	 */
	public static String cdata(Date date, String format, Locale locale) {
		if(date == null) return "";
		DateFormat formatter = new SimpleDateFormat(format, locale);
		return HtmlUtil.cdata(formatter.format(date));
	}
	
	/**
	 * Transforma o número decimal em texto usando o formato especificado.
	 * {@code null} é transformado numa string vazia.
	 */
	public static String cdata(Double value, int minIntDigits, 
			int minFracDigits, int maxFracDigits, Locale locale) {
		if(value == null) return "";
		NumberFormat formatter = NumberFormat.getNumberInstance(locale);
		formatter.setMinimumIntegerDigits(minIntDigits);
		formatter.setMinimumFractionDigits(minFracDigits);
		formatter.setMaximumFractionDigits(maxFracDigits);
		return HtmlUtil.cdata(formatter.format(value));
	}
	
	/**
	 * Contrói a representação de um valor monetário.
	 * {@code null} é transformado em "indefinido".
	 */
	public static String currency(Double value, String prefix, Locale locale) {
		if(value == null) return "indefinido";
		return HtmlUtil.cdata(prefix) + HtmlUtil.cdata(value, 1, 2, 2, locale);
	}
	
	/**
	 * Contrói a representação de uma quantidade.
	 */
	public static String quantity(Integer value, String nullText, 
			String singleFormat, String multiFormat, Locale locale) {
		if(value == null) return HtmlUtil.cdata(nullText);
		return HtmlUtil.cdata(String.format(
				(value < 2)? singleFormat: multiFormat, value));
	}
	
	/**
	 * Contrói a representação de uma quantidade.
	 */
	public static String quantity(Long value, String nullText, 
			String singleFormat, String multiFormat, Locale locale) {
		if(value == null) return HtmlUtil.cdata(nullText);
		return HtmlUtil.cdata(String.format(
				(value < 2)? singleFormat: multiFormat, value));
	}
	
	/**
	 * Retorna um {@link java.util.ResourceBundle} de acordo com as 
	 * localizações dadas pelo cliente. Apenas as línguas dadas pelo 
	 * cliente (e apenas exatamente elas) são buscadas, se o cliente 
	 * especificou alguma língua. Caso nenhuma tenha sido especificada,
	 * a língua padrão do sistema é usada (conforme o contrato do 
	 * método {@link javax.servlet.ServletRequest#getLocales})). Caso
	 * nenhum bundle seja encontrado em qualquer dessas línguas, o
	 * bundle raiz é usado se disponível. Caso contrário, lança uma
	 * exceção. Essa estratégia de busca não interfere na cadeia de
	 * parent bundles.
	 * 
	 * @param basename
	 * O nome básico do recurso a ser buscado (parcialmente qualificado,
	 * sem língua e sem extensão). O nome será qualificado pelo pacote
	 * {@code "ecommerce.resources"}.
	 * 
	 * @param request
	 * A requisição do cliente que especifica as línguas a serem usadas.
	 * 
	 * @return 
	 * O bundle correspondente, conforme descrito acima.
	 */
	public static ResourceBundle getBundle(String basename, HttpServletRequest request) {
		return ResourceBundle.getBundle("ecommerce.resources." + basename, 
				request.getLocale(), new ResourceBundleControl(request));
	}
	
	/**
	 * Este controle procura apenas pelas localizações dadas pelo cliente,
	 * inibindo o fallback por região.
	 */
	private static class ResourceBundleControl extends ResourceBundle.Control {
		
		private Map<Locale, Locale> locales;
		
		public ResourceBundleControl(HttpServletRequest request) {
			Enumeration<Locale> e = request.getLocales();
			
			//eliminar duplicações
			List<Locale> list = new LinkedList<Locale>();
			while(e.hasMoreElements()) {
				Locale l = e.nextElement();
				if(!list.contains(l))
					list.add(l);
			}
			
			this.locales = new HashMap<Locale, Locale>();
			if(!list.isEmpty()) {
				Iterator<Locale> i = list.iterator();
				Locale src = i.next(); 
				Locale dest = null;
				while(i.hasNext()) {
					dest = i.next();
					this.locales.put(src, dest);
					src = dest; dest = null;
				}
				this.locales.put(src, dest);
			}
		}
		
		@Override
		public Locale getFallbackLocale(String basename, Locale locale) {
			return locales.get(locale);
		}
		
		@Override
		public List<Locale> getCandidateLocales(String basename, Locale locale) {
			List<Locale> list = new LinkedList<Locale>();
			if(!Locale.ROOT.equals(locale))
				list.add(locale);
			list.add(Locale.ROOT);
			return list;
		}
		
	}
	
}
