
import java.awt.Point;
import java.util.*;
/**
 * The ADFGVX code was used by the German durin World War I
 * It is more difficult to deciher than the Playfair code because it uses a double
 * encryption but was deciphered by the French Army Lieutenant Georges Painvin before the end of the war.
 * 
 * All the encoded messages will only contain the letters ADFGVX. Actually any 6 letters would do
 * but ADFGVX were choosen because these letters are very different in Morse code.
 * 
 * When a telegraph operator has to transmit a message like "Hello my dear" he just read the 
 * sentence, can even remember it, and transmit the text "Hello my dear". However when he has
 * to transmit a coded string like "ZWHYW WQ JFGT" it has to read and transmit letter by letter so
 * the translation from letter to Morse is more error prone. Using letters that are quite different 
 * in Morse reduced to possibility of errors.
 * 
 * The Ceasar code uses a number of shifts.
 * The Vigenere and Playfair codes use a Key.
 * This code use a 6 X 6 matrix where are randomly stored the 26 letters of the alphabet and the 10
 * numeric digits. This matrix should be known by both the sender and the receiver.
 * Example:
 * 
 *      A  D  F  G  V  X
 *    +------------------
 *  A | D  6  E  A  M  1
 *  D | 0  I  N  3  C  B
 *  F | T  Y  S  W  Z  9
 *  G | 2  L  Q  O  K  V
 *  V | F  G  8  H  J  P
 *  X | V  X  4  5  R  7
 *  
 *  The first step of the encoding process is to represent every letter/number of the message
 *  by the ADFGVX letter of the row and the column where the letter is located in the matrix so
 *  
 *  H  e  l  l  o    m  y   d  e  a  r
 *  VG AF GD GD GG   AV FD  AA AG AG XV
 *  
 *  OK here nothing new. It is a simple substitution and a frequency analysis would permit to
 *  decipher that code quite fast.
 *  But now, as a second step, the code use a transposition that will make the 
 *  decipher process a lot more complicated.
 *  
 *  A key is used that must also be known by both the sender and the receiver
 *  Let's use JOHN as a key. We first write the key on a line and write the message as coded under it
 *  
 *   J O H N
 *   -------
 *   V G A F    H e
 *   G D G D    l l
 *   G G A V    o m
 *   F D A A    y d
 *   A G A G    e a
 *   X V        r 
 *   
 *   then we put the Key in alphabetical order and re-arrange the columns accordingly
 *   
 *   J O H N  ->   H J N O
 *   -------       -------
 *   V G A F  ->   A V F G
 *   G D G D  ->   G G D D
 *   G G A V  ->   A G V G
 *   F D A A  ->   A F A D
 *   A G A G  ->   A A G G
 *   X V D A  ->     X   V
 *   
 *   The final coded message is produced by reading letter each column in the right table from top to bottom 
 *   So the final message here is:
 *   A G A A A V G G F A X F D V A G G D G D G V
 *   --------- ----------- --------- -----------
 *     Col H     Col J       Col N     Col O 
 *     
 *   Contrary to the other codes previously seen, this one changes the total coded line
 *   when a letter is appended to the original message 
 *  
 */
public class Adfgvx {

	// the letters used that will be transmitted in Morse code
	private static final char[] morse = {'A', 'D', 'F', 'G', 'V', 'X'};
	// the key
	private String key;
	// the Grid used that will be randomly filled
	private char[][] grid;
    // the original columns (unsorted)
	private Column[] col;
	// the Colums sorted by their header
	private Column[] colAlpha;
	
	/**
	 * Constructor that receives the Key as parameter
	 */
	public Adfgvx(String key) {
		// Fill an arrayList with all the characters to put in the grid
		ArrayList<Character> al = new ArrayList<Character>();
		for(char c = 'A'; c <= 'Z'; c++)
			al.add(c);
		for(char c = '0'; c <= '9'; c++)
			al.add(c);
		// create a Random numbers generator to randomly extract the char from the arrayList
		Random ran = new Random();
		// create our Grid 
		grid = new char[morse.length][morse.length];
		// fill it
		for(int i = 0; i < 6; i++) {
			for(int j = 0; j < 6; j++) {
				// generate a random number between 0 and the arrayList size
				int index = ran.nextInt(al.size());
				// remove the char at that index and store it into the grid
				grid[i][j] = al.remove(index);
			}
		}

		// call our method to register the key
		setKey(key);
	}

	/**
	 * Use to set the initial key or to change it later on
	 */
	public void setKey(String key) {
		// if the key is null ignore it
		if(key == null) {
			this.key = "";
			return;
		}
		// convert it into char[] to check if twice the same digit
		char[] digit = key.toCharArray();
		int len = digit.length;
		// the key cannot contain twice the same character
		for(int i = 0; i < len - 1; i++) {
			for(int j = i + 1; j < len; j++) {
				if(digit[i] == digit[j]) {
					this.key = "";
					return;
				}
			}
		}
		// ok key is valid
		this.key = key;
		// build our columns with each charac of the key as header
		col = new Column[len];
		colAlpha = new Column[len];
		for(int i = 0; i < len; i++) {
			// the original columns
			col[i] = new Column(digit[i]);
			// and the ones that will be sorted later (but same column)
			colAlpha[i] = col[i];
		}
		// sort our second serie of columns in alphabetical order
		Arrays.sort(colAlpha);
		
	}

	/**
	 * To encode a message
	 */
	public String encode(String clear) {
        // common method that will chek that the key is valid and that
		// the message is OK. It also suppress illegal characters in the message
		char[] digit = msgToProcess(clear, true);
		if(digit.length == 0)
			return "";
		
		// prepare the columns (we multiply by 2 because we will need two coded letter
		// for evry letter in the original message)
		prepareColumns(digit.length * 2);

		// find the coordinates of each character in the original message
		// and add it row by row in each column
		int k = 0;							// index in the column array
		for(char c : digit) {
			Point p = findPos(c);
			// add the 2 letters
			col[k++].add(morse[p.x]);
			// wrap around at the end of the columns
			k %= col.length;
			col[k++].add(morse[p.y]);
			k %= col.length;
		}
		
		// use a StringBuilder to concatenate the char in each column
		StringBuilder sb = new StringBuilder(digit.length * 2);
		for(Column c : colAlpha) {
			sb.append(c.toString());
		}
		// return the full coded String but with ' ' between every pair
		digit = sb.toString().toCharArray();
		sb = new StringBuilder(digit.length + (digit.length / 2));
		// put the first 2 digits
		sb.append(digit[0]);
		sb.append(digit[1]);
		// then every other pair preceeded by ' '
		for(int i = 2; i < digit.length; i += 2) {
			sb.append(' ');
			sb.append(digit[i]);
			sb.append(digit[i+1]);
		}
		
		return sb.toString();
	}

	/**
	 * Decode the coded String received as parameter
	 */
	public String decode(String coded) {
        // common method that will chek that the key is valid and that
		// the message is OK. It also suppress illegal characters in the message
		char[] digit = msgToProcess(coded, false);
		if(digit.length == 0)
			return "";
		
		// prepare the columns
		prepareColumns(digit.length);
		
		// copy the coded message into each alpha sorted Columns
		int k = 0;			// index in the digit array
		for(Column c : colAlpha) {
			int size = c.getSize();			// number of charac in column
			for(int i = 0; i < size; i++)
				c.add(digit[k++]);   		// append next digit
		}

		// put back as a long string the contain of each original colum
		// row by row
		StringBuilder sb = new StringBuilder(digit.length);
		int size = col[0].getSize();		// the longest one
		// scan all rows
		for(int row = 0; row < size; row++) {
			// scan for that row all column
			for(Column c : col) {
				// if this Column has less row noi need to continue the following
				// ones will be the same
				if(row >= c.getSize())
					break;
				// so append the character at that row
				sb.append(c.getChar(row));
			}
		}
		
		// make a char array of the StringBuilder
		digit = sb.toString().toCharArray();
		// use a another char array for the decoded String 
		char[] decoded = new char[digit.length / 2];
		// pass through our string 2 characters at the time to find the
		// equivalent on the grid
		for(int i = 0; i < digit.length; i += 2) {
			// found the X coordinate in the grid
			int x = 0;
			for(; x < morse.length; x++) {
				if(digit[i] == morse[x])
					break;
			}
			// found the y coordinate in the grid
			int y = 0;
			for(; y < morse.length; y++) {
				if(digit[i+1] == morse[y])
					break;
			}
			// assign the value from the grid
			decoded[i/2] = grid[x][y];
		}
		return new String(decoded).toLowerCase();
	}
	
	/**
	 * For both coding and decoding returns an array of char of the message
	 * to code/decode
	 * The boolean coding is true for encoding and false for decoding
	 * Returns an empty array if the key is not valid
	 */
	private char[] msgToProcess(String str, boolean coding) {
		// if message is null return nothing
		if(str == null)
			return new char[0];
		// if I do not have a valid key return
		if(key.length() == 0)
			return new char[0];
		// keep only valid characters that we will stored in a StringBuilder
		StringBuilder sb = new StringBuilder(key.length());
		// convert to uppercase
		char[] digit = str.toUpperCase().toCharArray();
		// pass through each digit
		for(char c : digit) {
			if(coding) {
				// if encoding if a letter or a digit add it 
				if((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) 
					sb.append(c);
			}
			else {
				// when decoding only the morse letters are permitted
				for(char m : morse) {
					// if letter contained in morse letters
					if(m == c) {
						sb.append(c);
						break;			// no need to continue loop in the morse permitted letters
					}
				}
			}
		}
        // have digit to now be an array of just the valid character
		digit = sb.toString().toCharArray();
		// if empty return it
		if(digit.length == 0)
			return digit;
		// when decoding the number of letter must be even
		if(!coding) {
			if(digit.length % 2 != 0)
				return new char[0];
		}
		// return the array of letters to process
		return digit;
	}
	
	/**
	 * Prepare/initialize the col and colAlpha objects
	 */
	private void prepareColumns(int len) {
		// calculate the number of letters that will be in each column
		int nbPerCol = len / col.length;
		// make an array of these length
		int[] nb = new int[col.length];
		for(int i = 0; i < col.length; i++)
			nb[i] = nbPerCol;
		// now if the message length is not an exact multiple of the message length
		// the first columns will have one more row
		int reminder = len - (col.length * nbPerCol);
		for(int i = 0; i < reminder; i++)
			nb[i]++;
		
		// we can now set the size of each of our column object
		for(int i = 0; i < col.length; i++) {
			col[i].setSize(nb[i]);
		}
	}
	
	/**
	 * Find the position of the character in the grid
	 * the X and Y position will be index in the Morse array to find the ADFGVX letters to
	 * use to represent that digit
	 */
	private Point findPos(char c) {
		// scan the Grid
		for(int x = 0; x < 6; x++) {
			for(int y = 0; y < 6; y++) {
				// if match return the coords
				if(c == grid[x][y])
					return new Point(x, y);
			}
		}
		throw new IllegalStateException("Character " + c + " not found in Grid");
	}
	/**
	 * For debug purpose a method to display the Grid
	 */
	public void dumpGrid() {
		// header
		System.out.println("      GRID");
		System.out.println();
		// gap before printing A D F G V X
		System.out.print("    ");
		// the letters A D F G V X
		for(int i = 0; i < morse.length; i++)
			System.out.print(" " + morse[i]);
		System.out.println();
		// +---------- under the A D F G V X at the top
		System.out.print("  +--");
		for(int i = 0; i < morse.length; i++)
			System.out.print("--");
		System.out.println();
		// now the different row
		for(int i = 0; i < morse.length; i++) {
			// the letter at the beginning of the row
			System.out.print(morse[i] + " | ");
			// the Grid contents for that line
			for(int j = 0; j < morse.length; j++) {
				System.out.print(" " + grid[i][j]);
			}
			// ready for next line
			System.out.println();
		}
	}
	
	/**
	 * For the GUI
	 */
	public char[][] getGrid() {
		return grid;
	}
	public char[] getMorse() {
		return morse;
	}

	/**
	 * To test the class
	 */
	public static void main(String[] args) {
		// test the whole thing
		
		//--------------------------------------------
		// Automatic tests
		//--------------------------------------------
		// create an Adfgvx object with JOHN as Key
		Adfgvx adfgvx = new Adfgvx("JOHN");
		// dump the grid
		adfgvx.dumpGrid();
		// use a message
		String message = "This is the message to encode";
		// code it
		String coded = adfgvx.encode(message);
		System.out.println("Original: " + message);
		System.out.println("   Coded:   " + coded);
		// decode it back
		System.out.println(" Decoded: " + adfgvx.decode(coded));
		//---------------------------------------------
		// end of automatic tests
		//---------------------------------------------
		
		// now proceed with user input
		Scanner scan = new Scanner(System.in);
		System.out.print("\n    Enter a Key: ");
		String key = scan.nextLine();
		Adfgvx user = new Adfgvx(key);
		System.out.print("Enter a message: ");
		String msg = scan.nextLine();
		// dump the grid
		user.dumpGrid();
		System.out.println("  Key is: " + user.key);
		System.out.println("Original: " + msg);
		String cypher = user.encode(msg);
		System.out.println("   Coded: " + cypher);
		System.out.println(" Decoded: " + user.decode(cypher));
	}

	/**
	 * An internal class to hold the data (the character of each column)
	 * it implements comparable so the column could be sorted by alpahbetical order
	 */
	private class Column implements Comparable<Column> {

		// the letter A D F G V X at the head of the column
		private char header;
		// all the letters in the column
		private char[] letters;
		// use when we cumulate the digits in the letters array
		private int index;

		/**
		 * Constructor that receives the letter as header
		 */
		Column(char header) {
			this.header = header;
		}
		/**
		 * To set the number of elements in the column
		 */
		void setSize(int size) {
			// build array to receive all elements
			letters = new char[size];
			// reset that we are at element 0
			index = 0;
		}
		/** 
		 * To return, while decoding, the number of characters to insert in the Column
		 */
		int getSize() {
			return letters.length;
		}
		/**
		 * To add a letter to the column
		 */
		void add(char c) {
			letters[index++] = c;
		}
		/** 
		 * To get a single letter
		 */
		char getChar(int n) {
			return letters[n];
		}
		/**
		 * To return as a String the letters in the column
		 */
		public String toString() {
			return new String(letters);
		}
		
		/**
		 * To sort the columns by header
		 */
		public int compareTo(Column other) {
			return header - other.header;
		}
	}

}

