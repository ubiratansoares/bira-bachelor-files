package org.usp.grafos;

import java.io.*;
import org.jgrapht.graph.*;



public class GraphFileParser {
		
		public Pseudograph<String, DefaultEdge> read(File file) throws IOException {
			
			Pseudograph<String, DefaultEdge> graph = new Pseudograph<String, DefaultEdge> (DefaultEdge.class);
			
			BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
			
			String line = in.readLine();
			
			line = skipEmptyLines(in);
			
			while(line != null) {
			
				String[] args = line.split("\\t");
				String src = null, dest = null; 

				try {
					src = args[0]; 
					
					dest = args[1]; //System.out.println(src + " - " + dest);
					
					if(!graph.containsVertex(src)) graph.addVertex(src);
					
					if(!graph.containsVertex(dest)) graph.addVertex(dest);
					
					graph.addEdge(src, dest);
					
				} catch(ArrayIndexOutOfBoundsException e) {
					
					System.out.println(e.toString());
					
				} catch(NullPointerException e) {
					
					System.err.printf("A vertex was not found: either \"%s\" or \"%s\".%n", src, dest);
				}
				line = skipEmptyLines(in);
			}
			return graph;
		}
		
		private String skipEmptyLines(BufferedReader in) throws IOException {
			
			String line = in.readLine();
			
			while(line != null && line.trim().isEmpty()) 
				
				line = in.readLine(); //skip whitespaces
			
			return line;
		}
		
	}


	

