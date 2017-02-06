package servico;

import java.util.*;

import org.apache.axis.client.Service;
import org.apache.axis.client.Call;
import org.apache.axis.utils.Options;

import javax.xml.namespace.QName;
import javax.xml.rpc.ParameterMode;
 
public class Cliente {
     public static void main(String[] args) throws Exception {

		// Endereço, local onde encontra-se o Web Service
		String local = "http://localhost:8080/axis/Servico02.jws";
		 
		// Criando e configurando o serviço
		Call call = (Call) new Service().createCall();
		// Configurando o endereço.
		call.setTargetEndpointAddress(local);
	       	// Marcando o método a ser chamado. 
		call.setOperationName("listItems");

		// Parâmetros da função soma.
		Object[] param = new Object[]{new String("http://rss.slashdot.org/Slashdot/slashdot")};
	       	// Retorno da Função
		call.invoke(param);
	}
}
