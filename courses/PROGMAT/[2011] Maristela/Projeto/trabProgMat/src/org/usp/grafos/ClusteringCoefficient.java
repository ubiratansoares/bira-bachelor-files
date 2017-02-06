package org.usp.grafos;

import java.util.*;
import org.jgrapht.graph.*;
import org.jgrapht.traverse.*;
import org.jgrapht.alg.*;

public class ClusteringCoefficient {

	private double averageClusteringCoefficient;
	private double diameter;
	private ArrayList<Double> clusteringCoefficient;
	
	/*
	 * Construtor
	 * 
	 * Aceita um grafo e calcula o coeficiente de clusterização médio, 
	 * bem como o diâmetro do Grafo
	 * 
	 * */
	
	ClusteringCoefficient(SimpleGraph<String, DefaultEdge> graph){
				
		
		FloydWarshallShortestPaths<String, DefaultEdge> fwsp = new FloydWarshallShortestPaths<String, DefaultEdge>(graph);
		
		
		// Cálculo do diâmetro do grafo
		this.setDiameter(fwsp.getDiameter());
		
		// Cálculo do coeficiente de clusterização de cada vértice

		
		// Iterator para percorrer o grafo com busca em profundidade
		DepthFirstIterator<String, DefaultEdge> dfit = new DepthFirstIterator<String, DefaultEdge>(graph);
		
		
		// Iterator para percorrer lista de vizinhos de um vértice
		NeighborIndex<String, DefaultEdge> ni = new NeighborIndex<String, DefaultEdge>(graph);

		
		// Inicialização da lista que mantém o coeficiente de cada vértice
		this.clusteringCoefficient = new ArrayList<Double>();
		
		
		// Iteração sobre cada vértice
		while(dfit.hasNext()){
					
			// Inicializações
			String vertex = dfit.next();
			
        	int ki = graph.inDegreeOf(vertex) + graph.outDegreeOf(vertex);	
        	
        	int eij = 0;
         	
        	
        	// Obtém lista de vizinhos do vértice
        	List<String> neighbors = ni.neighborListOf(vertex);
        	
        	
        	// Explorando a vizinhança e contando os links entre vizinhos
			for(int i = 0; i < neighbors.size(); i++){
				for(int j = i + 1 ; j < neighbors.size(); j++){
					
					if(graph.containsEdge(neighbors.get(i), neighbors.get(j))){ eij++;}	
				}	
			}
			
			// Cálculo do coeficiente de clusterização do vértice
			double cc = (double) 2 * eij / (ki = ki - 1);
			
			// Atribuição do coeficiente na lista de coeficientes
			this.clusteringCoefficient.add(cc);
			
		}//end while
		
		
		// Obtendo o coeficiente médio de clusterização do grafo
		
		double sum = 0.0;
		
		for(Double cc : this.clusteringCoefficient){ sum += cc; }
		
		this.setAverageClusteringCoefficient(sum / this.clusteringCoefficient.size());
		
	}//end Constructor

// Setters and Getters
	
public void setAverageClusteringCoefficient(double averageClusteringCoefficient) {
	this.averageClusteringCoefficient = averageClusteringCoefficient;
}

public double getAverageClusteringCoefficient() {
	return averageClusteringCoefficient;
}

public void setDiameter(double diameter) {
	this.diameter = diameter;
}

public double getDiameter() {
	return diameter;
}
        		
}//end ClusteringCoefficient
        	
        




