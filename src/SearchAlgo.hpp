#pragma once
#include "Graph.hpp"

namespace Algorithm {

enum directions {
    up, down, right, left,
};

class Path {
private:
    double routeCost;
    std::vector<directions> solutionRoute;
public:
    Path();
    Path(std::vector<directions> route, double cost);
    //returns the route of the solution path
    const std::vector<directions> getSolutionRoute() const;
    double getRouteCost() const;
};


class SearchAlgo {

public:
    //operating the search algorithm
    virtual Path operator()( Graphs::MatrixGraph& g, size_t start, size_t end) = 0;
    virtual std::string name() const = 0;

};


class AstarAlgo : public SearchAlgo {

public:
    Path operator()( Graphs::MatrixGraph& g, size_t start, size_t end) ;
    //develops a single vertex in the graph
    void developVertex( Graphs::MatrixGraph& g, size_t vertex, std::vector<bool>& wasDeveloped,
    std::vector<double>& bestPathTo, std::vector<int>& whereWeCameFrom) ;

    virtual std::string name() const{
        return std::string("A*");
    }


};
class BFSAlgo : public SearchAlgo {

public:
    Path operator()( Graphs::MatrixGraph& g, size_t start, size_t end) ;
    virtual std::string name() const{
        return std::string("BFS");
    }
    //virtual ~BFSAlgo() {}

};

class DFSAlgo : public SearchAlgo {
public:
    Path operator()( Graphs::MatrixGraph& g, size_t start, size_t end) ;
    //develops a single vertex in the graph - recursion
    void developVertex( Graphs::MatrixGraph& g, size_t vertex, std::vector<bool>& wasDevelop,
    std::vector<double>& bestPathTo, std::vector<int>& whereWeCameFrom) ;
    virtual std::string name() const{
        return std::string("DFS");
    }
    //virtual ~DFSAlgo() {}

};

//develops a single vertex in the graph
void developVertexGlobal( Graphs::Graph& g, size_t vertex, std::vector<bool>& wasDeveloped,
     std::vector<double>& bestPathTo, std::vector<int>& whereWeCameFrom);
// returns the route
std::vector<directions> getDierections(std::vector<int>& whereWeCameFrom, size_t start, size_t end, size_t width);


class WrongAssignment {

public:
    void print() {
        std::cerr << "the start or end points are wrong";
    }
};

class WrongGraph {
public:
    void print() {
        std::cerr << "the graph is wrong";
    }
};

class NoRoute {
public:
    void print() {
        std::cerr << "there is no route in the graph from the start point to the finish point";
    }
};

class noAlgorithmGiven {
public:
    void print() {
        std::cerr << "no such algorith";
    }
};









}