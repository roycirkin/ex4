#pragma once 
#include "SearchAlgo.hpp"

namespace Solver {

enum Status_solver {
    success, no_path, wrong_graph, wrong_assignment, no_such_algorithm,
};

class Solver {
public:
    virtual std::string getName(){return std::string();}

};

template <class A>
class GraphSolver : public Solver{
private:
    Graphs::MatrixGraph graph;
    size_t start;
    size_t end;
    double price;
    std::vector<Algorithm::directions> route;
    
public:
    GraphSolver(){}
    GraphSolver(Graphs::MatrixGraph& graph, size_t start, size_t end);
    //solving the graph and returns the price that the algorithm gave and the route
    Status_solver solve();
    double getPrice();
    virtual std::string getName();
};










}