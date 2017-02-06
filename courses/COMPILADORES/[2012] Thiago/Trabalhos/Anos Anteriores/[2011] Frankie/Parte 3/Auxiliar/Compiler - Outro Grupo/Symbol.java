public class Symbol {
	
	private String id;
	private Type type;
	private Category category;
	private int bytesNumber;
	private int level;
	private ParameterPassingMode transferenceClass;
	private int parametersNumber;
	
	/**
	 * O construtor padrao.
	 */
	public Symbol() {
		
		this.id = null;
		this.type = null;
		this.category = null;
		this.bytesNumber = 0;
		this.level = -1;
		this.transferenceClass = null;
		this.bytesNumber = 0;
	}
	
	/**
	 * Constroi um simbolo com id "id".
	 * 
	 * @param id o "id" do simbolo.
	 */
	public Symbol(String id) {
		
		this.id = id;
		this.type = null;
		this.category = null;
		this.bytesNumber = 0;
		this.level = -1;
		this.transferenceClass = null;
		this.bytesNumber = 0;		
	}
	
	public void setId(String id) {
		
		this.id = id;
	}
	
	public String getId() {
		
		return this.id;
	}
	
	public void setType(Type type) {
		
		this.type = type;
	}

	public Type getType() {
		
		return type;
	}

	public void setCategory(Category category) {
		
		this.category = category;
	}

	public Category getCategory() {
		
		return category;
	}
	
	public void setBytesNumber(int bytesNumber) {
		
		this.bytesNumber = bytesNumber;
	}
	
	public int getBytesNumber() {
		
		return this.bytesNumber;
	}

	public void setLevel(int level) {
		this.level = level;
	}

	public int getLevel() {
		return this.level;
	}

	public void setTransferenceClass(ParameterPassingMode transferenceClass) {
		this.transferenceClass = transferenceClass;
	}

	public ParameterPassingMode getTransferenceClass() {
		return this.transferenceClass;
	}

	public void setParametersNumber(int parametersNumber) {
		
		this.parametersNumber = parametersNumber;
	}

	public int getParametersNumber() {
		
		return this.parametersNumber;
	}
	
	
}
