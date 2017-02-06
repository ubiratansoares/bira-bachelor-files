package rtk.template;

import java.util.*;

public class BlockElement extends TemplateElement {
	private List<TemplateElement> elements;
	
	public BlockElement(Type type, String id, 
			List<TemplateElement> elements) {
		super(type, id);
		
		//recusar tipos que não sejam blocos
		switch(type) {
			case TEXT:	case VAR:	case URI:
			case END:	case HEAD:	
				throw new IllegalArgumentException("The instruction is not a block.");
				
			default:
				break;
		}
		
		this.elements = new LinkedList<TemplateElement>(elements);
	}
	
	public List<TemplateElement> getElements() {
		return Collections.unmodifiableList(elements);
	}
	
	@Override
	public String toString() {
		return String.format("%s:\"%s\":%s", this.getType(), 
				this.getValue(), this.elements);
	}
	
}
