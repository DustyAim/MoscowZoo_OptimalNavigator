//Ставим соответствие животное - взгляд

#pragma once
#include "types.hpp"
#include "graph.hpp"
#include <vector>
#include <string>

struct AttachedAnimal {
    std::string name;
    std::vector<size_t> node_ids;
};

std::vector<AttachedAnimal> attach_animals(const std::vector<std::pair<Point, std::string>>& animals, const std::vector<Line>& connections, const Graph& graph);   // граф уже содержит все вершины