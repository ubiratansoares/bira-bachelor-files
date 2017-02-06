package controller;

/**
 * Exceção indica problemas na execução do 
 * controle relacionadas a logica de negócio.
 */
public class ControllerException extends Exception {

	private Enum<?> result;
	
	public ControllerException(Enum<?> result) {
		this.result = result;
	}

	public ControllerException(Enum<?> result, String msg) {
		super(msg);
		this.result = result;
	}

	public Enum<?> getResult() {
		return result;
	}
	
	private static final long serialVersionUID = 1L;

}
