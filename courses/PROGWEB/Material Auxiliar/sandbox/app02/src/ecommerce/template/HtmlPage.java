package ecommerce.template;

import java.util.*;

/**
 * Página em HTML. É possível configurar DOCTYPE, TITLE, HEAD e BODY.
 */
public class HtmlPage extends PageFragment {
	
	/* == setters == */
	
	/**
	 * Define o DOCTYPE do documento HTML. {@link Type#NONE} omite
	 * o DOCTYPE.
	 * 
	 * @param dtd
	 * Um DTD a ser gerado no documento: strict, transitional ou 
	 * frameset, ou nenhum (para não gerar a declaração de DOCTYPE);
	 */
	public void setDoctype(Type dtd) {
		this.dtd = (dtd != null)? dtd: Type.NONE;
	}
	
	/**
	 * Define o conteúdo do elemento TITLE.
	 * 
	 * @param title
	 * O título da página.
	 */
	public void setTitle(String title) {
		this.title = title;
	}
	
	/**
	 * Define o conteúdo do elemento HEAD. Todos os cabeçalhos são
	 * herdados exceto "Content-Type" e "Content-Language".
	 * 
	 * @param content
	 * Um fragmento com o conteúdo do cabeçalho HTML.
	 */
	public void setHeadContent(PageFragment content) {
		this.head = content;
		String ctype = this.getHeaders().get("Content-Type");
		String clang = this.getHeaders().get("Content-Language");
		this.clearHeaders();
		if(this.body != null) this.setHeaders(this.body.getHeaders());
		if(this.head != null) this.setHeaders(this.head.getHeaders());
		this.setHeader("Content-Type", ctype);
		this.setHeader("Content-Language", clang);
	}
	
	/**
	 * Define o conteúdo do elemento BODY. Todos os cabeçalhos são
	 * herdados exceto "Content-Type" e "Content-Language".
	 * 
	 * @param content
	 * Um fragmento com o conteúdo do corpo do documento.
	 */
	public void setBodyContent(PageFragment content) {
		this.body = content;
		String ctype = this.getHeaders().get("Content-Type");
		String clang = this.getHeaders().get("Content-Language");
		this.clearHeaders();
		if(this.head != null) this.setHeaders(this.head.getHeaders());
		if(this.body != null) this.setHeaders(this.body.getHeaders());
		this.setHeader("Content-Type", ctype);
		this.setHeader("Content-Language", clang);
	}
	
	/**
	 * Define a codificação do documento. Por padrão a codificação
	 * é UTF-8. Essa codificação substituirá o valor do campo 
	 * "Content-Type" para "text/html; charset={valor}". Caso a
	 * codificação especificada seja nula ou vazia, ela será omitida.
	 * Note que este método não coloca um elemento META no documento,
	 * mas inclui o campo na sugestão de cabeçalhos HTTP.
	 * 
	 * @param charset
	 * O nome da codificação a ser enviado no cabeçalho HTTP (pelo
	 * campo "Content-Type".
	 */
	public void setCharset(String charset) {
		if(charset != null && !charset.isEmpty())
			this.setHeader("Content-Type", "text/html; charset=" + charset);
		else
			this.setHeader("Content-Type", "text/html");
	}
	
	/* == construção da página == */
	
	/**
	 * Constrói a página HTML com DOCTYPE, título e conteúdo.
	 */
	protected void set() {
		switch(this.dtd) {
			case STRICT:		this.setOption("doctype", "strict");		break;
			case TRANSITIONAL:	this.setOption("doctype", "transitional");	break;
			case FRAMESET:		this.setOption("doctype", "frameset");		break;
			default:			this.setOption("doctype", null);
		}
		
		this.head.setBase(this.getBase());
		this.body.setBase(this.getBase());
		this.setValue("page_title", this.title);
		this.setValue("head_content", this.head);
		this.setValue("body_content", this.body);
	}
	
	/**
	 * Constrói a página HTML com DOCTYPE, título e conteúdo.
	 */
	protected void set(String group) {
		switch(this.dtd) {
			case STRICT:		this.setOption("doctype", "strict");		break;
			case TRANSITIONAL:	this.setOption("doctype", "transitional");	break;
			case FRAMESET:		this.setOption("doctype", "frameset");		break;
			default:			this.setOption("doctype", null);
		}
		
		this.head.setBase(this.getBase());
		this.body.setBase(this.getBase());
		this.setValue("page_title", this.title);
		this.setValue("head_content", this.head, group);
		this.setValue("body_content", this.body, group);
	}
	
	/**
	 * Constrói a página HTML com DOCTYPE, título e conteúdo.
	 */
	protected void set(Set<String> groups) {
		switch(this.dtd) {
			case STRICT:		this.setOption("doctype", "strict");		break;
			case TRANSITIONAL:	this.setOption("doctype", "transitional");	break;
			case FRAMESET:		this.setOption("doctype", "frameset");		break;
			default:			this.setOption("doctype", null);
		}
		
		this.head.setBase(this.getBase());
		this.body.setBase(this.getBase());
		this.setValue("page_title", this.title);
		this.setValue("head_content", this.head, groups);
		this.setValue("body_content", this.body, groups);
	}
	
	/* == atributos e construtores == */
	
	private static final PageTemplate template = 
			new PageTemplate(HtmlPage.class.getResource("HtmlPage.html"));

	/**
	 * Cria uma página vazia entitulada "Empty HTML Page", sem DTD.
	 * 
	 * @param locale
	 * A localização da página.
	 */
	public HtmlPage(Locale locale) {
		this(locale, Type.NONE, "Empty HTML Page");
	}
	
	/**
	 * Cria uma página vazia entitulada "Empty HTML Page".
	 * 
	 * @param locale
	 * A localização da página.
	 * 
	 * @param dtd
	 * O DOCTYPE a ser usado.
	 */
	public HtmlPage(Locale locale, Type dtd) {
		this(locale, dtd, "Empty HTML Page");
	}
	
	/**
	 * Cria uma página vazia sem DTD.
	 * 
	 * @param locale
	 * A localização da página.
	 * 
	 * @param title
	 * O título da página.
	 */
	public HtmlPage(Locale locale, String title) {
		this(locale, Type.NONE, title);
	}
	
	/**
	 * Cria uma página.
	 * 
	 * @param locale
	 * A localização da página.
	 * 
	 * @param dtd
	 * O DOCTYPE a ser usado.
	 * 
	 * @param title
	 * O título da página.
	 */
	public HtmlPage(Locale locale, Type dtd, String title) {
		super(template, locale);
		this.dtd = dtd;
		this.title = title;
		String lstr = locale.getLanguage();
		if(!lstr.isEmpty()) {
			if(!locale.getCountry().isEmpty())
				lstr += "-" + locale.getCountry();
			if(!locale.getVariant().isEmpty())
				lstr += "-" + locale.getVariant();
		}
		this.setHeader("Content-Language", lstr.toLowerCase());
	}
	
	private PageFragment head;
	private PageFragment body;
	private String title;
	private Type dtd;
	
	/* == classes aninhadas == */
	
	/**
	 * Os DTDs que definem a linguagem HTML oficialmente 
	 * reconhecidos pelo W3C. O valor NONE simplesmente
	 * omite a declaração DTD do documento.
	 */
	public enum Type {
		STRICT, TRANSITIONAL, FRAMESET, NONE
	}
	
}
