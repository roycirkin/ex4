#include "Graph.hpp"
#include <algorithm>

namespace Graphs {

void Graph::setCosts(const std::vector<double> costs) {
    m_costs = costs;
}
void Graph::setAdjList (const std::vector<vector<int>> adj) {
    m_adjList = adj;
}

// Graph Constructor
Graph::Graph(vector<Edge> const &edges, std::vector<double>& costs) {
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
            if ((edge.src < 0) || (edge.src > (int)costs.size()) || (edge.dest < 0) || (edge.dest > (int)costs.size())) {
                throw unvalidCosts();
            }
            m_adjList[edge.src].push_back(edge.dest);

            // Uncomment below line for undirected graph
            // adjList[edge.dest].push_back(edge.src);
        }
    }

    catch (unvalidCosts e) {
        e.printError();
    }

}

Graph::Graph() {}

MatrixGraph::MatrixGraph(matrix::Matrix& m) {
    Graph();
    try {
        std::vector<double> costs;
        std::vector<Edge> edges;

        //making the edges
        for (size_t i = 0; i < m.matrixGetHeight(); ++i) {
            for (size_t j = 0; j < m.matrixGetWidth(); ++j) {
                if ((m(i,j) < 1) && (m(i,j) != -1)) {
                    return;
                }
                costs.push_back(m(i,j));

                if (j != (m.matrixGetWidth() - 1)) {
                    edges.push_back({(int)(i * m.matrixGetWidth() + j), (int)(i * m.matrixGetWidth() + j + 1)});
                }
                if (j != 0) {
                    edges.push_back({(int)(i * m.matrixGetWidth() + j), (int)(i * m.matrixGetWidth() + j - 1)});
                }
                if (i != (m.matrixGetHeight() - 1)) {
                    edges.push_back({(int)(i * m.matrixGetWidth() + j), (int)((i + 1) * m.matrixGetWidth() + j)});
                }
                if (i != 0) {
                    edges.push_back({(int)(i * m.matrixGetWidth() + j), (int)((i - 1) * m.matrixGetWidth() + j)});
                }
                
            }
        }

        getAdjList().resize(costs.size());
        getCosts() = costs;
        // add edges to the directed graph
        for (auto &edge: edges) {
        
            // insert at the end
            if ((edge.src < 0) || (edge.src > (int)costs.size()) || (edge.dest < 0) || (edge.dest > (int)costs.size())) {
                getCosts().clear();
                getAdjList().clear();
                return;
            }
            getAdjList()[edge.src].push_back(edge.dest);

        }
         
    }
    catch (unvalidCosts e) {
        e.printError();
    }
}



int Graph::getSize() const{
    return m_costs.size();
}

size_t MatrixGraph::getWidth() {
size_t counter = 1;
while (std::count(getAdjList()[counter - 1].begin(), getAdjList()[counter - 1].end(), counter)) {
counter++;
}
return counter;
}


std::vector<vector<int>>& Graph::getAdjList() {
    return m_adjList;
}

std::vector<double>& Graph::getCosts() {
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








