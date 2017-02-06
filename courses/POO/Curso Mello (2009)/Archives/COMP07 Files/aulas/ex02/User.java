public class User {
	private String id;
	private String name;

	public void setId(String v) { id = v; }
	public void setName(String v) { name = v; }

	public String getId() { return id; }
	public String getName() { return name; }

	public static void main(String args[]) {
		User u = new User();
		u.setId("13145634512");
		u.setName("Teste User");
		System.out.println(u.getId()+", "+u.getName());

		Cell c = new Cell();
		c.setNumber("6776898");
		c.setCompany(0);
		System.out.println(c.getNumber()+", "+c.getCompany());
	}
}
