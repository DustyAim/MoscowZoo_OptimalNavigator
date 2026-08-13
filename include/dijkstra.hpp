#pragma once
#include "graph.hpp"
#include <vector>
#include <limits>

struct DijkstraResult {
    std::vector<double> dist;
    std::vector<size_t> prev;
    bool reachable(size_t node) const {
        return dist[node] < std::numeric_limits<double>::infinity();
    }
};

DijkstraResult dijkstra(const Graph& graph, size_t start);
std::vector<size_t> reconstruct_path(const DijkstraResult& result, size_t target);