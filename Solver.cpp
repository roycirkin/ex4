#include "Solver.hpp"
#include "SearchAlgo.hpp"

namespace Solver {

// Explicit template instantiation
template class GraphSolver<Algorithm::BFSAlgo>;
template class GraphSolver<Algorithm::DFSAlgo>;
template class GraphSolver<Algorithm::AstarAlgo>;


template <class A> 
GraphSolver<A>::GraphSolver(Graphs::MatrixGraph& graph, size_t start, size_t end)
    : graph(graph), start(start), end(end) {}

template <class A>
Status_solver GraphSolver<A>::solve() {
    try {
        //checks if we were given a search algorithm
        A alg;
        auto palg = static_cast<Algorithm::SearchAlgo*>(&alg);
        if (!palg) {
            throw Algorithm::noAlgorithmGiven();
        }
      
        Algorithm::Path solution = (*palg)(this->graph, this->start, this->end);//send path as a reference and return void
        price = solution.getRouteCost();
        route = solution.getSolutionRoute();
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
    } catch (Algorithm::noAlgorithmGiven e) {
        e.print();
        return no_such_algorithm;
    }

}

template <class A>
double GraphSolver<A>::getPrice() {
    return price;
}
template <class A>
std::string GraphSolver<A>::getName() {
    A alg;
    auto palg = static_cast<Algorithm::SearchAlgo*>(&alg);
    if (!palg) {
        throw Algorithm::noAlgorithmGiven();
    }

    return palg->name();
}




}