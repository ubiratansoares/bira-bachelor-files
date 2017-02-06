package compiler.lexer;

import compiler.LexicalException;

/**
 * Estado de símbolo inválido. Alguns símbolos podem não
 * pertencer ao alfabeto da linguagem.
 *
 */
public class InvalidSymbolState extends StopState {

	/**
	 * Lança uma exceção.
	 *
	 * @return
	 * Não retorna.
	 *
	 * @throws LexicalException
	 * Indica que o símbolo lido é inválido
	 * (não pertence ao alfabeto da linguagem).
	 */
	@Override
	public compiler.Token getToken(String str) throws LexicalException {
		throw new LexicalException(str, "Símbolo não reconhecido.");
	}

	/**
	 * Obtém uma instância desta classe.
	 *
	 * @return
	 * A instância desta classe.
	 */
	public static State getInstance() {
		return SINGLETON;
	}

	//oculta o construtor padrão
	private InvalidSymbolState() {
	}

	private static final State SINGLETON = new InvalidSymbolState();

}
