package compiler.lexer;

import compiler.Token;

/**
 * Estado de dois-pontos lidos do buffer.
 *
 */
public class ColonState extends State {

	/**
	 * Próximos estados:
	 * {@link AssignState},
	 * {@link ColonEndState}.
	 *
	 * @return
	 * O próxmo estado.
	 */
	@Override
	public State next(char c) {
		if(c == '=')
			return AssignState.getInstance();
		else
			return ColonEndState.getInstance();
	}

	/**
	 * Adiciona o caractere à cadeia, se for um '='. 
	 * Caso contrário, não altera a cadeia.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		if(c == '=')
			sb.append(c);
		return;
	}

	/**
	 * Retorna o token de dois-pontos.
	 *
	 * @return
	 * Um token de dois-pontos.
	 */
	@Override
	public compiler.Token getToken(String str) {
		return Token.createSymbol(str);
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
	private ColonState() {
	}

	private static final State SINGLETON = new ColonState();

}
