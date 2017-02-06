public class Sistema {
	public static void main(String args[]) {
		try {
			Pessoa p = new Pessoa("111", "Nome");
			System.out.println(p.getCpf()+" "+p.getNome());
		} catch (CPFLength e01) {
			e01.printStackTrace();
		} catch (CPFInvalid e02) {
			System.out.println(e02.getMessage());
		}
	}
}
