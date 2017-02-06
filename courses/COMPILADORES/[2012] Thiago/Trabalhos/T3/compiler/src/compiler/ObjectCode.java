package compiler;

import java.io.IOException;

/**
 * Código-objeto gerado pelo compilador.
 *
 */
public interface ObjectCode {

	/**
	 * Escreve o conteúdo do código-objeto na saída padrão.
	 */
	public void println();

	/**
	 * Escreve o conteúdo do código-objeto no arquivo especificado.
	 * 
	 * @param filename
	 * O nome do arquivo a ser escrito.
	 *
	 * @throws IOException
	 * Lançada caso ocorra qualquer problema na escrita do arquivo.
	 */
	public void writeFile(String filename) throws IOException;

}
