package compiler.t2a;

import compiler.*;
import compiler.Compiler;

/**
 * A implementação do compilador. Efetua análises até a análise sintática, sem
 * tratamento de erros (a compilação aborta no primeiro erro).
 * 
 * @version 2.0.2
 */
public class CompilerImpl implements Compiler {

	private SyntacticAnalyzer parser;

	/**
	 * Construtor vazio.
	 */
	public CompilerImpl() {
	}

	/**
	 * Prepara e configura o compilador.
	 */
	public void init(String[] args) {

		KeywordsTable table = new KeywordsTable();
		table.init();

		LexicalAnalyzer lexer = new LexerImpl();
		lexer.setKeywordsTable(table);

		ParserImpl parserImpl = new ParserImpl();
		parserImpl.setLexer(lexer);
		this.parser = parserImpl;
		return;
	}

	/**
	 * Compila o programa especificado. A compilação é de única passada,
	 * e a análise sintática é descendente, preditiva e recursiva.
	 *
	 * @param source
	 * O código-fonte do programa a ser compilado.
	 *
	 * @return
	 * {@code null}, se a compilação for bem-sucedida.
	 *
	 * @throws CompilerException
	 * Lançada caso ocorra algum problema na compilação.
	 */
	public ObjectCode compile(SourceCode source) throws CompilerException {
		//o parser comanda o processo de compilação
		return ((ObjectCode) parser.parse(source));
	}

}
