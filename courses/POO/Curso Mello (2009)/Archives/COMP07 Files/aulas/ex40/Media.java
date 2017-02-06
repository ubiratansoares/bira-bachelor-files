public class Media extends Item {
	
	private int time;
	private int tracks;
	private String company;

	public Media(String title, int qtt, int availability,
			int time, int tracks, String company) {
		super (title, qtt, availability);
		this.time = time;
		this.tracks = tracks;
		this.company = company;
	}

	public void setTime(int time) { this.time = time; }
	public void setTracks(int tracks) { this.tracks = tracks; }
	public void setCompany(String company) { this.company = company; }

	public int getTime() { return this.time; }
	public int getTracks() { return this.tracks; }
	public String getCompany() { return this.company; }

	public void print() {
		super.print();
		System.out.println("MEDIA "+this.time+", "+
				this.tracks+", "+
				this.company);
	}
}
