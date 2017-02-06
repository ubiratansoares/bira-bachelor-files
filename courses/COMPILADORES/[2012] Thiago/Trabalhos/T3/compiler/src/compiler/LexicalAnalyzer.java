package compiler;

/**
 * Analisador léxico do compilador.
 *
 */
public abstract class LexicalAnalyzer {

	private KeywordsTable keywords;
	private SourceCode source;

	/**
	 * Construtor vazio.
	 */
	protected LexicalAnalyzer() {
	}

	/**
	 * Define a tabela de palavras reservadas usada pelo
	 * analisador léxico.
	 *
	 * @param table
	 * A tabela de palavras reservadas.
	 */
	public void setKeywordsTable(KeywordsTable table) {
		this.keywords = table;
		return;
	}

	/**
	 * Define o código-fonte de onde extrair os tokens
	 * durante o processo de compilação.
	 * 
	 * @param src
	 * O código-fonte de onde extrair os tokens.
	 */
	public void setSourceCode(SourceCode src) {
		this.source = src;
		return;
	}

	/**
	 * Obtém o código-fonte de onde extrair os tokens
	 * durante o processo de compilação.
	 *
	 * @return
	 * O código-fonte de onde extrair os tokens.
	 */
	public SourceCode getSourceCode() {
		return source;
	}

	/**
	 * Retorna o próximo token do código-fonte.
	 *
	 * @return
	 * Um token identificado pelo analisador léxico.
	 *
	 * @throws LexicalException
	 * Lançada caso uma cadeia inválida seja encontrada
	 * pela análise léxica.
	 */
	public abstract Token next() throws LexicalException;

	/**
	 * Verifica se um token é na verdade uma palavra reservada. Um token
	 * é uma palavra reservada se é um identificador e está na tabela de
	 * palavras reservadas.
	 *
	 * @param token
	 * O token a ser verificado.
	 *
	 * @return
	 * O próprio token, se não for uma palavra reservada,
	 * ou outro token apropriado caso contrário.
	 */
	protected Token checkKeyword(Token token) {
		if(token.getType() == TokenType.IDENTIFIER) {
			if(keywords.contains(token.getString()))
				return Token.createKeyword(token.getString());
		}
		return token;
	}

}
