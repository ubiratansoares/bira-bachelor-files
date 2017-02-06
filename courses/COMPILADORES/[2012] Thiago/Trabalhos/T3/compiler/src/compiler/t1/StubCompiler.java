package compiler.t1;

import compiler.Compiler;
import compiler.KeywordsTable;
import compiler.LexicalAnalyzer;
import compiler.ObjectCode;
import compiler.SourceCode;
import compiler.SyntacticAnalyzer;

import compiler.CompilerException;

/**
 * Uma implementação de fachada para o compilador.
 * 
 * @version 1.0.2
 */
public class StubCompiler implements Compiler {

	private SyntacticAnalyzer parser;

	/**
	 * Construtor vazio.
	 */
	public StubCompiler() {
	}

	/**
	 * Prepara e configura o compilador.
	 */
	public void init(String[] args) {

		KeywordsTable table = new KeywordsTable();
		table.init();

		LexicalAnalyzer lexer = new LexerImpl();
		lexer.setKeywordsTable(table);

		StubParser stubParser = new StubParser();
		stubParser.setLexer(lexer);
		this.parser = stubParser;

		return;
	}

	/**
	 * Compila o programa especificado.
	 *
	 * @param source
	 * O código-fonte do programa a ser compilado.
	 *
	 * @return
	 * Uma lista de tokens.
	 *
	 * @throws CompilerException
	 * Lançada caso ocorra algum problema na compilação.
	 */
	public ObjectCode compile(SourceCode source) throws CompilerException {
		return ((ObjectCode) parser.parse(source));
	}

}
