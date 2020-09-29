#include "Graph.hpp"

namespace Graphs {

// Graph Constructor
Graph::Graph(vector<Edge> const &edges, std::vector<double>& costs)
{
    try {
        for (auto d : costs) {
            if ((d < 1) && (d != -1)) {
                throw unvalidCosts();
            }
        }
        // resize the vector to N elements of type vector<int>
        m_adjList.resize(costs.size());
        m_costs = costs;
        // add edges to the directed graph
        for (auto &edge: edges) {
        
            // insert at the end
            m_adjList[edge.src].push_back(edge.dest);

            // Uncomment below line for undirected graph
            // adjList[edge.dest].push_back(edge.src);
        }
    }

    catch (unvalidCosts e) {
        e.printError();
    }

}
int Graph::getSize() const{
    return m_costs.size();
}

const std::vector<vector<int>>& Graph::getAdjList() {
    return m_adjList;
}

const std::vector<double>& Graph::getCosts() {
    return m_costs;
}

void printGraph(Graph const& graph)
{
    for (int i = 0; i < graph.getSize(); i++)
    {
        // print current vertex number
        cout << i << " --> ";

        // print all neighboring vertices of vertex i
        for (int v : graph.m_adjList[i])
            cout << v << " ";

        cout << " | " << "cost is: " << graph.m_costs[i];
        cout << endl;
    }
}


}








