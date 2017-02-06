import java.util.Vector;
import java.io.*;

public class Biblioteca {

	private Vector books;
	private Vector users;

	public Biblioteca() {
		books = new Vector();
		users = new Vector();
	}

	// Book
	public void addBook() throws Exception {
		String isbn, title, author;

		BufferedReader br = 
			new BufferedReader(new InputStreamReader(System.in));

		System.out.println("Digite os dados do Livro");
		System.out.print("Isbn: ");
		isbn = br.readLine();
		System.out.print("Title: ");
		title = br.readLine();
		System.out.print("Author: ");
		author = br.readLine();
		
		Book b = new Book(isbn, title, author);

		books.add(b);
	}

	public void listBooks() {
		for (int i = 0; i < books.size(); i++) {
			Book b = (Book) books.elementAt(i);
			System.out.println(b.getIsbn()+", "+
					b.getTitle()+", "+
					b.getAuthor());
		}
	}

	public static void main(String args[]) throws Exception {
		Biblioteca b = new Biblioteca();
		b.addBook();
		b.addBook();
		b.addBook();
		b.listBooks();
	}
}
