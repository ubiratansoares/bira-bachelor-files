import java.net.*;

public interface Component {
	// interfaces existem em Java
	// para resolver o problema de heranca multipla
	//
	// DEFINICAO: eh similar a uma classe abstrata pura
	
	//public int INSERT = 0; // QUALQUER VARIAVEL DECLARADA NA INTERFACE VIRA CONSTANTE

	public void run(Socket s) throws Exception;
}
