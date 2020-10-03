#pragma once 
#include "SearchAlgo.hpp"

namespace Solver {

enum Status_solver {
    success, no_path, wrong_graph, wrong_assignment, no_such_algorithm, unValidMessage
};

class Solver {
public:
    virtual std::string getName() const {return std::string();}
    virtual Status_solver solve() {return success;}
};

class GraphSolveBase {
private:


public:

};

template <class A>
class GraphSolver : public GraphSolveBase{
private:
    Graphs::MatrixGraph graph;
    size_t start;
    size_t end;
    double price;
    std::vector<Algorithm::directions> route;
    
public:
    GraphSolver(){}
    GraphSolver(Graphs::MatrixGraph& graph, size_t start, size_t end);
    void setGraph(Graphs::MatrixGraph& graph);
    void setStart(size_t start);
    void setEnd(size_t end);
    //solving the graph and returns the price that the algorithm gave and the route
    virtual Status_solver solve();
    double getPrice() const;
    virtual std::string getName() const;
    std::string getRoute() const;
};










}