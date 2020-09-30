#include "SearchAlgo.hpp"
#include <queue>
#include <algorithm>
#include <math.h>
#include <limits>

namespace Algorithm {

    Path* BFSAlgo::operator()(const Graphs::Graph& g, size_t start, size_t end) const{

        if (start == end) {
            Path* sol = new Path();
            return sol;
        }
        if (((int)start < 0) || ((int)start > g.getSize()) || ((int)end  < 0) || ((int)end > g.getSize())) {
            throw WrongAssignment();
        }
        if (!g.getIsMatrix()) {
            throw WrongGraph();
        }

        std::queue<int> bfsQueue;
        std::vector<double> bestPathTo;
        std::vector<bool> wasDeveloped;
        std::vector<int> whereWeCameFrom;
        bestPathTo.resize(g.getSize());
        wasDeveloped.resize(g.getSize());
        whereWeCameFrom.resize(g.getSize());
        for (size_t it = 0; it < bestPathTo.size(); ++it) {
            bestPathTo[it] = -1;
            wasDeveloped[it] = false;
            if (it == start) {
                bestPathTo[start] = g.getCosts()[start];
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
                if ((g.getCosts()[vertex] == -1) || (g.getCosts()[newVer] == -1) || (bestPathTo[vertex] == -1)) {
                    continue;
                }
                if (bestPathTo[newVer] == -1) {
                    bestPathTo[newVer] = g.getCosts()[newVer] +bestPathTo[vertex];
                    whereWeCameFrom[newVer] = vertex;
                }
                else {
                    if ((g.getCosts()[newVer] + bestPathTo[vertex]) < bestPathTo[newVer]) {
                        bestPathTo[newVer] = g.getCosts()[newVer] + bestPathTo[vertex];
                        whereWeCameFrom[newVer] = vertex;
                    }
                }
            }
        }
        if (bestPathTo[end] == -1) {
            throw NoRoute();
        }
        std::cout << std::endl;
        std::cout << bestPathTo[end];
        std::vector<directions> route = getDierections(whereWeCameFrom, start, end, g.getWidth());
        Path* solution = new Path(route, bestPathTo[end]);
        return solution;
    }



    Path* DFSAlgo::operator()(const Graphs::Graph& g, size_t start, size_t end) const{

    if (start == end) {
        Path* sol = new Path();
        return sol;
    }
    if (((int)start < 0) || ((int)start > g.getSize()) || ((int)end  < 0) || ((int)end > g.getSize())) {
            throw WrongAssignment();
    }
    if (!g.getIsMatrix()) {
            throw WrongGraph();
    }
    std::vector<double> bestPathTo;
    std::vector<bool> wasDeveloped;
    std::vector<int> whereWeCameFrom;
    bestPathTo.resize(g.getSize());
    wasDeveloped.resize(g.getSize());
    whereWeCameFrom.resize(g.getSize());
    for (size_t it = 0; it < bestPathTo.size(); ++it) {
        bestPathTo[it] = -1;
        wasDeveloped[it] = false;
        if (it == start) {
            bestPathTo[it] = g.getCosts()[it];
        }
    }
    developVertex(g, start, wasDeveloped, bestPathTo, whereWeCameFrom);
    if (bestPathTo[end] == -1) {
        throw NoRoute();
    }
    std::cout << std::endl;
    std::cout << bestPathTo[end];
    std::vector<directions> route = getDierections(whereWeCameFrom, start, end, g.getWidth());
    Path* solution = new Path(route, bestPathTo[end]);
    return solution;
    }



    void DFSAlgo::developVertex(const Graphs::Graph& g, size_t vertex, std::vector<bool>& wasDeveloped,
    std::vector<double>& bestPathTo, std::vector<int>& whereWeCameFrom) const {
        developVertexGlobal(g, vertex, wasDeveloped, bestPathTo, whereWeCameFrom);
        for (auto newVer : g.getAdjList()[vertex]) {
            if (!wasDeveloped[newVer]) {
                developVertex(g, newVer, wasDeveloped, bestPathTo, whereWeCameFrom);
            }
        }

    }


    Path* AstarAlgo::operator()(const Graphs::Graph& g, size_t start, size_t end) const{
    if (start == end) {
        Path* sol = new Path();
        return sol;
    }
    if (((int)start < 0) || ((int)start > g.getSize()) || ((int)end  < 0) || ((int)end > g.getSize())) {
        throw WrongAssignment();
    }
    if (!g.getIsMatrix()) {
            throw WrongGraph();
    }
    
    std::vector<double> bestPathTo;
    std::vector<bool> wasDeveloped;
    std::vector<int> whereWeCameFrom;
    std::vector<double> huristic;
    std::vector<int> vertexPriorityQueue;
    bestPathTo.resize(g.getSize());
    wasDeveloped.resize(g.getSize());
    whereWeCameFrom.resize(g.getSize());
    huristic.resize(g.getSize());
    int endY = end / g.getWidth();
    int endX = end % g.getWidth();

    for (size_t it = 0; it < bestPathTo.size(); ++it) {
        bestPathTo[it] = -1;
        wasDeveloped[it] = false;
        if (it == start) {
            bestPathTo[it] = g.getCosts()[it];
        }

        int posY = it / g.getWidth();
        int posX = it % g.getWidth();
        huristic[it] = std::sqrt(std::pow((double)(posX - endX), 2) + std::pow((double)(posY - endY), 2));
    }
    vertexPriorityQueue.push_back(start);
    while (!vertexPriorityQueue.empty()) {
        double min = std::numeric_limits<double>::max();
        int nextVertexToDevelop;
        for (auto ver : vertexPriorityQueue) {
            if ((bestPathTo[ver] != -1) && (bestPathTo[ver] + huristic[ver] < min)) {
                min = bestPathTo[ver] + huristic[ver];
                nextVertexToDevelop = ver;
            }
        }

        vertexPriorityQueue.erase(std::remove(vertexPriorityQueue.begin(), vertexPriorityQueue.end(), nextVertexToDevelop),
         vertexPriorityQueue.end());
        developVertex(g, nextVertexToDevelop, wasDeveloped, bestPathTo, whereWeCameFrom);
        if (nextVertexToDevelop == (int)end) {
            break;
        }
        for (auto newVer : g.getAdjList()[nextVertexToDevelop]) {
            if (!wasDeveloped[newVer]) {
                if (!std::count(vertexPriorityQueue.begin(), vertexPriorityQueue.end(), newVer)) {
                    vertexPriorityQueue.push_back(newVer);
                }
            }
        }
        for (size_t i = 0; i < vertexPriorityQueue.size(); ++i) {
            int vertex = vertexPriorityQueue[i];
            if (g.getCosts()[vertex] == -1) {
                vertexPriorityQueue.erase(vertexPriorityQueue.begin() + i);
            }
        }

    }
    if (bestPathTo[end] == -1) {
        throw NoRoute();
    }
    std::cout << std::endl;
    std::cout << bestPathTo[end];
    std::vector<directions> route = getDierections(whereWeCameFrom, start, end, g.getWidth());
    Path* solution = new Path(route, bestPathTo[end]);
    return solution;
    }


    void AstarAlgo::developVertex(const Graphs::Graph& g, size_t vertex, std::vector<bool>& wasDeveloped,
     std::vector<double>& bestPathTo, std::vector<int>& whereWeCameFrom) const {
        developVertexGlobal(g, vertex, wasDeveloped, bestPathTo, whereWeCameFrom);
    }


void developVertexGlobal(const Graphs::Graph& g, size_t vertex, std::vector<bool>& wasDeveloped,
     std::vector<double>& bestPathTo, std::vector<int>& whereWeCameFrom) {
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
                bestPathTo[newVer] = g.getCosts()[newVer] + bestPathTo[vertex];
                whereWeCameFrom[newVer] = vertex;

            }
            else {
                if ((g.getCosts()[newVer] + bestPathTo[vertex]) < bestPathTo[newVer]) {
                    bestPathTo[newVer] = g.getCosts()[newVer] + bestPathTo[vertex];
                    whereWeCameFrom[newVer] = vertex;

                }
            }
        }
    }

    const std::vector<directions> Path::getSolutionRoute() const {
        return solutionRoute;
    }

    double Path::getRouteCost() const {
        return routeCost;
    }
    Path::Path(){}
    Path::Path(std::vector<directions> route, double cost) : routeCost(cost), solutionRoute(route) {}

    std::vector<directions> getDierections(std::vector<int>& whereWeCameFrom, size_t start, size_t end, size_t width) {
        std::vector<directions> dierections;
        int currentVertex = end;
        while (currentVertex != (int)start) {
            if (whereWeCameFrom[currentVertex] == currentVertex - (int)width) {
            dierections.push_back(down);
            }
            else if (whereWeCameFrom[currentVertex] == currentVertex + (int)width) {
            dierections.push_back(up);
            }
            else if (whereWeCameFrom[currentVertex] == currentVertex - 1) {
            dierections.push_back(right);
            }
            else if (whereWeCameFrom[currentVertex] == currentVertex + 1) {
            dierections.push_back(left);
            } else {
                throw NoRoute();
            }
            currentVertex = whereWeCameFrom[currentVertex];
        }
        std::reverse(dierections.begin(), dierections.end());
        return dierections;
    }



}




