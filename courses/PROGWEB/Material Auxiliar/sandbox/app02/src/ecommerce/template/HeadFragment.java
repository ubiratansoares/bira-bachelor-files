package ecommerce.template;

import java.net.URI;
import java.util.*;

/**
 * Gera o conteúdo do elemento HEAD. Este fragmento gera elementos
 * LINK ou SCRIPT para incluir CSS, ou SCRIPT para incluir JavaScript.
 */
public class HeadFragment extends PageFragment {
	
	/* == atributos e construtores == */
	
	private static final PageTemplate template = 
		new PageTemplate(HeadFragment.class.getResource("HeadFragment.html"));
	
	public HeadFragment() {
		super(template);
		this.css = new LinkedList<Element>();
		this.js = new LinkedList<Element>();
	}
	
	private List<Element> css;
	private List<Element> js;
	
	/* == setters == */
	
	/**
	 * Adiciona uma referência a um arquivo CSS externo. 
	 * O tipo de mídia associado será "all".
	 * 
	 * @param file
	 * A URI que aponta para o local do arquivo, do ponto
	 * de vista do cliente.
	 */
	public void addStyleSheet(URI file) {
		css.add(new Element(CSSMediaType.ALL, file));
	}
	
	/**
	 * Adiciona um elemento STYLE com código CSS.
	 * O tipo de mídia associado será "all".
	 * 
	 * @param code
	 * O código CSS do elemento STYLE.
	 */
	public void addStyleSheet(String code) {
		css.add(new Element(CSSMediaType.ALL, code));
	}
	
	/**
	 * Adiciona uma referência a um arquivo CSS externo.
	 */
	public void addStyleSheet(URI file, CSSMediaType media) {
		css.add(new Element(media, file));
	}
	
	/**
	 * Adiciona um elemento STYLE com código CSS.
	 */
	public void addStyleSheet(String code, CSSMediaType media) {
		css.add(new Element(media, code));
	}
	
	/**
	 * Adiciona um elemento SCRIPT com referência a um arquivo
	 * JavaScript externo. O script é marcado para carregar mais
	 * tarde, pois garante que não irá executar "document.write"
	 * (o que fará a página carregar mais rápido).
	 */
	public void addScript(URI file) {
		js.add(new Element(true, file));
	}
	
	/**
	 * Adiciona um elemento SCRIPT contendo código JavaScript. O script 
	 * é marcado para carregar mais tarde, pois garante que não irá 
	 * executar "document.write" (o que fará a página carregar mais 
	 * rápido).
	 */
	public void addScript(String code) {
		js.add(new Element(true, code));
	}
	
	/**
	 * Adiciona um elemento SCRIPT com referência a um arquivo
	 * JavaScript externo.
	 */
	public void addScript(URI file, boolean defer) {
		js.add(new Element(defer, file));
	}
	
	/**
	 * Adiciona um elemento SCRIPT contendo código JavaScript.
	 */
	public void addScript(String code, boolean defer) {
		js.add(new Element(defer, code));
	}
	
	/* == construção da página == */
	
	/**
	 * Contrói o fragmento, listando elementos LINK, STYLE e SCRIPT.
	 */
	protected void set() {
		for(Element e : css) {
			if(e.isExternal()) {
				this.setOption("css_location", "css_external");
				this.setTarget("css_ref", e.getURI());
			} else {
				this.setOption("css_location", "css_embedded");
				this.setValue("css_code", e.getSourceCode());
			}
			this.setValue("css_media", e.getMediaType().toString().toLowerCase());
			this.stepLoop("css_loop");
		}
		
		for(Element e : js) {
			if(e.isExternal()) {
				this.setOption("js_location", "js_external");
				this.setTarget("js_ref", e.getURI());
			} else {
				this.setOption("js_location", "js_embedded");
				this.setValue("js_code", e.getSourceCode());
			}
			this.setValue("js_defer", e.isDeferred()? "defer": "");
			this.stepLoop("js_loop");
		}
	}
	
	/**
	 * Apenas invoca {@link #set()}.
	 */
	protected void set(String group) {
		this.set();
	}
	
	/**
	 * Apenas invoca {@link #set()}.
	 */
	protected void set(Set<String> group) {
		this.set();
	}
	
	/* == classes aninhadas == */
	
	private class Element {
		private boolean external;
		private boolean defer;
		private CSSMediaType media;
		private URI file;
		private String code;
		
		public Element(CSSMediaType media, URI file) {
			this.external = true;
			this.media = media;
			this.file = file;
		}
		
		public Element(CSSMediaType media, String code) {
			this.external = false;
			this.media = media;
			this.code = code;
		}
		
		public Element(boolean defer, URI file) {
			this.external = true;
			this.defer = defer;
			this.file = file;
		}
		
		public Element(boolean defer, String code) {
			this.external = false;
			this.defer = defer;
			this.code = code;
		}
		
		public boolean isExternal() {
			return external;
		}
		
		public boolean isDeferred() {
			return defer;
		}
		
		public CSSMediaType getMediaType() {
			return media;
		}
		
		public URI getURI() {
			return file;
		}
		
		public String getSourceCode() {
			return code;
		}
		
	}
	
	/**
	 * Tipos de mídia reconhecidos pela especificação CSS 2.1.
	 */
	public static enum CSSMediaType {
		ALL, BRAILLE, EMBOSSED, HANDHELD, PRINT, 
		PROJECTION, SCREEN, SPEECH, TTY, TV
	}
	
}
