#pragma once 
#include "SearchAlgo.hpp"

namespace Solver {

enum Status_solver {
    success, no_path, wrong_graph, wrong_assignment, no_such_algorithm,
};

template <class A>
class GraphSolver {
private:
    Graphs::Graph graph;
    size_t start;
    size_t end;
public:
    GraphSolver(Graphs::Graph& graph, size_t start, size_t end);
    Status_solver solve(double* price) const;

};










}