#pragma once
#include <vector>
#include <string>
#include "types.hpp"
#include "json.hpp"

using json = nlohmann::json;

std::unordered_map<std::string, std::string> parse_other_tags(const std::string& other_tags);

json load_json(const std::string& filename);

void extract_roads_and_connections(const json& data, std::vector<Line>& roads, std::vector<Line>& connections);

void extract_points(const json& data, std::vector<std::pair<Point, std::string>>& animals,
                    std::vector<Point>& entrances_main, std::vector<Point>& entrances_side,
                    std::vector<Point>& exits_main, std::vector<Point>& exits_side);

void extract_polygons(const json& data); //На тебя давно так, я не использую полигоны