package com.galpao.app;

import com.galpao.bd.*;
import java.sql.*;

public class Pessoa {
	// conexao com o BD
	private DBConnection db;

	private String telefone;
	private String nome;
	private String rua;
	private String numero;
	private String complemento;
	private String bairro;
	private String cep;

	public Pessoa() throws Exception {
		db = new DBConnection();
	}
	public Pessoa(String telefone, String nome, String rua, 
			String numero, String complemento, 
			String bairro, String cep) throws Exception {
		db = new DBConnection();

		this.telefone = telefone;
		this.nome = nome;
		this.rua = rua;
		this.numero = numero;
		this.complemento = complemento;
		this.bairro = bairro;
		this.cep = cep;
	}

	public void setTelefone(String telefone) { this.telefone = telefone; }
	public void setNome(String nome) {this.nome = nome; }
	public void setRua(String rua) {this.rua = rua; }
	public void setNumero(String numero) {this.numero = numero; }
	public void setComplemento(String complemento) {
		this.complemento = complemento;
	}
	public void setBairro(String bairro) {this.bairro = bairro; }
	public void setCep(String cep) {this.cep = cep; }

	public String getTelefone() {  return this.telefone; }
	public String getNome() { return this.nome; }
	public String getRua() { return this.rua; }
	public String getNumero() { return this.numero; }
	public String getComplemento() {
		 return this.complemento;
	}
	public String getBairro() { return this.bairro; }
	public String getCep() { return this.cep; }

	public void insert() throws Exception {
		// insert no BD
		db.execute("insert into pessoa (telefone, nome, rua, numero, complemento, bairro, cep) values ('"+this.telefone+"', '"+this.nome+"', '"+this.rua+"', '"+this.numero+"', '"+this.complemento+"', '"+this.bairro+"', '"+this.cep+"')");
	}

	public void update() throws Exception {
		// update no BD
		db.execute("update into pessoa set nome = '"+this.nome+"', rua = '"+this.rua+"', numero = '"+this.numero+"', complemento = '"+this.complemento+"', bairro = '"+this.bairro+"', cep = '"+this.cep+"' where telefone = '"+this.telefone+"'");
	}

	public void remove() throws Exception {
		// remove no BD
		db.execute("delete from pessoa where telefone = '"+this.telefone+"'");
	}

	public ResultSet findAll() throws Exception {
		// select do BD
		return null;
	}
}
