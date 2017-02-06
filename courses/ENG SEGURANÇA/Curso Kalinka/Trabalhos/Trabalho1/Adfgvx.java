// Adfgvx.java
// Esse programa eh uma implementacao simples da cifra Adfgvx, utilizada na Primeira Guerra Mundial.
// Trata-se de uma cifra duplamente cifrada, na qual a mensagem cifrada somente conterah as letra ADFGVX.
// Essas letras foram as escolhidas para compor a mensagem criptografa pois sao muito diferente em codigo Morse.
// Maiores detalhes internos da cifra estao no relatorio que acompanha esse codigo-fonte

import java.awt.Point;
import java.util.*;


public class Adfgvx {

	// Letras usadas no Codigo morse, que dao origem ao nome da cifra
	private static final char[] morse = {'A', 'D', 'F', 'G', 'V', 'X'};
	
	private String key;
	private char[][] grid;
    private Column[] col;
	private Column[] colAlpha;

	// Construtor padrao
	// Aceita a chavw como parametro e gera uma nova matrix Adfgvx
	
	public Adfgvx(String key) {
		ArrayList<Character> al = new ArrayList<Character>();
	
		for(char c = 'A'; c <= 'Z'; c++) al.add(c);
		for(char c = '0'; c <= '9'; c++) al.add(c);

		Random ran = new Random();

		// Criando a matriz de cifragem Adfgvx 

		grid = new char[morse.length][morse.length];


		for(int i = 0; i < 6; i++) {
			for(int j = 0; j < 6; j++) {

				// Gera um numero aleatorio entre 0 e o tamanho do arrayList
				int index = ran.nextInt(al.size());

				// Remove o caracter indexado do array e armazena na matrix
				grid[i][j] = al.remove(index);
			}
		}

		setKey(key);
	}

	public void setKey(String key) {
	
		if(key == null) {
			this.key = "";
			return;
		}
		
		char[] digit = key.toCharArray();
		int len = digit.length;
		
		// Fazendo a checagem de letras repetidas na chave
		for(int i = 0; i < len - 1; i++) {

			for(int j = i + 1; j < len; j++) {

				if(digit[i] == digit[j]) {
					this.key = "";
					return;
				}
			}
		}
		
		// A chave eh valida
		this.key = key;

		// Construindo as colunas com as letras da chave como header
		col = new Column[len];
		colAlpha = new Column[len];
		
		for(int i = 0; i < len; i++) {
			col[i] = new Column(digit[i]);
			colAlpha[i] = col[i];
		}
		Arrays.sort(colAlpha);
		
	}

	// Metodo para cifrar a mensagem

	public String encode(String clear) {

		char[] digit = msgToProcess(clear, true);

		if(digit.length == 0) return "";
		
		prepareColumns(digit.length * 2);

		// Encontrando as coordenadas de cada caracter da mensagem original
		// e adicionando nas linhas de cada coluna

		int k = 0;
									
		for(char c : digit) {
		
			Point p = findPos(c);
			col[k++].add(morse[p.x]);
			k %= col.length;
			col[k++].add(morse[p.y]);
			k %= col.length;
		}
		
		StringBuilder sb = new StringBuilder(digit.length * 2);
		for(Column c : colAlpha) {
			sb.append(c.toString());
		}
		
		digit = sb.toString().toCharArray();
		sb = new StringBuilder(digit.length + (digit.length / 2));
		
		sb.append(digit[0]);
		sb.append(digit[1]);
		
		for(int i = 2; i < digit.length; i += 2) {
			sb.append(' ');
			sb.append(digit[i]);
			sb.append(digit[i+1]);
		}
		
		return sb.toString();
	}

	// Metodo para decifrar a mensagem
	
	public String decode(String coded) {
    
		char[] digit = msgToProcess(coded, false);
		
		if(digit.length == 0) return "";
		
		prepareColumns(digit.length);
		
		int k = 0;		

		for(Column c : colAlpha) {
			int size = c.getSize();		
			for(int i = 0; i < size; i++)
				c.add(digit[k++]);   		
		}

		StringBuilder sb = new StringBuilder(digit.length);

		int size = col[0].getSize();	

		for(int row = 0; row < size; row++) {
			for(Column c : col) {

				if(row >= c.getSize()) break;
				sb.append(c.getChar(row));
			}
		}
		
		digit = sb.toString().toCharArray();

		char[] decoded = new char[digit.length / 2];

		for(int i = 0; i < digit.length; i += 2) {

			int x = 0;
			for(; x < morse.length; x++) {
				if(digit[i] == morse[x])
					break;
			}

			int y = 0;
			for(; y < morse.length; y++) {
				if(digit[i+1] == morse[y])
					break;
			}
			
			decoded[i/2] = grid[x][y];
		}
		
		return new String(decoded).toLowerCase();
	}
	

	
	// Metodo para retornar um array de char para a mensagem a ser cifrada/decifrada

	private char[] msgToProcess(String str, boolean coding) {

		if(str == null)	return new char[0];
		
		if(key.length() == 0)return new char[0];

		StringBuilder sb = new StringBuilder(key.length());

		char[] digit = str.toUpperCase().toCharArray();

		for(char c : digit) {
			if(coding) {

				if((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) 
					sb.append(c);
			}
			else {

				for(char m : morse) {
					if(m == c) {
						sb.append(c);
						break;		
					}
				}
			}
		}
		
        digit = sb.toString().toCharArray();
		
		if(digit.length == 0)
			return digit;
		
		if(!coding) {
			if(digit.length % 2 != 0)
				return new char[0];
		}
		
		return digit;
	}
	
	// Inicializando os objetos auxiliares col e colAlpha
	
	private void prepareColumns(int len) {
	
		int nbPerCol = len / col.length;
	
		int[] nb = new int[col.length];
	
		for(int i = 0; i < col.length; i++)
			nb[i] = nbPerCol;
	
		int reminder = len - (col.length * nbPerCol);
	
		for(int i = 0; i < reminder; i++)
			nb[i]++;
		
		// we can now set the size of each of our column object
		for(int i = 0; i < col.length; i++) {
			col[i].setSize(nb[i]);
		}
	}
	
	
	// Metodo para encontrar a posicao do caracter na matrix, sendo q as coordenada X e Y serao os indices no vetor
	// das letras do codigo Morse
	
	private Point findPos(char c) {
	
		for(int x = 0; x < 6; x++) {
			for(int y = 0; y < 6; y++) {
	
				if(c == grid[x][y])
					return new Point(x, y);
			}
		}
	
		throw new IllegalStateException("Character " + c + " not found in Grid");
	}
	
	// Imprime a matrix

	public void dumpGrid() {

		System.out.println("      GRID");

		System.out.println();

		System.out.print("    ");

		for(int i = 0; i < morse.length; i++)
			System.out.print(" " + morse[i]);

		System.out.println();

		System.out.print("  +--");

		for(int i = 0; i < morse.length; i++)
			System.out.print("--");

		System.out.println();

		for(int i = 0; i < morse.length; i++) {

			System.out.print(morse[i] + " | ");

			for(int j = 0; j < morse.length; j++) {
				System.out.print(" " + grid[i][j]);
			}
			System.out.println();
		}
	}
	
	public static void main(String[] args) {

		Scanner scan = new Scanner(System.in);

		System.out.print("\n    Enter a Key: ");

		String key = scan.nextLine();

		Adfgvx user = new Adfgvx(key);

		System.out.print("\nEnter a message: ");

		String msg = scan.nextLine();
		
		user.dumpGrid();

		System.out.println("  \nKey is: " + user.key);

		System.out.println("Original: " + msg);

		String cypher = user.encode(msg);

		System.out.println("   \nCoded: " + cypher);

		System.out.println(" Decoded: " + user.decode(cypher));
	}

	// Classe interna para representar as colunas de header 

	private class Column implements Comparable<Column> {

		private char header;

		private char[] letters;
		
		private int index;

		Column(char header) {
			this.header = header;
		}
		
		void setSize(int size) {
		
			letters = new char[size];
			index = 0;
		}
		
		int getSize() {
			return letters.length;
		}
		
		void add(char c) {
			letters[index++] = c;
		}
		
		char getChar(int n) {
			return letters[n];
		}
		
		public String toString() {
			return new String(letters);
		}
		
		public int compareTo(Column other) {
			return header - other.header;
		}
	}

}

