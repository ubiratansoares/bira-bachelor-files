package org.usp.stochastic;

import org.apache.commons.math.distribution.*;
import java.util.*;

public class ComponentSimulation{
	
	private ArrayList<Double> simulatedFailures = new ArrayList<Double>();
	private double mean = 60;
	private double hours = 8760.0;
	private double accumulated = 0.0;
	
	ComponentSimulation() throws Exception{
		
			ExponentialDistributionImpl exp = new ExponentialDistributionImpl(mean);
		
			while(accumulated < hours){
			
				try{
					double rexp = exp.sample();
					simulatedFailures.add(rexp);
					accumulated += rexp;
				}
				catch(Exception e){ 
					System.out.println("\n\nERROR : CANT GENERATE DISTRIBUTION!\n\n"); 
				}
					
			} //end while
	
	}//end Constructor
	
	public ArrayList<Double> getSimulation(){
		
		return this.simulatedFailures;
	}
	
}// end ComponentSimulation
