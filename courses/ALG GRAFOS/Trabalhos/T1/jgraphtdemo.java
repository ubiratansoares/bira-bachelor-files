/* DirectedGraphDemo.java --- demonstration of operations on directed graphs
 * Copyright (C) 2008  Minh Van Nguyen <nguyenminh2@gmail.com>
 *
 * This program is free software; you can redistribute it and/or  modify
 * it under the terms of the GNU General Public License as published  by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

import java.util.List;
import org.jgrapht.alg.*;
import org.jgrapht.*;
import org.jgrapht.graph.*;

/**
 * This class demonstrates some of the operations that can be performed on
 * directed graphs. After constructing a basic directed graph, it computes all
 * the strongly connected components of this graph. It then finds the shortest
 * path from one vertex to another using Dijkstra's shortest path algorithm.
 * The sample code should help to clarify to users of JGraphT that the class
 * org.jgrapht.alg.DijkstraShortestPath can be used to find shortest paths
 * within directed graphs.
 *
 * @author Minh Van Nguyen
 * @since 2008-01-17
 */
public class DirectedGraphDemo {
	
    public static void main(String args[]) {
        // constructs a directed graph with the specified vertices and edges
        DirectedGraph<String, DefaultEdge> directedGraph =
            new DefaultDirectedGraph<String, DefaultEdge>
            (DefaultEdge.class);
        directedGraph.addVertex("a");
        directedGraph.addVertex("b");
        directedGraph.addVertex("c");
        directedGraph.addVertex("d");
        directedGraph.addVertex("e");
        directedGraph.addVertex("f");
        directedGraph.addVertex("g");
        directedGraph.addVertex("h");
        directedGraph.addVertex("i");
        directedGraph.addEdge("a", "b");
        directedGraph.addEdge("b", "d");
        directedGraph.addEdge("d", "c");
        directedGraph.addEdge("c", "a");
        directedGraph.addEdge("e", "d");
        directedGraph.addEdge("e", "f");
        directedGraph.addEdge("f", "g");
        directedGraph.addEdge("g", "e");
        directedGraph.addEdge("h", "e");
        directedGraph.addEdge("i", "h");

        // computes all the strongly connected components of the directed graph
        StrongConnectivityInspector sci =
            new StrongConnectivityInspector(directedGraph);
        List stronglyConnectedSubgraphs = sci.stronglyConnectedSubgraphs();

        // prints the strongly connected components
        System.out.println("Strongly connected components:");
        for (int i = 0; i < stronglyConnectedSubgraphs.size(); i++) {
            System.out.println(stronglyConnectedSubgraphs.get(i));
        }
        System.out.println();

        // Prints the shortest path from vertex i to vertex c. This certainly
        // exists for our particular directed graph.
        System.out.println("Shortest path from i to c:");
        List path =
            DijkstraShortestPath.findPathBetween(directedGraph, "i", "c");
        System.out.println(path + "\n");

        // Prints the shortest path from vertex c to vertex i. This path does
        // NOT exist for our particular directed graph. Hence the path is
        // empty and the variable "path" must be null.
        System.out.println("Shortest path from c to i:");
        path = DijkstraShortestPath.findPathBetween(directedGraph, "c", "i");
        System.out.println(path);
    }
}