#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "matrix/ClassMatrix.hpp"
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
	//the costs of the vertexes
    std::vector<double> m_costs;

public:

	Graph();
	Graph(vector<Edge> const &edges, std::vector<double>&  costs);
    int getSize() const;
    std::vector<vector<int>>& getAdjList() ;
	std::vector<double>& getCosts() ;
	size_t getWidth();
	void setCosts(std::vector<double> costs);
	void setAdjList (std::vector<vector<int>> adj);
	friend void printGraph(Graph const& graph);
};

class MatrixGraph : public Graph{
public:
	MatrixGraph();
	MatrixGraph(matrix::Matrix& m);
	size_t getWidth() ;


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