package rtk.view;

import rtk.template.*;

public class HeadFragment extends PageFragment {
	
	private static final PageTemplate template = 
		new PageTemplate(HeadFragment.class.getResource("HeadFragment.html"));
		
	public HeadFragment() {
		super(template);
	}
	
}
