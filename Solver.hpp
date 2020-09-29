#pragma once 
#include "SearchAlgo.hpp"

namespace Solver {

enum Status_solver {
    success, no_path, wrong_graph, wrong_assignment
};

class GraphSolver {
private:
    Graphs::Graph graph;
    size_t start;
    size_t end;
    Algorithm::SearchAlgo* alg;
public:
    GraphSolver(Graphs::Graph graph, size_t start, size_t end, Algorithm::SearchAlgo* alg);
    Status_solver solve() const;

};










}