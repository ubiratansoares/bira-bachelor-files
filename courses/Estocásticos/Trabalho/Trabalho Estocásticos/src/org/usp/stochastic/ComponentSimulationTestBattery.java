package org.usp.stochastic;

import java.util.*;

public class ComponentSimulationTestBattery {
	
	public static void main(String[] args) throws Exception{
		
		ArrayList<Double> meanResults = new ArrayList<Double>();
		
		int runs = 100;
		
		for(int i = 0; i < 200; i++) meanResults.add(0.0);
		
		for(int i = 0; i < runs; i++){
		
			ComponentSimulation c = new ComponentSimulation();
		
			ArrayList<Double> results = c.getSimulation();
		
			for(int j = 0; j < results.size(); j++){ 
				meanResults.set(j, meanResults.get(j) + results.get(j));
			}		
		}
		
		for(int i = 0; i < 200; i++) meanResults.set(i, meanResults.get(i) / runs);

		System.out.println("[BATTERY TEST RUN]\n");
				
		for(int i = 1; i < meanResults.size() + 1; i++){
			
			if(i % 8 == 0)	System.out.printf("[%d] %.2f \n", i, meanResults.get(i - 1));
			else System.out.printf("[%d] %.2f \t", i, meanResults.get(i - 1));
		}
	}//end Main
}//end CompomentSimulationTestBattery
