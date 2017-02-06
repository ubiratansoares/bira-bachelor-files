package org.usp.exemplo01ws;

import java.util.*;
import org.apache.axis.client.Service;
import org.apache.axis.client.Call;
 
public class Cliente {
     public static void main(String[] args) throws Exception {
		String local = "http://localhost:8080/axis/User.jws";
		 
		Call call = (Call) new Service().createCall();
		call.setTargetEndpointAddress(local);
		call.setOperationName("insert");
		Object[] param = new Object[]{
			new String("nome1"), 
			new String("login1"), 
			new String("passwd1")};

		Integer ret = (Integer) call.invoke(param);
		System.out.println("Retorno = "+ret);
	}
}
