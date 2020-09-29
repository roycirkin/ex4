#pragma once
#include "Graph.hpp"

namespace Algorithm {

class SearchAlgo {

public:
    virtual size_t solve(const Graphs::Graph& g, size_t start, size_t end) const = 0 ;
};


class BFSAlgo : public SearchAlgo {

public:
    size_t solve(const Graphs::Graph& g, size_t start, size_t end) const;
};

class DFSAlgo : public SearchAlgo {
public:
    size_t solve(const Graphs::Graph& g, size_t start, size_t end) const;
    void developVertex(const Graphs::Graph& g, size_t vertex, std::vector<bool>& wasDevelop, std::vector<double>& bestPathTo) const;
};



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









}