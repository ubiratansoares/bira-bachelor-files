package compiler;

import java.util.List;

/**
 * Analisador sintático.
 *
 * @version 3.0.0
 */
public interface SyntacticAnalyzer {

	/**
	 * Efetua a análise sintática do código-fonte.
	 *
	 * @param source
	 * O código-fonte do programa a ser analisado.
	 *
	 * @return
	 * O resultado da compilação.
	 *
	 * @throws CompilerException
	 * Lançada para indicar qualquer erro de compilação.
	 */
	public ObjectCode parse(SourceCode source) throws CompilerException;

	/**
	 * Obtém a lista de erros de compilação.
	 * 
	 * @return
	 * A lista de erros de compilação.
	 */
	public List<CompilationError> getErrors();

}
