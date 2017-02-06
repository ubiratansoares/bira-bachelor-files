public class Item extends Object {
	private static int static_code = 0; // proximo codigo do item

	private int code;
	private String title;
	private int qtt;
	private int availability;

	public Item(String title, int qtt, int availability) {
		this.code = static_code++;
		this.setTitle(title);
		this.setQtt(qtt);
		this.setAvailability(availability);
	}

	public void setTitle (String title) { this.title = title; }
	public void setQtt (int qtt) { this.qtt = qtt; }
	public void setAvailability (int availability) {
		this.availability = availability; 
	}

	public int getCode() { return this.code; }
	public String getTitle() { return this.title; }
	public int getQtt() { return this.qtt; }
	public int getAvailability() { return this.availability; }

	// VIRTUAL
	public void print() {
		System.out.println("ITEM "+this.code+", "+
				this.title+", "+
				this.qtt+", "+
				this.availability);
	}
}
