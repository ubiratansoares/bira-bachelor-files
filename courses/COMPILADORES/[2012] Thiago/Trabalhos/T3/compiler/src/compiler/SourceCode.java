package compiler;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.Reader;

import java.util.ArrayList;
import java.util.List;
import java.util.ListIterator;

import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Código-fonte de um programa LALG. O código-fonte é lido de um arquivo,
 * e carregado em um buffer. O caracter 'EOF' não é adicionado ao fim do
 * buffer.
 *
 */
public class SourceCode {

	private List<Char> buffer;
	private ListIterator<Char> marker;

	/**
	 * Cria um objeto código-fonte vazio.
	 */
	public SourceCode() {
		buffer = new ArrayList<Char>();
		marker = buffer.listIterator();
	}

	/**
	 * Carrega o arquivo de código-fonte para o buffer.
	 *
	 * @param filename
	 * O nome do arquivo a ser carregado.
	 *
	 * @throws FileNotFoundException
	 * Lançada caso o arquivo não seja encontrado.
	 *
	 * @throws IOException
	 * Lançada caso ocorra qualquer outro problema de E/S.
	 */
	public void load(String filename) throws IOException {
		Reader input = null;
		int line, column, c;
		try {

			buffer.clear(); //descarta o conteúdo antigo
			input = new BufferedReader(new FileReader(filename));

			try {
				line = 0; column = 0;
				c = input.read();
				while(c != -1) { //EOF

					buffer.add(new Char((char) c, line, column));
					column ++;

					if(c == '\n') {
						column = 0;
						line++;
					}

					c = input.read();
				}
			} catch(IOException e) {
				//caso a leitura falhe
				buffer.clear();
				throw e;
			}

			return;
		} finally {
			marker = buffer.listIterator();
			if(input != null) {
				input.close();
				input = null;
			}
		}
	}

	/**
	 * Verifica se o marcador de leitura atingiu o fim de arquivo.
	 *
	 * @return
	 * {@code true} caso o método {@link #getchar()} possa ser chamado.
	 */
	public boolean hasNext() {
		return marker.hasNext();
	}

	/**
	 * Obtém o próximo caractere do código-fonte,
	 * avançando o marcador de leitura.
	 *
	 * @return
	 * O próximo caractere lido do código-fonte.
	 *
	 * @throws EOFException
	 * Lançada ao tentar obter o caractere de fim de arquivo.
	 */
	public char getchar() {
		if(marker.hasNext())
			return marker.next().getValue();
		else
			throw new EOFException();
	}

	/**
	 * Retrocede o marcador de leitura de um caractere.
	 * Caso não haja como retroceder, nada é feito.
	 */
	public void ungetchar() {
		if(marker.hasPrevious())
			marker.previous();
		return;
	}

	/**
	 * Obtém o número da linha atual do marcador. A primeira linha
	 * é a linha 0.
	 *
	 * @return
	 * A linha onde o marcador está posicionado.
	 */
	public int getLineNumber() {
		Char c;
		if(marker.hasNext()) {
			//linha do próximo caracter
			c = marker.next();
			marker.previous();
			return c.getLine();
		} else if(marker.hasPrevious()) {
			//fim de arquivo na mesma linha
			c = marker.previous();
			marker.next();
			if(c.getValue() == '\n')
				return c.getLine() + 1;
			else
				return c.getLine();
		} else
			return 0; //há apenas uma linha vazia
	}

	/**
	 * Obtém o número da coluna atual do marcador. A primeira coluna
	 * é a coluna 0.
	 *
	 * @return
	 * A coluna onde o marcador está posicionado.
	 */
	public int getColumnNumber() {
		Char c;
		if(marker.hasNext()) {
			//coluna do próximo caracter
			c = marker.next();
			marker.previous();
			return c.getColumn();
		} else if(marker.hasPrevious()) {
			//fim de arquivo na próxima coluna
			c = marker.previous();
			marker.next();
			if(c.getValue() == '\n')
				return 0;
			else
				return c.getColumn() + 1;
		} else
			return 0; //há apenas uma linha vazia
	}

	//Objeto caractere com informações adicionais
	private static class Char {

		private char value;
		private int line;
		private int column;

		public Char(char value, int line, int column) {
			this.value = value;
			this.line = line;
			this.column = column;
		}

		public char getValue() {
			return value;
		}

		public int getLine() {
			return line;
		}

		public int getColumn() {
			return column;
		}

	}

}
