package org.usp.stochastic;

import java.util.*;

public class ComponentSimulationTest {
	
	public static void main(String[] args) throws Exception{
	
		ComponentSimulation c = new ComponentSimulation();

		ArrayList<Double> simulation = c.getSimulation();

		System.out.println("[START] \n\nUNIQUE SIMULATION FOR EACH COMPONENT FAILURE\n");
		
		for(int i = 1; i < simulation.size() + 1; i++){
			
			if(i % 8 == 0)	System.out.printf("[%d] %.2f \n", i, simulation.get(i - 1));
		
			else System.out.printf("[%d] %.2f \t", i, simulation.get(i - 1));
		}

		System.out.println("\n\n[END]");
		
		int k = 10;
		double k10time = 0.0;
		
		for(int i = 0; i < k; i++) k10time += simulation.get(i);
		
		System.out.println("\n -> TIME UNTIL 10th COMPONENT FAIL = " + k10time);
		
		ArrayList<Double> costs = new ArrayList<Double>();
		
		double r = 0.05;
		double beta = 10;
		double lambda = (double) 1/60;
		
		double alpha = 1 / (1 + r);
		
		for(int i = 0; i < simulation.size(); i++){
			double cost = Math.exp(-alpha * simulation.get(i)) * beta;
			costs.add(cost);
		}
		
		double totalCost = 0.0;
		
		System.out.println("\n -> MAINTENANCE COST UNTIL 10th COMPONENT FAIL = " + costs.get(10));
		
		for(int i = 0; i < costs.size(); i++) {totalCost += costs.get(i);}
		
		System.out.println("\n -> TOTAL SYSTEM MAINTENANCE COST = " + totalCost);
		
		double estimatedParameter = totalCost / simulation.size();
		
		System.out.println("\n -> ESTIMATED PARAMETER FOR COST DISTRIBUTION = " + estimatedParameter);
		
		System.out.println("\n -> ESTIMATED LAMBDA FOR COST DISTRIBUTION = " + 1/estimatedParameter);

		double teorethicalExpectedValueCost = beta * lambda / alpha;
		
		System.out.println("\n -> TEORETHICAL EXPECTED VALUE FOR COST DISTRIBUTION = " + teorethicalExpectedValueCost);

		double praticalExpectedValueCost = totalCost / costs.size();
		
		System.out.println("\n -> EMPIRIC EXPECTED VALUE FOR COST DISTRIBUTION = " + praticalExpectedValueCost);

	}
}
