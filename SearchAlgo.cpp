#include "SearchAlgo.hpp"
#include <queue>


namespace Algorithm {

    size_t BFSAlgo::solve(const Graphs::Graph& g, size_t start, size_t end) const{

        if (start == end) {
            return 0;
        }
        if (((int)start < 0) || ((int)start > g.getSize()) || ((int)end  < 0) || ((int)end > g.getSize())) {
            throw WrongAssignment();
        }
        //if graph is wrong
        //throw WrongGraph();

        std::queue<int> bfsQueue;
        std::vector<double> bestPathTo;
        std::vector<bool> wasDeveloped;
        bestPathTo.resize(g.getSize());
        wasDeveloped.resize(g.getSize());
        for (size_t it = 0; it < bestPathTo.size(); ++it) {
            bestPathTo[it] = -1;
            wasDeveloped[it] = false;
            if (it == 0) {
                bestPathTo[it] = g.getCosts()[it];
            }
        }

        bfsQueue.push(start);
        while (!bfsQueue.empty()) {
            int vertex = bfsQueue.front();
            bfsQueue.pop();
            if (wasDeveloped[vertex] == true) {
                continue;
            }
            wasDeveloped[vertex] = true;
            for (auto newVer : g.getAdjList()[vertex]) {
                bfsQueue.push(newVer);
                if ((g.getCosts()[vertex] != -1) && (g.getCosts()[newVer] != -1) && (bestPathTo[vertex] != -1)) {
                    if (bestPathTo[newVer] == -1) {
                        bestPathTo[newVer] = g.getCosts()[newVer] +bestPathTo[vertex];
                    }
                    else {
                        if ((g.getCosts()[newVer] + bestPathTo[vertex]) < bestPathTo[newVer]) {
                            bestPathTo[newVer] = g.getCosts()[newVer] + bestPathTo[vertex];
                        }
                    }
                }
            }
        }
        if (bestPathTo[end] == -1) {
            throw NoRoute();
        }
        std::cout << std::endl;
        std::cout << bestPathTo[end];
        return bestPathTo[end];//???
    }



    size_t DFSAlgo::solve(const Graphs::Graph& g, size_t start, size_t end) const{

    if (start == end) {
        return 0;
    }
    if (((int)start < 0) || ((int)start > g.getSize()) || ((int)end  < 0) || ((int)end > g.getSize())) {
            throw WrongAssignment();
    }
    std::vector<double> bestPathTo;
    std::vector<bool> wasDeveloped;
    bestPathTo.resize(g.getSize());
    wasDeveloped.resize(g.getSize());
    for (size_t it = 0; it < bestPathTo.size(); ++it) {
        bestPathTo[it] = -1;
        wasDeveloped[it] = false;
        if (it == 0) {
            bestPathTo[it] = g.getCosts()[it];
        }
    }
    developVertex(g, 0, wasDeveloped, bestPathTo);
    if (bestPathTo[end] == -1) {
        throw NoRoute();
    }
    std::cout << std::endl;
    std::cout << bestPathTo[end];
    return bestPathTo[end];
    }



    void DFSAlgo::developVertex(const Graphs::Graph& g, size_t vertex, std::vector<bool>& wasDeveloped,
    std::vector<double>& bestPathTo) const {

        if (wasDeveloped[vertex] == true) {
            return;
        }
        wasDeveloped[vertex] = true;

        if ((g.getCosts()[vertex] == -1) ||  (bestPathTo[vertex] == -1)){
            return;
        }
        for (auto newVer : g.getAdjList()[vertex]) {
            if (g.getCosts()[newVer] == -1) {
                continue;
            }
            if (bestPathTo[newVer] == -1) {
                bestPathTo[newVer] = g.getCosts()[newVer] +bestPathTo[vertex];
            }
            else {
                if ((g.getCosts()[newVer] + bestPathTo[vertex]) < bestPathTo[newVer]) {
                    bestPathTo[newVer] = g.getCosts()[newVer] + bestPathTo[vertex];
                }
            }
        }
        for (auto newVer : g.getAdjList()[vertex]) {
            developVertex(g, newVer, wasDeveloped, bestPathTo);
        }

    }




    




}