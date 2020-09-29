#pragma once
#include <iostream>
#include <vector>
#include <map>
using namespace std;

namespace Graphs {
// data structure to store graph edges
struct Edge {
	int src, dest;
};

// class to represent a graph object
class Graph
{
private: 
	vector<vector<int>> m_adjList;
    std::vector<double> m_costs;

public:
	// construct a vector of vectors to represent an adjacency list

	// Graph Constructor
	Graph(vector<Edge> const &edges, std::vector<double>&  costs);
    int getSize() const;
    const std::vector<vector<int>>& getAdjList();
	const std::vector<double>& getCosts();
	friend void printGraph(Graph const& graph);
};

// print adjacency list representation of graph
void printGraph(Graph const& graph);



class unvalidCosts {

public:
	void printError() {
		std::cerr << "unvalid costs";
	}

};


}