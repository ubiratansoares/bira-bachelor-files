package compiler.lexer;

/**
 * Estado de parada. O autômato deve parar de consumir símbolos ao atingir um 
 * estado desse tipo. Então o autômato pode identificar um token como válido
 * ou não.
 *
 */
public abstract class StopState extends State {

	/**
	 * Não faz nada.
	 */
	@Override
	public void updateString(char c, StringBuilder sb) {
		return;
	}

	/**
	 * Retorna {@code null}. Estados de parada não devem ter
	 * próximos estados.
	 *
	 * @param c
	 * Não utilizado.
	 *
	 * @return
	 * Apenas {@code null}.
	 */
	@Override
	public State next(char c) {
		return null;
	}

}
