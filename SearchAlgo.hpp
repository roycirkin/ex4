#pragma once
#include "Graph.hpp"

namespace Algorithm {

enum Status_algo {

    success, no_path, wrong_graph
}

class SearchAlgo {

public:
    virtual Status_algo solve(Graphs::Graph& g) const = 0 ;
};


class BFSAlgo : public SearchAlgo {
public:
    Status_algo solve(Graphs::Graph& g) const;
};

class DFSAlgo : public SearchAlgo {
public:
    Status_algo solve(Graphs::Graph& g) const;
};













}