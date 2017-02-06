package org.usp.grafos;

import org.jgrapht.graph.*;
import org.jgrapht.alg.*;
import org.jgrapht.traverse.*;

public class GraphEccentricity {

    public static void main(String args[]) {
    	
        SimpleGraph<String, DefaultEdge> g = new SimpleGraph<String, DefaultEdge> (DefaultEdge.class);
        
        g.addVertex("a");
        g.addVertex("b");
        g.addVertex("c");
        g.addVertex("d");
        g.addVertex("e");
        g.addVertex("f");
        g.addVertex("g");
 
        g.addEdge("a", "b");
        g.addEdge("a", "d");
        g.addEdge("d", "c");
        g.addEdge("c", "b");
        g.addEdge("d", "e");
        g.addEdge("c", "e");
        g.addEdge("e", "f");
        g.addEdge("d", "g");
        
        FloydWarshallShortestPaths<String, DefaultEdge> fwsp = 
        	new FloydWarshallShortestPaths<String, DefaultEdge>(g);
        
        //double[] eccentricity = new double[6];
        
        
        //System.out.println(fwsp.getDiameter());

        DepthFirstIterator<String, DefaultEdge> it1 = new DepthFirstIterator<String, DefaultEdge>(g);

        String u,v;
        double aux;

        //System.out.println("\nPrinting Vertex Names:\n\n");
        
        while (it1.hasNext()) {
        	
        	aux = 0.0;
           
        	v = it1.next();
        	
        	//System.out.println("v = "+v);
        	
            DepthFirstIterator<String, DefaultEdge> it2 = new DepthFirstIterator<String, DefaultEdge>(g);
           
        	while(it2.hasNext()){
        	
        			u = it2.next();
        			
        			//System.out.println("u = "+u);    	           
        			
        			if(u != v){
        				
        				double distance = fwsp.shortestDistance(u, v);
        				
        				if(distance > aux) aux = distance;
 
        			}
        	}
        
        	System.out.println("Vextex = "+v+ " | E(v) =  "+aux);
        	//it2.finalize();
        }
        	
        	//System.out.println(v+" ");
        
    }
}
