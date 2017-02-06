package rtk.template;

import java.net.*;
import java.util.*;

import static rtk.template.TemplateElement.Type.*;

/**
 * Representa um fragmento de documento em formato de texto. Por 
 * exemplo, o documento pode ser uma página HTML, um documento em XML,
 * ou um simples texto sem formato. Um fragmento pode ser uma tag HTML,
 * tag XML, função do JavaScript ou trecho de um texto simples.
 * 
 * O fragemento deve ser carregado de um molde, e opcionalmente pode
 * substituir variáveis ou processar instruções simples. As instruções
 * aceitas por esta implementação são:
 * 
 * "HEAD <param>:<valor>": sugere um valor para um parâmetro do 
 * 		cabeçalho HTTP (o usuário decidirá aceitá-lo ou não);
 * "VAR <id>": variável de substituição (vazio por padrão, 
 * 		propaga indentações);
 * "URI <id>": URI de substituição (vazio por padrão), semelhante ao
 * 		VAR, mas relativiza endereços com relação a uma base;
 * "OPTIONAL <id> ... END <id>": fragmento opcional (oculto por padrão);
 * "LOOP <id> ... END <id>": fragmento repetitivo (as repetições 
 * 		podem ser diferentes, pois também podem conter instruções);
 * "SELECT <id> OPTION <id1> ... OPTION <id2> ... OPTION <idn> ... END <id>":
 * 		fragmentos opcionais, escolhe uma das opções ou oculta 
 * 		todas elas (oculta todas por padrão).
 * 
 * Além disso, cada fragmento gerencia suas próprias permissões, e o
 * comportamento padrão é permitir que todos os usuários acessem seu
 * conteúdo. Caso um usuário não deva acessar um fragmento, o fragmento
 * gera uma cadeia vazia como resultado (mas ainda é possível que 
 * implementações lancem {@link java.lang.RuntimeException}s ao
 * verificarem as permissões.
 * 
 * Por fim, cada fragmento gerencia um conjunto de recursos 
 * compartilhados. Esses recursos poderão ser criados pelos 
 * próprios fragmentos quando indisponíveis, mas geralmente
 * é melhor usar a mesma instância de um recurso caro tanto 
 * quanto isso seja viável. Por padrão, nenhum recurso será
 * criado quando indisponível.
 */
public abstract class PageFragment {
	
	/* == gerenciamento de recursos == */
	
	/**
	 * Compartilha um recurso (descritor de arquivo, socket, 
	 * conexão com a base de dados, etc) com este fragmento.
	 * Fica a critério do fragmento usar ou não um recurso, 
	 * mas a implementação do fragmento deve garantir que:
	 * 
	 * 1) O recurso não seja liberado se tiver sido compartilhado 
	 * por este método (isto é, se ele não for criado pelo próprio
	 * fragmento);
	 * 
	 * 2) O recurso seja criado e liberado pelo próprio fragmento
	 * caso não esteja disponível, se possível.
	 * 
	 * @param id
	 * Um identificador pelo qual o fragmento obterá o recurso.
	 * 
	 * @param resource
	 * O recurso a ser compartilhado.
	 */
	public void shareResource(String id, Object resource) {
		if(resource == null) return;
		
		SharedResource r = resources.get(id);
		if(r == null) return;
		if(!r.type.isAssignableFrom(resource.getClass()))
			throw new IllegalArgumentException();
		
		r.value = resource;
	}
	
	/**
	 * Declara um recurso esperado pelo fragmento. Implementações
	 * deverão declarar os tipos e os identificadores dos recursos
	 * antes de poder acessá-los.
	 * 
	 * @param id
	 * O identificador usado para acessar o recurso.
	 * 
	 * @param type
	 * O tipo do recurso compartilhado.
	 */
	protected void expectResource(String id, Class<?> type) {
		resources.put(id, new SharedResource(type));
	}
	
	/**
	 * Obtém o recurso compartilhado. Caso não esteja disponível
	 * (isto é, se {@code null} estiver associado a ele) uma nova
	 * instância poderá ser criada pelo método 
	 * {@link #getDefaultResource}. A implementação poderá descobrir
	 * se um recurso foi criado ou obtido através do método 
	 * {@link #isShared}.
	 * 
	 * @param id
	 * O identificador usado para recuperar o recurso.
	 * 
	 * @param type
	 * O tipo do recurso (como declarado).
	 * 
	 * @return
	 * O recurso compartilhado, uma nova instância do recurso ou 
	 * {@code null}.
	 */
	protected <T> T getSharedResource(String id, Class<T> type) {
		Object r = resources.get(id).value;
		if(r == null) 
			r = this.getDefaultResource(id, type);
		return type.cast(r);
	}
	
	/**
	 * Cria uma nova instância do recurso caso ele não esteja 
	 * disponível. A implementação padrão apenas retorna {@code null}.
	 * 
	 * @param id
	 * O identificador do recurso.
	 * 
	 * @param type
	 * O tipo do recurso (como declarado).
	 * 
	 * @return
	 * Uma nova instância do recurso. Se esse recurso precisar ser
	 * liberado, ele deverá ser liberado pelo próprio fragmento, ao 
	 * invés de ser deixado aberto.
	 */
	protected <T> T getDefaultResource(String id, Class<T> type) {
		return null;
	}
	
	/**
	 * Verifica se um recurso foi criado pelo fragmento ou 
	 * compartilhado pelo usuário. Um recurso é criado pelo
	 * fragmento, se o usuário não compartilhá-lo.
	 * 
	 * @param id
	 * O identificador do recurso.
	 */
	protected boolean isShared(String id) {
		return (resources.get(id).value != null);
	}
	
	/**
	 * Remove todos os recursos compartilhados. O fragmento 
	 * ainda pode obter os recursos usando instâncias novas, 
	 * se for possível criá-las.
	 */
	public void clearResources() {
		for(SharedResource r : resources.values())
			r.value = null;
	}
	
	/* == controle de permissões == */
	
	/**
	 * Verifica se o fragmento deve ser servido para um usuário sem
	 * credenciais. A implementação padrão permite que qualquer usuário 
	 * tenha acesso ao conteúdo. 
	 * 
	 * @return
	 * {@code true} caso o usuário deva receber o conteúdo do fragmento
	 * processado; {@code false} caso contrário.
	 */
	protected boolean isAllowed() {
		return true;
	}
	
	/**
	 * Verifica se o fragmento deve ser servido para um usuário com
	 * a credencial especificada. A implementação padrão permite que 
	 * qualquer usuário tenha acesso ao conteúdo. Fica a cargo de cada
	 * implementação processar essa credencial.
	 * 
	 * @param group
	 * Uma das credenciais do usuário.
	 * 
	 * @return
	 * {@code true} caso o usuário deva receber o conteúdo do fragmento
	 * processado; {@code false} caso contrário.
	 */
	protected boolean isAllowed(String group) {
		return true;
	}
	
	/**
	 * Verifica se o fragmento deve ser servido para um usuário com
	 * as credenciais especificadas. A implementação padrão permite 
	 * que qualquer usuário tenha acesso ao conteúdo. Fica a cargo de 
	 * cada implementação processar essas credenciais.
	 * 
	 * @param groups
	 * Todas as credenciais do usuário.
	 * 
	 * @return
	 * {@code true} caso o usuário deva receber o conteúdo do fragmento
	 * processado; {@code false} caso contrário.
	 */
	protected boolean isAllowed(Set<String> group) {
		return true;
	}
	
	/* == mapeamento dos valores == */
	
	/**
	 * Define um valor de substituição.
	 * 
	 * @param id
	 * O identificador usado no molde para a instrução VAR.
	 * 
	 * @param value
	 * O valor que substituirá todas as instâncias da instrução
	 * VAR com identificador correspondente no molde.
	 * 
	 * @see #setTarget
	 */
	protected void setValue(String id, String value) {
		if(vars.containsKey(id))
			vars.put(id, value);
	}
	
	/**
	 * Define uma URI de substituição. As URIs serão normalizadas e 
	 * relativizadas com relação a uma base especificada antes da
	 * substituição.
	 * 
	 * @param id
	 * O identificador usado no molde para a instrução URI.
	 * 
	 * @param value
	 * O valor que substituirá todas as instâncias da instrução
	 * URI com identificador correspondente no molde. A URI deve
	 * ser preferencialmente absoluta para que a relativização 
	 * seja feita corretamente.
	 * 
	 * @see #setValue 
	 * @see #setBase
	 */
	protected void setTarget(String id, java.net.URI value) {
		try {
			//criar cópia normalizada
			String str = (value != null)? value.toString(): "";
			if(targets.containsKey(id))
				targets.put(id, new URI(str).normalize());
		} catch(URISyntaxException e) {
			//ignorar
		}
	}
	
	/**
	 * Define a URI base que será usada para relativizar todas as 
	 * URIs usadas no documento. Esta URI deve ser preferencialmente 
	 * absoluta para que a relativização seja feita corretamente.
	 * 
	 * @param value
	 * A URI que será usada como base para relativizar todas as URIs
	 * deste fragmento.
	 * 
	 * @see #setTarget
	 */
	public void setBase(java.net.URI value) {
		try {
			//criar cópia normalizada
			String str = (value != null)? value.toString(): "";
			baseURI = new URI(str).normalize();
		} catch(URISyntaxException e) {
			//ignorar
		}
	}
	
	/**
	 * Obtém a URI base que será usada para relativizar todas as 
	 * URIs usadas no documento. Esta URI deve ser preferencialmente 
	 * absoluta para que a relativização seja feita corretamente.
	 * 
	 * @return 
	 * A URI que será usada como base para relativizar todas as URIs
	 * deste fragmento.
	 * 
	 * @see #setTarget
	 */
	public java.net.URI getBase() {
		try {
			//criar cópia normalizada
			String str = (baseURI != null)? baseURI.toString(): "";
			return new URI(str).normalize();
		} catch(URISyntaxException e) {
			//ignorar
			return null;
		}
	}
	
	/**
	 * Define a visibilidade de um fragmento opcional. Fragmentos
	 * ocultos serão completamente removidos do documento resultante.
	 * 
	 * @param id
	 * O identificador usado no molde para a instrução URI.
	 * 
	 * @param value
	 * {@code true} mantém o fragmento contido na instrução no 
	 * documento resultante. {@code false} remove o fragmento do
	 * resultado (incluindo todas as instruções contidas nela).
	 */
	protected void displayOptional(String id, boolean value) {
		if(flags.containsKey(id))
			flags.put(id, value);
	}
	
	/**
	 * Define a opção a ser usada pelo fragmento SELECT. Por padrão,
	 * nenhum dos fragmentos OPTION é escolhido (portanto todos serão
	 * removidos do resultado). Uma (e apenas uma) das opções pode ser
	 * escolhida para ser mantida, sendo que o restante dos fragmentos
	 * será descartado.
	 * 
	 * @param groupId
	 * O identificador do fragmento SELECT cuja opção será definida.
	 * 
	 * @param optionId
	 * O identificador do fragmento OPTION a ser escolhido. Pode ser
	 * um identificador não correspondente ou {@code null} para indicar
	 * que todas as opções devem ser removidas.
	 */
	protected void setOption(String groupId, String optionId) {
		if(vars.containsKey(groupId))
			vars.put(groupId, optionId);
	}
	
	/**
	 * Define uma sugestão de valor para um HTTP header. O usuário do
	 * fragmento não é obrigado usar a sugestão, mas ele pode ser usado
	 * para guiá-lo de acordo com sua conveniência.
	 * 
	 * @param key
	 * O campo HTTP cujo valor será definido.
	 * 
	 * @param value
	 * O valor sugerido pelo fragmento para esse campo.
	 */
	protected void putHeader(String key, String value) {
		headers.put(key, value);
	}
	
	/**
	 * Obtém os headers HTTP sugeridos pelo fragmento. As sugestões 
	 * podem ser provenientes do molde ou da implementação (subclasse),
	 * e fica a critério da implementação oferecer um ou outro valor.
	 * 
	 * @return
	 * Um mapa com todas as sugestões de headers HTTP do fragmento. O
	 * mapa pode conter sugestões do molde, da aplicação ou até mesmo
	 * de fragmentos usados internamente.
	 */
	public Map<String, String> getHeaders() {
		return Collections.unmodifiableMap(headers);
	}
	
	/**
	 * Descarta todos os valores de substituição, de controle e conteúdo
	 * de buffers de loops, definindo todos os valores para o padrão.
	 */
	protected void clear() {
		vars.clear();
		for(String id : template.getSelectBlockIds())
			vars.put(id, null);
		for(String id : template.getVarElementIds())
			vars.put(id, null);
		
		targets.clear();
		for(String id : template.getUriElementIds())
			targets.put(id, null);
		
		flags.clear();
		for(String id : template.getOptionalBlockIds())
			flags.put(id, false);
		
		for(String id : template.getLoopBlockIds())
			loops.get(id).setLength(0);
	}
	
	/* == processamento do fragmento == */
	
	/**
	 * Resolve e insere uma iteração de laço especificado. O conteúdo
	 * do laço é resolvido com os valores de substituição atuais 
	 * (incluindo o resultado de outras instruções como OPTIONAL e 
	 * LOOP) e o resultado é armazenado para uma futura substituição
	 * final (pelo método {@link #build}).
	 * 
	 * @param id
	 * O identificador da instrução LOOP cujo conteúdo deve ser 
	 * processado. Todas as variáveis e instruções de interesse 
	 * dentro dessa instrução devem ter sido previamente resolvidas.
	 * 
	 * @see #clearLoop
	 */
	protected void stepLoop(String id) {
		BlockElement loop = template.getLoopBlock(id);
		if(loop != null) {
			StringBuilder sb = loops.get(id);
			for(TemplateElement e : loop.getElements())
				this.process(e, sb);
		}
	}
	
	/**
	 * Desfaz um laço especificado. O método se destina a reiniciar 
	 * laços internos para que laços externos processem novas iterações.
	 * Note que todo o conteúdo gerado pelo laço é perdido ao invocar
	 * este método (a não ser que ele tenha sido usado como parte do
	 * processamento de um laço mais externo).
	 * 
	 * @param id
	 * O identificador da instrução LOOP cujo conteúdo deve ser 
	 * descartado. Note que este método descarta apenas o conteúdo
	 * do buffer deste laço, portanto outros laços (internos ou 
	 * externos) não são afetados.
	 */
	protected void clearLoop(String id) {
		StringBuilder sb = loops.get(id);
		if(sb != null) sb.setLength(0);
	}
	
	/**
	 * Este método será chamado pelo método {@link #get()} antes do 
	 * início do processamento do resultado. Implementações poderão 
	 * definir as variáveis de substituição nessa hora (os métodos 
	 * de definição dessas variáveis não são públicas para permitir 
	 * que implementações ocultem a construção do fragmento e exponham 
	 * getters e setters próprios no lugar). A implementação padrão
	 * é vazia.
	 */
	protected void set() {
		return;
	}
	
	/**
	 * Este método será chamado pelo método 
	 * {@link #get(java.lang.String)} antes 
	 * do início do processamento do resultado. 
	 * 
	 * @param group
	 * A credencial do usuário que requisitou o fragmento.
	 */
	protected void set(String group) {
		return;
	}
	
	/**
	 * Este método será chamado pelo método 
	 * {@link #get(java.util.Set<java.lang.String>)} 
	 * antes do início do processamento do resultado. 
	 * 
	 * @param groups
	 * As credenciais do usuário que requisitou o fragmento.
	 */
	protected void set(Set<String> groups) {
		return;
	}
	
	/**
	 * Processa o fragmento, substituindo todas as variáveis e 
	 * resolvendo instruções do molde. Este método requisita o 
	 * fragemento sem apresentar credenciais.
	 * 
	 * @return
	 * Uma string com o conteúdo final do fragmento.
	 */
	public String get() {
		if(this.isAllowed()) {
			flags.put(PageTemplate.ROOT_ID, true);
			this.set();
			StringBuilder sb = new StringBuilder();
			this.process(template.getTemplate(), sb);
			return sb.toString();
		} else
			return "";
	}
	
	/**
	 * Processa o fragmento, substituindo todas as variáveis e 
	 * resolvendo instruções do molde. Este método requisita o
	 * fragmento apresentando uma única credencial.
	 * 
	 * @param group
	 * Uma das credenciais do usuário que requisitou o fragmento.
	 * 
	 * @return
	 * Uma string com o conteúdo final do fragmento.
	 */
	public String get(String group) {
		if(this.isAllowed(group)) {
			flags.put(PageTemplate.ROOT_ID, true);
			this.set(group);
			StringBuilder sb = new StringBuilder();
			this.process(template.getTemplate(), sb);
			return sb.toString();
		} else
			return "";
	}
	
	/**
	 * Processa o fragmento, substituindo todas as variáveis e 
	 * resolvendo instruções do molde. Este método requisita o
	 * fragmento apresentando todas as credenciais.
	 * 
	 * @param groups
	 * Todas as credenciais do usuário que requisitou o fragmento.
	 * 
	 * @return
	 * Uma string com o conteúdo final do fragmento.
	 */
	public String get(Set<String> groups) {
		if(this.isAllowed(groups)) {
			flags.put(PageTemplate.ROOT_ID, true);
			this.set(groups);
			StringBuilder sb = new StringBuilder();
			this.process(template.getTemplate(), sb);
			return sb.toString();
		} else
			return "";
	}
	
	private void process(TemplateElement e, StringBuilder buffer) {
		switch(e.getType()) {
			case TEXT: { //echo
				buffer.append(e.getValue());
			} return;
			
			case VAR: { //replace
				String value = vars.get(e.getValue());
				if(value != null) buffer.append(value);
			} return;
			
			case URI: { //replace
				java.net.URI value = targets.get(e.getValue());
				if(value != null)
					buffer.append(baseURI.relativize(value));
			} return;
			
			default: {
				this.process((BlockElement) e, buffer);
			} return;
		}
	}
	
	private void process(BlockElement b, StringBuilder buffer) {
		switch(b.getType()) {
			case OPTIONAL: {
				if(flags.get(b.getValue())) {
					for(TemplateElement e : b.getElements())
						process(e, buffer);
				}
			} return;
			
			case SELECT: {
				for(TemplateElement e : b.getElements()) {
					String id = vars.get(b.getValue());
					if(e.getValue().equals(id)) //OPTION
						process((BlockElement) e, buffer);
				}
			} return;
			
			case OPTION: {
				for(TemplateElement e : b.getElements())
					process(e, buffer);
			} break;
			
			case LOOP: {
				buffer.append(loops.get(b.getValue()).toString());
			} break;
		}
	}
	
	/**
	 * Exibe o molde usado pelo fragmento.
	 */
	@Override
	public String toString() {
		return template.toString();
	}
	
	/* == atributos e construtores == */
	
	private PageTemplate template;
	
	private Map<String, String> headers;
	private Map<String, String> vars;
	private Map<String, Boolean> flags;
	private Map<String, StringBuilder> loops;
	private Map<String, URI> targets;
	private URI baseURI;
	
	private Map<String, SharedResource> resources;
	
	/**
	 * Cria um fragmento de página vazio. O fragmento já terá um
	 * molde carregado, mas não terá nenhum valor de substituição
	 * inicializado. A URI de base será vazia e as condições serão
	 * inicializadas como falsas.
	 */
	protected PageFragment(PageTemplate template) {
		this.headers = new HashMap<String, String>();
		this.vars = new HashMap<String, String>();
		this.flags = new HashMap<String, Boolean>();
		this.targets = new HashMap<String, java.net.URI>();
		
		try {
			this.baseURI = new URI("");
		} catch(URISyntaxException e) {
			//ignorar
		}
		
		this.loops = new HashMap<String, StringBuilder>();
		for(String id : template.getLoopBlockIds())
			loops.put(id, new StringBuilder());
		
		this.headers.putAll(template.getHeaders());
		
		this.template = template;
		this.clear();
		
		this.resources = new HashMap<String, SharedResource>();
	}
	
	/* == classes aninhadas == */
	
	private class SharedResource {
		public Object value;
		public Class<?> type;
		
		public SharedResource(Class<?> type) {
			this.type = type;
			this.value = null;
		}
	}
	
}
