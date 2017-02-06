package org.usp.grafos;

import java.util.*;

import org.jgrapht.*;


public class FloydWarshall<V, E>{

    private Graph<V, E> graph;
    private List<V> vertices;
    private double diameter = 0.0;
    private double [][] d = null;
    private int [][] backtrace = null;

  
    // Construtor
    
    public FloydWarshall(Graph<V, E> graph)
    {
        this.graph = graph;
        this.vertices = new ArrayList<V>(graph.vertexSet());
    }

    // Methods
    /**
     * @return o grafo no qual esse algoritmo executa
     */
    public Graph<V, E> getGraph(){ return graph;}

    


    /**
     * Calcula a matriz de caminhos mais curtos bem como o diâmtro do grafo
     *
     */
    private void lazyCalculateMatrix()
    {
        if (d != null) {
            // already done
            return;
        }

        int n = vertices.size();

        // init the backtrace matrix
        backtrace = new int[n][n];
        for (int i = 0; i < n; i++) {
            Arrays.fill(backtrace[i], -1);
        }

        // initialize matrix, 0
        d = new double[n][n];
        for (int i = 0; i < n; i++) {
            Arrays.fill(d[i], Double.POSITIVE_INFINITY);
        }

        // initialize matrix, 1
        for (int i = 0; i < n; i++) {
            d[i][i] = 0.0;
        }

        // initialize matrix, 2
        Set<E> edges = graph.edgeSet();
        for (E edge : edges) {
            V v1 = graph.getEdgeSource(edge);
            V v2 = graph.getEdgeTarget(edge);

            int v_1 = vertices.indexOf(v1);
            int v_2 = vertices.indexOf(v2);

            d[v_1][v_2] = graph.getEdgeWeight(edge);
            if (!(graph instanceof DirectedGraph<?, ?>)) {
                d[v_2][v_1] = graph.getEdgeWeight(edge);
            }
        }

        // run fw alg
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    double ik_kj = d[i][k] + d[k][j];
                    if (ik_kj < d[i][j]) {
                        d[i][j] = ik_kj;
                        backtrace[i][j] = k;
                        diameter = (diameter > d[i][j]) ? diameter : d[i][j];
                    }
                }
            }
        }
    }

    /**
     * Obtém o comprimento do caminho mais curto
     *
     * @param a primeiro vértice
     * @param b segundo vértice
     *
     * @return distância mais curta entre a e b
     */
    public double shortestDistance(V a, V b)
    {
        lazyCalculateMatrix();

        return d[vertices.indexOf(a)][vertices.indexOf(b)];
    }

    /**
     * @return retorna o diâmetro do grafo
     */
    public double getDiameter()
    {
        lazyCalculateMatrix();
        return diameter;
    }



}

