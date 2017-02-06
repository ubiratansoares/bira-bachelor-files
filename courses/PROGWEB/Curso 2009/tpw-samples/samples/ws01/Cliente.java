import org.apache.axis.client.Service;
import org.apache.axis.client.Call;
 
public class Cliente {
     public static void main(String[] args) throws Exception {
		// Endereço, local onde encontra-se o Web Service
		String local = "http://localhost:8080/axis/Servico.jws";
		 
		// Criando e configurando o serviço
		Call call = (Call) new Service().createCall();
		// Configurando o endereço.
		call.setTargetEndpointAddress(local);
	       	// Marcando o método a ser chamado. 
		call.setOperationName("soma");
	       
		// Parâmetros da função soma.
		Object[] param = new Object[]{new Integer(2),new Integer(4)};
	       	// Retorno da Função
		Integer ret = (Integer)call.invoke(param);
		 
		// Imprime o resultado: ret = 2 + 4.
		System.out.println("Resultado da soma : " + ret);
	}
}
