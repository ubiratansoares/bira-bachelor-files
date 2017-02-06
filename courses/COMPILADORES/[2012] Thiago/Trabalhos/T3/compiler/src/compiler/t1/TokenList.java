package compiler.t1;

import compiler.*;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.Writer;

import java.util.List;
import java.util.LinkedList;

import java.io.IOException;

/**
 * Uma lista de tokens.
 *
 * @version 1.0.2
 */
public class TokenList implements ObjectCode {

	private List<Token> tokens;

	/**
	 * Cria uma lista vazia.
	 */
	public TokenList() {
		tokens = new LinkedList<Token>();
	}

	/**
	 * Adiciona o token ao final da lista.
	 *
	 * @param t
	 * O token a ser adicionado.
	 */
	public void append(Token t) {
		tokens.add(t);
		return;
	}

	/**
	 * Escreve o conteúdo da lista na saída padrão.
	 */
	public void println() {
		System.out.println("[inicio]");
		for(Token t : tokens)
			System.out.printf("%s - %s%n", t.getString(), t.getType());
		System.out.println("[fim]");
		return;
	}

	/**
	 * Escreve o conteúdo da lista no arquivo especificado.
	 * 
	 * @param filename
	 * O nome do arquivo a ser escrito.
	 *
	 * @throws IOException
	 * Lançada caso ocorra qualquer problema na escrita do arquivo.
	 */
	public void writeFile(String filename) throws IOException {
		Writer stream = new BufferedWriter(new FileWriter(filename));
		PrintWriter out = new PrintWriter(stream);

		for(Token t : tokens)
			out.printf("%s - %s%n", t.getString(), t.getType());

		out.close();
		return;
	}

}
