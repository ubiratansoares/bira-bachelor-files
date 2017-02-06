package org.usp.grafos;

import org.jgrapht.graph.*;
import org.jgrapht.alg.*;
import java.util.*;

public class TSPClosestNeighbor {

	private ArrayList<String> pathList;
	private String randomStartVertex;
	private SimpleWeightedGraph<String, DefaultWeightedEdge> graph;
	
	/*
	 * Construtor
	 * 
	 * Aceita um grafo g e executa a heurística do vizinho mais próximo para 
	 * aproximar o TSP
	 * 
	 * */
	
	@SuppressWarnings("unchecked")
	TSPClosestNeighbor(SimpleWeightedGraph<String, DefaultWeightedEdge> g){
	
		//SimpleWeightedGraph<String, DefaultWeightedEdge> g = new SimpleWeightedGraph<String, DefaultWeightedEdge> (DefaultWeightedEdge.class);
        
		this.graph = new SimpleWeightedGraph<String, DefaultWeightedEdge> (DefaultWeightedEdge.class);
		
		this.graph = (SimpleWeightedGraph) g.clone();
		
		Set<String> s = g.vertexSet();
        
		//System.out.println("Vertex Set Size = "+s.size()+"\n");
    
		// A escolha do vértice inicial é randômica
		Random random = new Random();
        
		int r = random.nextInt(s.size());
            	     
		ArrayList<String> vertexList = new ArrayList<String>(s);
    
		this.pathList  = new ArrayList<String>();   
		this.randomStartVertex =  vertexList.get(r);
    
		//System.out.println("Picked Random Vertex = " + vertexList.get(r)+"\n");
        
		String v = vertexList.remove(r);
		this.pathList.add(v);
        
		//double accumulated = 0.0;
        
		while(!vertexList.isEmpty()){
        	                 	
			NeighborIndex ni = new NeighborIndex(g);
        	
			Iterator<String> neighbors = ni.neighborsOf(v).iterator();
        	
			double min = 999999999.9;
			
			String minVertex = "";
        	
			while(neighbors.hasNext()) {
        		
				String currentVertex = neighbors.next();
				//System.out.println("Neighbor "+currentVertex);
        		
				if(!vertexList.contains(currentVertex)) {
					continue;
				}	
        		
				//System.out.println("Edge "+g.getEdge(v, currentVertex));
				double weight = g.getEdgeWeight(g.getEdge(v, currentVertex));
        		
				//System.out.println("Weight: "+weight);
				if(weight <= min) {
        			
					min = weight;
					minVertex = currentVertex;
        		
				}
				
			}//end while
        	
			//accumulated += min;
        
			v = minVertex;
        
			this.pathList.add(v);
        
			vertexList.remove(v);
        	
			//System.out.println("Next Vertex = "+minVertex+ " | Accumulated Cost = "+accumulated);
      		
        	
		}//end while
		
	
        
    }//end constructor
		
		
	public String getPath(){
		
		String ret =  new String("\n\nSTART VERTEX = ");
		
		ret = ret.concat(this.randomStartVertex + "\n\n");
				
		double accumulated = 0.0;
				
		for(int i = 0; i < pathList.size() - 1; i++){	
			//System.out.println("VERTEX = "+pathList.get(i));	
			
			ret = ret.concat("\nNEXT VERTEX = " + pathList.get(i+1));
			
			accumulated += graph.getEdgeWeight(graph.getEdge(pathList.get(i), pathList.get(i+1)));
			
			ret = ret.concat("  | ACCUMULATED COST = "+ accumulated + "\n");
		}
		
		ret = ret.concat("\nNEXT VERTEX = " + this.randomStartVertex);
		
		accumulated += graph.getEdgeWeight(graph.getEdge(pathList.get(0), pathList.get(pathList.size()-1)));
		
		ret = ret.concat("  | ACCUMULATED COST = "+ accumulated + "\n");
		
		return ret;
	}	
		
}


