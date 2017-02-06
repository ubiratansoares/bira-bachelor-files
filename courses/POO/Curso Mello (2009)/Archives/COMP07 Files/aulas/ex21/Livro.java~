public class Livro {
	
	// cria uma variavel para a classe
	// se é para a classe, ela é compartilhada
	// entre todas as instâncias (objetos) dessa classe
	public static int codigo_estatico = 0;
	private int codigo;
	private String titulo;
	private String autor;

	public Livro(String titulo, String autor) {
		this.codigo = codigo_estatico++;
		this.titulo = titulo;
		this.autor = autor;
	}

	public void setTitulo(String titulo) { this.titulo = titulo; }
	public void setAutor (String autor) { this.autor = autor; }

	public int getCodigo() { return this.codigo; }
	public String getTitulo() { return titulo; }
	public String getAutor() { return autor; }

	public int getEstatico() { return codigo_estatico; }

	public static void main(String args[]) {

		Livro.codigo_estatico = 10;
		
		Livro l1 = new Livro("titulo 1", "autor 1");
		Livro l2 = new Livro("titulo 2", "autor 2");
		Livro l3 = new Livro("titulo 3", "autor 3");

		System.out.println(l1.getCodigo());
		System.out.println(l2.getCodigo());
		System.out.println(l3.getCodigo());

		System.out.println(l1.getEstatico());
		System.out.println(l2.getEstatico());
		System.out.println(l3.getEstatico());
	}
}
