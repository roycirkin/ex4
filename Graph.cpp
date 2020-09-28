#include "Graph.hpp"

namespace Graphs {

	// Graph Constructor
	Graph::Graph(vector<Edge> const &edges, int N)
	{
		// resize the vector to N elements of type vector<int>
		adjList.resize(N);

		// add edges to the directed graph
		for (auto &edge: edges)
		{
			// insert at the end
			adjList[edge.src].push_back(edge.dest);

			// Uncomment below line for undirected graph
			// adjList[edge.dest].push_back(edge.src);
		}
	}





    void printGraph(Graph const& graph, int N)
    {
        for (int i = 0; i < N; i++)
        {
            // print current vertex number
            cout << i << " --> ";

            // print all neighboring vertices of vertex i
            for (int v : graph.adjList[i])
                cout << v << " ";
            cout << endl;
        }
    }









}