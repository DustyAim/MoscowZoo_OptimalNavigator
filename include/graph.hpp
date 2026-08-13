#pragma once
#include <algorithm>
#include "types.hpp"
#include <vector>
#include <map>

struct Graph {
    std::vector<Point> nodes;
    std::vector<std::vector<Edge>> adj;
    };

Graph build_graph(const std::vector<Line>& rodes, const std::vector<Line>& connections);


