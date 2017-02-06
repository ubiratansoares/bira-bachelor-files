package org.usp.robotwsmapped;

import java.util.*;
import org.usp.news.*;

import org.apache.axis.client.Service;
import org.apache.axis.client.Call;
import org.apache.axis.utils.Options;

import javax.xml.namespace.QName;
import javax.xml.rpc.ParameterMode;
 
public class Cliente {
     public static void main(String[] args) throws Exception {
		// Endereço, local onde encontra-se o Web Service
		Options options = new Options(args);
		 
		// Criando e configurando o serviço
		Call call = (Call) new Service().createCall();

        	QName qn = new QName( "urn:Robot", "RSS" );
        	call.registerTypeMapping(RSS.class, qn,
              		new org.apache.axis.encoding.ser.BeanSerializerFactory(
				RSS.class, qn),        
              		new org.apache.axis.encoding.ser.BeanDeserializerFactory(
				RSS.class, qn));        

		// Configurando o endereço.
		call.setTargetEndpointAddress(new java.net.URL(options.getURL()));
	       	// Marcando o método a ser chamado. 
		call.setOperationName( new QName ("Robot", "getNews") );
	       
		// Parâmetros da função soma.
		Object[] param = new Object[]{new String("http://rss.slashdot.org/Slashdot/slashdot")};
	       	// Retorno da Função
		Vector ret = (Vector)call.invoke(param);
		 
		for (int i = 0; i < ret.size(); i++) {
			RSS rss = (RSS) ret.elementAt(i);
			System.out.println("title = "+rss.getTitle());
			System.out.println("link = "+rss.getLink());
			System.out.println("description = "+rss.getDescription());
			System.out.println();

			rss.insert();
		}
	}
}
