import java.util.Hashtable;
import java.util.Enumeration;
import java.util.*;
import java.io.*;

public class symbolTable {

	private Hashtable<String,symbol> hashtable; 
	
	public symbolTable()
	{
		this.setHashtable(new Hashtable<String, symbol>());
		setPrimitive();
	}
	
	public symbol busca(String identificador)
	{
		return this.hashtable.get(identificador);
	}
	
	public void elimina(int nivel)
	{
		Enumeration<symbol> e = this.hashtable.elements();
		while (e.hasMoreElements()) {
			symbol s = e.nextElement();
			if (s.getNivel() == nivel)
				this.hashtable.remove(s.getIdentificador());
		}
	}
	
	public symbol insere(String identificador)
	{	
		
			symbol s = new symbol();
			s.setIdentificador(identificador);
			
			if (this.hashtable.containsKey(identificador))
				return null;
			
			this.hashtable.put(identificador,s);
			return s;
		
	}
	
	public boolean declarado(String identificador, int nivel)
	{
		symbol s = this.hashtable.get(identificador);
		if (s == null)
			return false;
		if (s.getNivel() == nivel)
			return true;
		else
			return false;
		
	}
	
	public void setaAtributos(symbol s, String categoria, String tipo, Integer nBytes, 
			Integer nParametros, int nivel, ArrayList<symbol> listaParametros,
			Integer endereco, Integer valor, String classeTransferencia)
	{
		s.setCategoria(categoria);
		s.setTipo(tipo);
		s.setNBytes(nBytes);
		s.setNParametros(nParametros);
		s.setNivel(nivel);
		if (listaParametros == null)
			s.setListaParametros(new ArrayList<symbol>());
		else
			s.setListaParametros(listaParametros);
		s.setEndereco(endereco);
		s.setValor(valor);
		s.setClasseTransferencia(classeTransferencia);
		this.hashtable.put(s.getIdentificador(),s);
	}
	
	public symbol obtemAtributos(symbol s)
	{
		return s;
	}

	public void setHashtable(Hashtable<String,symbol> hashtable)
	{
		this.hashtable = hashtable;
	}

	public Hashtable<String,symbol> getHashtable()
	{
		return hashtable;
	}
	
	public void setPrimitive()
	{
		symbol s = new symbol();
		ArrayList<symbol> p = new ArrayList<symbol>();
		s.setClasseTransferencia("valor");
		p.add(s);
		
		/*Integer*/
		s = new symbol();
		s.setIdentificador("integer");
		s.setCategoria("tipo");
		s.setNBytes(4);
		s.setNivel(0);
		s.setTipo("integer");
		this.hashtable.put(s.getIdentificador(),s);
		
		/*Boolean*/
		s = new symbol();
		s.setIdentificador("boolean");
		s.setCategoria("tipo");
		s.setNBytes(1);
		s.setNivel(0);
		s.setTipo("boolean");
		this.hashtable.put(s.getIdentificador(), s);
		
		/*True*/
		s = new symbol();
		s.setIdentificador("true");
		s.setCategoria("const");
		s.setNBytes(1);
		s.setNivel(0);
		s.setValor(1);
		s.setTipo("boolean");
		this.hashtable.put(s.getIdentificador(), s);

		/*False*/
		s = new symbol();
		s.setIdentificador("false");
		s.setCategoria("const");
		s.setNBytes(1);
		s.setNivel(0);
		s.setValor(0);
		s.setTipo("boolean");
		this.hashtable.put(s.getIdentificador(), s);

		/*Read*/
		s = new symbol();
		s.setIdentificador("read");
		s.setCategoria("proc");
		s.setNivel(1);
		s.setNParametros(1);
		s.setListaParametros(p);
		this.hashtable.put(s.getIdentificador(), s);
		
		/*Write*/
		s = new symbol();
		s.setIdentificador("write");
		s.setCategoria("proc");
		s.setNivel(0);
		s.setNParametros(1);
		s.setListaParametros(p);
		this.hashtable.put(s.getIdentificador(), s);
	}
	
}
