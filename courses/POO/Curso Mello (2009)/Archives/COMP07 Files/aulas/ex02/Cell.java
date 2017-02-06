public class Cell {
	private String number;
	private int company;

	public void setNumber(String v) { 
		if (v.length() > 5 && v.length() < 8) 
			number = v; 
	}
	public void setCompany(int v) { company = v; }

	public String getNumber() { return number; }
	public int getCompany() { return company; }

	public static void main(String args[]) {
		Cell c = new Cell();
		c.setNumber("6776898");
		c.setCompany(0);
		System.out.println(c.getNumber()+", "+c.getCompany());
	}
}
