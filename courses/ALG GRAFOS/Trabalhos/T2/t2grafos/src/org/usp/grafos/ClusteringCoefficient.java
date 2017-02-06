package org.usp.grafos;

import java.util.*;
import org.jgrapht.graph.*;
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
	
	ClusteringCoefficient(Pseudograph<String, DefaultEdge> graph){
				
		// Cálculo de caminhos mínimos de todos para todos os vértices com Floyd-Warshall
		FloydWarshall<String, DefaultEdge> fwsp = new FloydWarshall<String, DefaultEdge>(graph);
		
		
		// Cálculo do diâmetro do grafo
		this.setDiameter(fwsp.getDiameter());
		
		// Cálculo do coeficiente de clusterização de cada vértice
		
		// Iterator para percorrer lista de vizinhos de um vértice
		NeighborIndex<String, DefaultEdge> ni = new NeighborIndex<String, DefaultEdge>(graph);
		
		// Inicialização da lista que mantém o coeficiente de cada vértice
		this.clusteringCoefficient = new ArrayList<Double>();
		
		// Iteração sobre cada vértice
		for(String vertex : graph.vertexSet()){
			        	
        	// Obtém lista de vizinhos do vértice
        	Set<String> neighbors = ni.neighborsOf(vertex);
                	
        	int ki = neighbors.size();

    		double cc;
    		
        	if(ki > 1){
        	
        		int eij = 0;

        	
        		//Montando subgrafo de vizinhaça do vértice
        		UndirectedSubgraph<String, DefaultEdge> neighborsSubGraph = new UndirectedSubgraph<String,DefaultEdge>(graph,neighbors,null);

			
        		// Encontrando lista de arestas do subgrafo composto por vizinho
        		Set<DefaultEdge> neighborsLinks = neighborsSubGraph.edgeSet();
			
        		// Determinando quantidade de links
        		eij = neighborsLinks.size() / 2;

			
        		// Calculando o coeficiente de clusterização para o vértice
        		cc = 2 * (double) eij / (ki  * (ki - 1));
        	}
        	
			else cc = 0;
        	
			// Atribuição do coeficiente na lista de coeficientes
			this.clusteringCoefficient.add(cc);
			
		}//end while
		
		
		// Obtendo o coeficiente médio de clusterização do grafo, ponderando somente os vértice conectados		
		double sum = 0.0;
		
		for(Double cc : this.clusteringCoefficient){ sum += cc;}
		
		
		// Cálculo do coeficiente de clusterização médio do grafo
		this.setAverageClusteringCoefficient(sum / (double) this.clusteringCoefficient.size());
		
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

	public ArrayList<Double> getCoefficients(){
		return this.clusteringCoefficient;
	}


}//end ClusteringCoefficient


        	
        




