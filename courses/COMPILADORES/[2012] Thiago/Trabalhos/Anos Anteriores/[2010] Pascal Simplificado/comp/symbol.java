import java.lang.String;
import java.util.*;
import java.io.*;

public class symbol 
{
	private String identificador;
	private String categoria;
	private String tipo;
	private Integer nBytes;
	private Integer nParametros;
	private int nivel;
	private ArrayList<symbol> listaParametros; 
	private Integer endereco;
	private Integer valor;
	private String classeTransferencia;
	
	public symbol()
	{
		this.categoria = null;
		this.tipo = null;
		this.nBytes = null;
		this.nParametros = null;
		this.nivel = 0;
		this.listaParametros = new ArrayList<symbol>();
		this.endereco = null;
		this.valor = null;
		this.classeTransferencia = null;
	}
	
	public void setIdentificador(String identificador) {
		this.identificador = identificador;
	}

	public String getIdentificador() {
		return identificador;
	}
	
	public void setCategoria(String categoria){
		this.categoria = categoria;
	}
	
	public String getCategoria(){
		return this.categoria;
	}
	
	public void setTipo(String tipo)
	{
		this.tipo = tipo;
	}
	
	public String getTipo(){
		return this.tipo;
	}
	
	public void setNBytes(Integer nBytes)
	{
		this.nBytes = nBytes;
	}
	
	public Integer getNBytes(){
		return this.nBytes;
	}
	
	public void setNParametros(Integer nParametros){
		this.nParametros = nParametros;
	}
	
	public Integer getNParametros(){
		return this.nParametros;
	}
	
	public void setNivel(int nivel){
		this.nivel = nivel;
	}
	
	public int getNivel(){
		return this.nivel;
	}
	
	public void setListaParametros(ArrayList<symbol> listaParametros){
		if (listaParametros == null)
			this.listaParametros = new ArrayList<symbol>();
		else
			this.listaParametros = listaParametros;
	}
	
	public ArrayList<symbol> getListaParametros(){
		return this.listaParametros;
	}

	public void setEndereco(Integer endereco) {
		this.endereco = endereco;
	}

	public Integer getEndereco() {
		return endereco;
	}

	public void setValor(Integer valor) {
		this.valor = valor;
	}

	public Integer getValor() {
		return valor;
	}

	public void setClasseTransferencia(String classeTransferencia) {
		this.classeTransferencia = classeTransferencia;
	}

	public String getClasseTransferencia() {
		return classeTransferencia;
	}

}
