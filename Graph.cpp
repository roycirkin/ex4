#include "Graph.hpp"
#include <algorithm>

namespace Graphs {

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

Graph::Graph(matrix::Matrix& m) {
    try {
        std::vector<double> costs;
        std::vector<Edge> edges;
        for (size_t i = 0; i < m.matrixGetHeight(); ++i) {
            for (size_t j = 0; j < m.matrixGetWidth(); ++j) {
                if ((m(i,j) < 1) && (m(i,j) != -1)) {
                    return;
                }
                costs.push_back(m(i,j));

                if (j != (m.matrixGetWidth() - 1)) {
                    edges.push_back({(int)(i * m.matrixGetWidth() + j), (int)(i * m.matrixGetWidth() + j + 1)});
                    //edges[i * m.matrixGetWidth() + j].push_back(i * m.matrixGetWidth() + j + 1);
                }
                if (j != 0) {
                    edges.push_back({(int)(i * m.matrixGetWidth() + j), (int)(i * m.matrixGetWidth() + j - 1)});
                    //edges[i * m.matrixGetWidth() + j].push_back(i * m.matrixGetWidth() + j - 1);
                }
                if (i != (m.matrixGetHeight() - 1)) {
                    edges.push_back({(int)(i * m.matrixGetWidth() + j), (int)((i + 1) * m.matrixGetWidth() + j)});
                    //edges[i * m.matrixGetWidth() + j].push_back((i + 1) * m.matrixGetWidth() + j);
                }
                if (i != 0) {
                    edges.push_back({(int)(i * m.matrixGetWidth() + j), (int)((i - 1) * m.matrixGetWidth() + j)});
                    //edges[i * m.matrixGetWidth() + j].push_back((i - 1) * m.matrixGetWidth() + j);
                }
                
            }
        }

        m_adjList.resize(costs.size());
        m_costs = costs;
        // add edges to the directed graph
        for (auto &edge: edges) {
        
            // insert at the end
            if ((edge.src < 0) || (edge.src > (int)costs.size()) || (edge.dest < 0) || (edge.dest > (int)costs.size())) {
                m_costs.clear();
                m_adjList.clear();
                return;
            }
            m_adjList[edge.src].push_back(edge.dest);

            // Uncomment below line for undirected graph
            // adjList[edge.dest].push_back(edge.src);
        }
        isMatrix = true;

    }
    catch (unvalidCosts e) {
        e.printError();
    }
}



int Graph::getSize() const{
    return m_costs.size();
}

size_t Graph::getWidth() const {
if (!isMatrix) {
    return 0;
}
size_t counter = 1;
while (std::count(m_adjList[counter - 1].begin(), m_adjList[counter - 1].end(), counter)) {
counter++;
}
return counter;
}


const std::vector<vector<int>>& Graph::getAdjList() const{
    return m_adjList;
}

const std::vector<double>& Graph::getCosts() const{
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








