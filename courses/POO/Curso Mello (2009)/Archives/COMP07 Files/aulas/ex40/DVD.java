public class DVD extends Media {
	private String region;

	public DVD(String title, int qtt, int availability,
			int time, int tracks, String company,
			String region) {
		super(title, qtt, availability, time, tracks, company);
		this.region = region;

	}

	public void setRegion(String region) { this.region = region; }
	public String getRegion() { return this.region; }

	public void print() {
		super.print();
		System.out.println("DVD "+this.region);
	}
}
