#pragma once
#include "types.hpp"
#include "json.hpp"
#include <string>
#include <vector>

void visualize_raw_data(
    const std::vector<Line>& roads,
    const std::vector<Line>& connections,
    const std::vector<std::pair<Point, std::string>>& animals,
    const std::vector<Point>& entrances_main,
    const std::vector<Point>& entrances_side,
    const std::vector<Point>& exits_main,
    const std::vector<Point>& exits_side,
    const std::string& filename = "zoo_raw.html"
);