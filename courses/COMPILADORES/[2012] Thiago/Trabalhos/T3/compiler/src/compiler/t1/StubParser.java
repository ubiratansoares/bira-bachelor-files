package compiler.t1;

import compiler.*;
import java.util.Collections;
import java.util.List;

/**
 * Analisador sintático de fachada.
 *
 * @version 1.0.2
 */
public class StubParser implements SyntacticAnalyzer {

	private LexicalAnalyzer lexer;

	/**
	 * Construtor vazio.
	 */
	public StubParser() {
	}

	/**
	 * Define o analisador léxico pelo qual obter tokens.
	 *
	 * @param lexer
	 * O analisador léxico a ser usado na compilação.
	 */
	public void setLexer(LexicalAnalyzer lexer) {
		this.lexer = lexer;
		return;
	}

	/**
	 * Análise sintática de fachada. Apenas lista os pares "cadeia - token"
	 * gerados pelo analisador léxico. Os erros também são listados.
	 *
	 * @param source
	 * O código-fonte do programa a ser analisado.
	 *
	 * @return
	 * Uma lista de tokens.
	 */
	public ObjectCode parse(SourceCode source) {

		Token token;
		TokenList output = new TokenList();
		lexer.setSourceCode(source);

		try {
			token = lexer.next();
		} catch(LexicalException e) {
			token = e.getToken();
		}

		while(token.getType() != TokenType.EOF) {
			try {
				output.append(token);
				token = lexer.next();
			} catch(LexicalException e) {
				token = e.getToken();
			}
		}

		return output;
	}

	/**
	 * {@inheritDoc }
	 */
	public List<CompilationError> getErrors() {
		return Collections.emptyList();
	}

}
