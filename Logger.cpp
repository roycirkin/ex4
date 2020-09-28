#include <Graph.hpp>

namespace Graphs
{
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
} // namespace name
