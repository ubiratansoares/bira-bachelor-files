package compiler.t3b;

/**
 * Instrução da MaqHipo.
 *
 */
public class Instruction {

	private Code code;
	private Number argument;

	/**
	 * Cria uma instrução sem argumento.
	 *
	 * @param code
	 * O código da instrução.
	 */
	public Instruction(Code code) {
		this.code = code;
		this.argument = 0;
	}

	/**
	 * Cria uma instrução com código e argumento.
	 *
	 * @param code
	 * O código da instrução.
	 *
	 * @param arg
	 * O argumento da instrução.
	 */
	public Instruction(Code code, Number arg) {
		this.code = code;
		this.argument = arg;
	}

	/**
	 * Redefine a instrução.
	 *
	 * @param code
	 * O novo código da instrução.
	 *
	 * @param arg
	 * O novo argumento da instrução.
	 */
	public void reset(Code code, Number arg) {
		this.code = code;
		this.argument = arg;
		return;
	}

	/**
	 * Obtém o código da instrução.
	 *
	 * @return
	 * O código da instrução.
	 */
	public Code getCode() {
		return code;
	}

	/**
	 * Define o valor do argumento da instrução.
	 *
	 * @param value
	 * O valor do argumento.
	 */
	public void setArgument(Number value) {
		this.argument = value;
	}

	/**
	 * Obtém o valor do argumento da instrução.
	 *
	 * @return
	 * O valor do argumento da instrução.
	 */
	public int getIntegerArgument() {
		return argument.intValue();
	}

	/**
	 * Obtém o valor do argumento da instrução.
	 *
	 * @return
	 * O valor do argumento da instrução.
	 */
	public double getRealArgument() {
		return argument.doubleValue();
	}

	/**
	 * Retorna a instrução do código-objeto,
	 * com seu argumento, caso exista.
	 * 
	 * @return
	 * A instrução, como será escrita no arquivo.
	 */
	@Override
	public String toString() {
		switch(code) {
			//instruções com argumento
			case LDC: case LDV: case LDP: case STR:
			case JMP: case JPF: case JPP:
			case MALLOC: case FREE: case PRAD:
				return code + " " + argument;

			//instruções sem argumento
			default:
				return code.toString();
		}
	}

	/**
	 * Instrução do código-objeto.
	 */
	public static enum Code {
		//load, store
		LDC("CRCT"), LDV("CRVL"), LDP("PARAM"), STR("ARMZ"),

		//aritméticos, lógicos, relacionais
		ADD("SOMA"), SUB("SUBT"), MLT("MULT"), DIV("DIVI"), 
		INV("INVE"), NOT("NEGA"), AND("CONJ"), OR("DISJ"),
		CPLT("CPME"), CPGT("CPMA"), CPLE("CPMI"), CPGE("CMAI"),
		CPEQ("CPIG"), CPNE("CDES"), 
		
		//desvios
		JMP("DSVI"), JPF("DSVF"), JPP("CHPR"), JPR("RTPR"),

		//read, write
		READ("LEIT"), WRITE("IMPR"),

		//programa principal
		START("INPP"), STOP("PARA"), 
		MALLOC("ALME"), FREE("DESM"),

		//procedimentos
		PRAD("PUSHER"), BIND("COPVL");

		private String code;

		private Code(String code) {
			this.code = code;
		}

		/**
		 * Retorna a instrução do código-objeto.
		 *
		 * @return
		 * A instrução do código-objeto.
		 */
		@Override
		public String toString() {
			return code;
		}
	}

}
