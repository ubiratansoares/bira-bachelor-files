package org.usp.grafos;

import org.jgrapht.graph.*;
import org.jgrapht.alg.*;
import org.jgrapht.traverse.*;
import java.util.*;

public class TSPMininumSpanningTree {

	private KruskalMinimumSpanningTree<String,DefaultWeightedEdge> kmst;
	private SimpleWeightedGraph<String, DefaultWeightedEdge> mstgraph;
	
    TSPMininumSpanningTree(SimpleWeightedGraph<String, DefaultWeightedEdge> g ) {
    	
 
        this.kmst = new KruskalMinimumSpanningTree<String,DefaultWeightedEdge>(g);
        
        Set<DefaultWeightedEdge> set = kmst.getEdgeSet();
        
        //System.out.println(set.toString());

        this.mstgraph = new SimpleWeightedGraph (DefaultWeightedEdge.class);
        
        Iterator<DefaultWeightedEdge> it1 = set.iterator();
        
        while(it1.hasNext()){
        	
        	DefaultWeightedEdge edge = new DefaultWeightedEdge();
        	
        	edge = it1.next();
        	
        	//System.out.println("EDGE" + g.getEdgeTarget(edge));

        	if(! mstgraph.containsVertex(g.getEdgeSource(edge))){
        	
        		mstgraph.addVertex(g.getEdgeSource(edge));
        		
        	}
        	
           	if(! mstgraph.containsVertex(g.getEdgeTarget(edge))){
            	
        		mstgraph.addVertex(g.getEdgeTarget(edge));
        	     		
        	}
        	
        	
        }//end while
        
        //System.out.println(mstgraph.toString());
        
        Iterator<DefaultWeightedEdge> it2 = set.iterator();

        
        while(it2.hasNext()){
        	
        	DefaultWeightedEdge edge = new DefaultWeightedEdge();
        	DefaultWeightedEdge aux;
        	
        	edge = it2.next();
        	
        	//System.out.println("EDGE" + g.getEdgeTarget(edge));
        	
            aux = mstgraph.addEdge(g.getEdgeSource(edge),g.getEdgeTarget(edge));
            mstgraph.setEdgeWeight(aux, g.getEdgeWeight(edge));
        		
        	
        }//end while
        
    }//end constructor


    public String getPath(){
    	
		String ret =  new String();
				
		double accumulated = 0.0;
				
		DepthFirstIterator<String, DefaultWeightedEdge> preorder = new DepthFirstIterator<String, DefaultWeightedEdge>(this.mstgraph);
		
		String currentVertex = preorder.next();
		ret = ret.concat("\nBEGIN VERTEX = " + currentVertex);
		
		while(preorder.hasNext()){
			String nextVertex = preorder.next();
			ret = ret.concat("\nNEXT VERTEX = " + nextVertex);
			
			accumulated += mstgraph.getEdgeWeight(mstgraph.getEdge(currentVertex, nextVertex));
			currentVertex = nextVertex;
			
			
			
		}//end while
			
		
		return ret;
		
    	
    }//end getPath 

}
