package org.usp.math;

import java.util.*;
import org.apache.axis.client.Service;
import org.apache.axis.client.Call;
 
public class Cliente {
     public static void main(String[] args) throws Exception {
		String local = "http://localhost:8080/axis/Math.jws";
		 
		Call call = (Call) new Service().createCall();
		call.setTargetEndpointAddress(local);
		call.setOperationName("soma");

		Object[] param = new Object[]{
			new Integer(5), 
			new Integer(6) };

		Integer ret = (Integer) call.invoke(param);

		System.out.println("Retorno = "+ret);
	}
}
