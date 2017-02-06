package rtk.template;

import java.util.*;

/**
 * Fragmento de página vazio.
 */
public final class EmptyFragment extends PageFragment {
	
	private static final PageTemplate template = 
			new PageTemplate();

	public EmptyFragment() {
		super(template);
	}
	
}
