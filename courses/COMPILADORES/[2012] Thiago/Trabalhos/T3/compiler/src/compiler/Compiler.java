package compiler;

/**
 * A interface de um compilador.
 *
 */
public interface Compiler {

	/**
	 * Prepara e configura o compilador.
	 */
	public void init(String[] args);

	/**
	 * Compila o programa especificado.
	 *
	 * @param source
	 * O código-fonte do programa a ser compilado.
	 *
	 * @return
	 * O código-objeto gerado pela compilação.
	 *
	 * @throws CompilerException
	 * Lançada caso ocorra algum problema na compilação.
	 */
	public ObjectCode compile(SourceCode source) throws CompilerException;

}
