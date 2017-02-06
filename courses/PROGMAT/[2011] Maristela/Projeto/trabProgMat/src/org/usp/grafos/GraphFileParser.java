package org.usp.grafos;

import java.io.*;
import org.jgrapht.graph.*;

/* Leitor de arquivos de grafos em texto.
 * 
 * 
 * vertices:
 *   VERTEX_NAME_1
 *   VERTEX_NAME_2
 *   ...
 * 
 * edges:
 *   "VERTEX_NAME" "VERTEX_NAME" WEIGHT
 *   "VERTEX_NAME" "VERTEX_NAME" WEIGHT
 *   ...
 */

public class GraphFileParser {
		
		public SimpleWeightedGraph<String, DefaultWeightedEdge> read(File file) throws IOException {
			
			SimpleWeightedGraph<String, DefaultWeightedEdge> graph = new SimpleWeightedGraph<String, DefaultWeightedEdge> (DefaultWeightedEdge.class);
			
			BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream(file)));
			
			String line = in.readLine();
			
			line = skipEmptyLines(in);
			
			if(line == null || !line.trim().equals("vertices:"))  return graph;
			
			line = skipEmptyLines(in);
			
			while(line != null && !line.trim().equals("edges:")) {
				graph.addVertex(line.trim());
				line = skipEmptyLines(in);
			}
			
			while(line != null) {
			
				String[] args = line.split("\"");
				String src = null, dest = null; double weight;
				try {
					src = args[1].trim();
					
					dest = args[3].trim();
					
					try {
						weight = Double.valueOf(args[4]);
					} catch(NumberFormatException e) {
						weight = Double.NaN;
					} catch(NullPointerException e) {
						weight = Double.NaN;
					}
					
			        DefaultWeightedEdge e;
					
					e = graph.addEdge(src, dest); graph.setEdgeWeight(e, weight);
					
				} catch(ArrayIndexOutOfBoundsException e) {
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


	

