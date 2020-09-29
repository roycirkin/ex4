#include "Solver.hpp"

namespace Solver {

GraphSolver::GraphSolver(Graphs::Graph graph, size_t start, size_t end, Algorithm::SearchAlgo* alg)
    : graph(graph), start(start), end(end), alg(alg) {}


Status_solver GraphSolver::solve() const {
    try {
        size_t price = this->alg->solve(this->graph, this->start, this->end);
        price++;///////////////////////////////////////////////




        return success;
    } catch (Algorithm::NoRoute e) {
        e.print();
        return no_path;
    } catch (Algorithm::WrongGraph e) {
        e.print();
        return wrong_graph;
    } catch (Algorithm::WrongAssignment e) {
        e.print();
        return wrong_assignment;
    }

}






}