package rtk.template;

import java.io.*;
import java.net.*;
import java.util.*;
import java.util.regex.*;

import static rtk.template.TemplateElement.Type;
import static rtk.template.TemplateElement.Type.*;

/**
 * Molde de página. Possui métodos para carregar e analisar moldes.
 * O analisador é personalizável, mas as subclasses ainda devem gerar
 * a mesma estrutura do molde (isto é, a representação textual pode
 * mudar, mas a representação em memória não pode).
 */
public class PageTemplate {

	@Override
	public String toString() {
		return String.format("{headers: %s, template: %s}", 
				headers, template);
	}
	
	/* == getters == */
	
	/**
	 * Obtém a raiz do molde. A raiz dá acesso a todos os outros
	 * elementos do molde.
	 * 
	 * @see #ROOT_ID
	 */
	public BlockElement getTemplate() {
		return template;
	}
	
	/**
	 * Obtém todos os cabeçalhos HTTP sugeridos pelo molde.
	 * Os cabeçalhos estarão mapeados por campo:valor, e são
	 * apenas sugestões (o controlador deverá decidir usá-las
	 * ou não).
	 */
	public Map<String, String> getHeaders() {
		return Collections.unmodifiableMap(headers);
	}
	
	/**
	 * Obtém todos os identificadores usados para elementos VAR.
	 */
	public Set<String> getVarElementIds() {
		return Collections.unmodifiableSet(vars);
	}
	
	/**
	 * Obtém todos os identificadores usados para elementos URI.
	 */
	public Set<String> getUriElementIds() {
		return Collections.unmodifiableSet(uris);
	}
	
	/**
	 * Obtém todos os identificadores usados para elementos I18N.
	 */
	public Set<String> getInternationalizationKeys() {
		return Collections.unmodifiableSet(i18ns);
	}
	
	/**
	 * Obtém todos os identificadores usados para blocos OPTIONAL.
	 */
	public Set<String> getOptionalBlockIds() {
		return Collections.unmodifiableSet(optionals);
	}
	
	/**
	 * Obtém todos os identificadores usados para blocos LOOP.
	 */
	public Set<String> getLoopBlockIds() {
		return Collections.unmodifiableSet(loops.keySet());
	}
	
	/**
	 * Obtém um bloco LOOP.
	 * 
	 * @param id
	 * O identificador desse bloco.
	 */
	public BlockElement getLoopBlock(String id) {
		return loops.get(id);
	}
	
	/**
	 * Obtém todos os identificadores usados para blocos SELECT.
	 */
	public Set<String> getSelectBlockIds() {
		return Collections.unmodifiableSet(selects.keySet());
	}
	
	/**
	 * Obtém todos os identificadores usados para blocos OPTION
	 * de um dado bloco SELECT.
	 * 
	 * @param selectId
	 * O identificador do bloco SELECT do qual obter os 
	 * identificadores de blocos OPTION.
	 */
	public Set<String> getOptionBlockIds(String selectId) {
		return Collections.unmodifiableSet(selects.get(selectId));
	}
	
	/* == personalização do parser == */
	
	/**
	 * Retorna a expressão regular que identifica as instruções de
	 * processamento. 
	 * 
	 * A implementação padrão espera que as instruções sejam da forma 
	 * "'<%' INSTR VALUE '%>'", em que INSTR é composto de letras 
	 * maiúsculas ou minúsculas, hífen ou '_', e VALUE é qualquer
	 * sequência de caracteres que não seja o delimitador direito.
	 * 
	 * Subclasses estão livres para usar outra expressão regular.
	 */
	protected Pattern getPattern() {
		return Pattern.compile(String.format(
				"%s\\s*([a-zA-Z_][\\w_-]+)(\\s+(.+?))?\\s*%s", 
				prefix, suffix));
	}
	
	/**
	 * Retorna o delimitador esquerdo de uma instrução. A implementação 
	 * padrão delimita as instruções entre '<%' e '%>'. 
	 * 
	 * Subclasses estão livres para escolher outros delimitadores, mas 
	 * é recomendado usar delimitadores que não interfiram nos editores 
	 * favoritos da linguagem do arquivo gerado pelo molde (os 
	 * delimitadores padrões são dos scriptlets JSP).
	 */
	protected String getPrefix() {
		return "<%";
	}
	
	/**
	 * Retorna o delimitador direito de uma instrução. A implementação 
	 * padrão delimita as instruções entre '<%' e '%>'. 
	 * 
	 * Subclasses estão livres para escolher outros delimitadores, mas 
	 * é recomendado usar delimitadores que não interfiram nos editores 
	 * favoritos da linguagem do arquivo gerado pelo molde (os 
	 * delimitadores padrões são dos scriptlets JSP).
	 */
	protected String getSuffix() {
		return "%>";
	}
	
	/**
	 * Retorna o mapeamento das instruções de processamento. O 
	 * mapeamento ajudará a traduzir as palavras reservadas para
	 * uma enum que representa a instrução. 
	 * 
	 * A implementação padrão usa o próprio nome das constantes enum 
	 * como identificadores de comando (como a tradução é sensível ao
	 * caso, e os nomes estão em letras maiúsculas, os comandos deverão
	 * estar em letras maiúsculas no texto também).
	 * 
	 * Subclasses estão livres para sobrescrever este mapeamento.
	 * 
	 * @return
	 * O mapeamento da representação textual para a constante enum.
	 */
	protected Map<String, Type> getInstructionMap() {
		Map<String, Type> map = new HashMap<String, Type>();
		for(Type t : Type.class.getEnumConstants())
			map.put(t.toString(), t);
		return map;
	}
	
	/* == contrução do molde == */
	
	/**
	 * Carrega a representação textual do molde.
	 * 
	 * @param location
	 * Uma URL que aponta para o arquivo com a representação textual
	 * do molde (pode não ser um arquivo físico, basta ser acessível
	 * como stream de caracteres).
	 */
	public void load(URL location) throws IOException {
		InputStream stream = location.openStream();
		try {
			BufferedReader reader = new BufferedReader(
					new InputStreamReader(stream));
			StringBuilder sb = new StringBuilder();
			try {
				File file = new File(location.toURI());
				Long cap = Long.valueOf(Math.min(file.length(), Integer.MAX_VALUE));
				sb.ensureCapacity(cap.intValue());
			} catch(URISyntaxException e) {
				sb.ensureCapacity(4096);
			}
			
			String line = reader.readLine();
			while(line != null) {
				sb.append(line + "\n");
				line = reader.readLine();
			}
			
			this.parse(sb.toString());
		} finally {
			stream.close();
		}
	}
	
	/**
	 * Constrói a estrutura do molde a partir da representação 
	 * textual carregada de alguma fonte.
	 * 
	 * @param text
	 * A representação textual do molde.
	 */
	protected void parse(String text) {
		List<TemplateElement> flat = this.parseText(text);
		this.parseHeaders(flat.iterator());
		this.template = this.parseBlock(
				new TemplateElement(OPTIONAL, ROOT_ID), 
				flat.iterator());
		optionals.add(ROOT_ID);
		return;
	}
	
	//parsing do texto
	private List<TemplateElement> parseText(String text) {
		List<TemplateElement> flat = 
				new LinkedList<TemplateElement>();
		
		//pattern: (nova linha)(tabs e espaços)(qualquer coisa exceto mudança de linha)(fim da cadeia)
		Pattern ip = Pattern.compile("\\n([\\ \\t]*)[^\\n\\x0b\\f\\r]*\\z");
		String indent; Matcher im;
		
		Matcher m = pattern.matcher(text);
		int start = 0, end = 0; String str;
		indent = "";
		while(m.find()) {
			//texto
			end = m.start();
			str = text.substring(start, end);
			im = ip.matcher(str);
			if(im.find()) indent = im.group(1);
			flat.add(new TemplateElement(TEXT, str));
			start = m.end();
			
			if(instructions.containsKey(m.group(1))) {
				Type t = instructions.get(m.group(1));
				switch(t) {
					case VAR: case I18N: //indentados
						flat.add(new TemplateElement(t, m.group(3), indent));
						break;
					
					case LOCALE: //vazios
						flat.add(new TemplateElement(t));
						break;
						
					default: //normais
						flat.add(new TemplateElement(t, m.group(3)));
						break;
				}
			} else {
				//inválidos, tratar como texto
				im = ip.matcher(m.group(0));
				if(im.find()) indent = im.group(1);
				flat.add(new TemplateElement(TEXT, m.group(0)));
			}
		}
		//texto no final do documento
		end = text.length();
		str = text.substring(start, end);
		flat.add(new TemplateElement(TEXT, str));
		
		return flat;
	}
	
	private void parseHeaders(Iterator<TemplateElement> i) {
		Pattern hp = Pattern.compile("([^:]+):(.*)");
		while(i.hasNext()) {
			TemplateElement e = i.next();
			if(e.getType() == HEAD) {
				Matcher hm = hp.matcher(e.getValue());
				if(hm.matches()) {
					headers.put(hm.group(1).trim(), 
							hm.group(2).trim());
				}
				i.remove();
			}
		}
	}
	
	private BlockElement parseBlock(
			TemplateElement begin, Iterator<TemplateElement> i) {
		List<TemplateElement> node = new LinkedList<TemplateElement>();
		
		while(i.hasNext()) {
			TemplateElement e = i.next();
			switch(e.getType()) {
				case TEXT: {
					if(!e.getValue().isEmpty())
						node.add(e);
				} break;
				
				case VAR: {
					vars.add(e.getValue());
					node.add(e);
				} break;
				
				case URI: {
					uris.add(e.getValue());
					node.add(e);
				} break;
				
				case I18N: {
					i18ns.add(e.getValue());
					node.add(e);
				} break;
				
				case LOCALE: {
					node.add(e);
				} break;
				
				case OPTIONAL: {
					optionals.add(e.getValue());
					node.add(this.parseBlock(e, i));
				} break;
				
				case LOOP: {
					BlockElement loop = this.parseBlock(e, i);
					loops.put(loop.getValue(), loop);
					node.add(loop);
				} break;
				
				case SELECT: {
					selects.put(e.getValue(), new HashSet<String>());
					node.add(this.parseSelect(e.getValue(), i));
				} break;
				
				case END: {
					if(e.getValue().equals(begin.getValue())) {
						return new BlockElement(begin.getType(), 
								begin.getValue(), node);
					}
				} break;
			}
		}
		
		return new BlockElement(begin.getType(), begin.getValue(), node);
	}
	
	private BlockElement parseSelect(
			String id, Iterator<TemplateElement> i) {
		List<TemplateElement> node = new LinkedList<TemplateElement>();
		List<TemplateElement> opt = new LinkedList<TemplateElement>();
		
		TemplateElement e = null;
		boolean lookahead = false;
		while(i.hasNext()) {
			if(!lookahead) e = i.next();
			lookahead = false;
			switch(e.getType()) {
				case OPTION: {
					opt.clear();
					String optId = e.getValue();
					selects.get(id).add(optId);
					e = this.parseOption(id, opt, i);
					if(e != null) lookahead = true;
					node.add(new BlockElement(OPTION, optId, opt));
				} break;
				
				case END: {
					if(e.getValue().equals(id))
						return new BlockElement(SELECT, id, node);
				} break;
			}
		}
		
		return new BlockElement(SELECT, id, node);
	}
	
	private TemplateElement parseOption(String selId, 
			List<TemplateElement> node, Iterator<TemplateElement> i) {
		
		while(i.hasNext()) {
			TemplateElement e = i.next();
			switch(e.getType()) {
				case TEXT: {
					if(!e.getValue().isEmpty())
						node.add(e);
				} break;
				
				case VAR: {
					vars.add(e.getValue());
					node.add(e);
				} break;
				
				case URI: {
					uris.add(e.getValue());
					node.add(e);
				} break;
				
				case I18N: {
					i18ns.add(e.getValue());
					node.add(e);
				} break;
				
				case LOCALE: {
					node.add(e);
				} break;
				
				case OPTIONAL: {
					optionals.add(e.getValue());
					node.add(this.parseBlock(e, i));
				} break;
				
				case LOOP: {
					BlockElement loop = this.parseBlock(e, i);
					loops.put(loop.getValue(), loop);
					node.add(loop);
				} break;
				
				case SELECT: {
					selects.put(e.getValue(), new HashSet<String>());
					node.add(this.parseSelect(e.getValue(), i));
				} break;
				
				case OPTION:
					return e;
				
				case END: {
					if(e.getValue().equals(selId))
						return e;
				} break;
			}
		}
		
		return null;
	}
	
	/* == atributos e construtores == */
	
	public static final String ROOT_ID = "__ROOT";
	
	private BlockElement template;
	private Map<String, String> headers;
	private Set<String> vars;
	private Set<String> uris;
	private Set<String> i18ns;
	private Set<String> optionals;
	private Map<String, BlockElement> loops;
	private Map<String, Set<String>> selects;
	
	private Pattern pattern;
	private String prefix;
	private String suffix;
	
	private Map<String, Type> instructions;
	
	/**
	 * Cria um molde vazio e configurado para leitura.
	 */
	public PageTemplate() {
		this.prefix = this.getPrefix();
		this.suffix = this.getSuffix();
		this.pattern = this.getPattern();
		this.instructions = this.getInstructionMap();
		
		this.template = new BlockElement(OPTIONAL, 
				ROOT_ID, new ArrayList<TemplateElement>());
		this.headers = new HashMap<String, String>();
		this.vars = new HashSet<String>();
		this.uris = new HashSet<String>();
		this.i18ns = new HashSet<String>();
		this.optionals = new HashSet<String>();
		this.loops = new HashMap<String, BlockElement>();
		this.selects = new HashMap<String, Set<String>>();
	}
	
	/**
	 * Carrega um molde da representação textual do arquivo
	 * (ou stream de caracteres) localizado na URL especificada.
	 * 
	 * @throws RuntimeException
	 * Lançada caso o método {@link #load} subjacente lance qualquer
	 * exceção (a exceção é encapsulada em uma RuntimeException para 
	 * que possa ser usado convenientemente em inicializações, o que
	 * obviamente traz alguns riscos).
	 */
	public PageTemplate(java.net.URL location) {
		this();
		try {
			this.load(location);
		} catch(IOException e) {
			throw new RuntimeException(e);
		}
	}
	
}
