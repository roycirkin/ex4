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
    const std::vector<directions> getSolutionRoute() const;
    double getRouteCost() const;
};



class SearchAlgo {

public:
    virtual Path* operator()(const Graphs::Graph& g, size_t start, size_t end) const = 0;
};


class AstarAlgo : public SearchAlgo {

public:
    Path* operator()(const Graphs::Graph& g, size_t start, size_t end) const;
    void developVertex(const Graphs::Graph& g, size_t vertex, std::vector<bool>& wasDeveloped,
     std::vector<double>& bestPathTo, std::vector<int>& whereWeCameFrom) const;
};
class BFSAlgo : public SearchAlgo {

public:
    Path* operator()(const Graphs::Graph& g, size_t start, size_t end) const;
};

class DFSAlgo : public SearchAlgo {
public:
    Path* operator()(const Graphs::Graph& g, size_t start, size_t end) const;
    void developVertex(const Graphs::Graph& g, size_t vertex, std::vector<bool>& wasDevelop,
     std::vector<double>& bestPathTo, std::vector<int>& whereWeCameFrom) const;
};

void developVertexGlobal(const Graphs::Graph& g, size_t vertex, std::vector<bool>& wasDeveloped,
     std::vector<double>& bestPathTo, std::vector<int>& whereWeCameFrom);
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