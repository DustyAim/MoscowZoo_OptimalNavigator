#include "attachments.hpp"
#include <map>
#include <limits>
#include <iostream>
#include <algorithm>

std::vector<AttachedAnimal> attach_animals(
    const std::vector<std::pair<Point, std::string>>& animals,
    const std::vector<Line>& connections,
    const Graph& graph)
{
    std::map<Point, size_t> point_to_node;
    for (size_t i = 0; i < graph.nodes.size(); ++i) {
        point_to_node[graph.nodes[i]] = i;
    }

    std::vector<AttachedAnimal> result;
    const double EPS = 1e-5; //допуск

    for (const auto& animal : animals) {
        const Point& animal_pos = animal.first;
        const std::string& name = animal.second;

        std::vector<size_t> node_ids;
        size_t line_count = 0;

        for (const auto& line : connections) {
            if (line.size() < 2) continue;
            const Point& p1 = line.front();
            const Point& p2 = line.back();

            // Проверяем оба конца
            bool matched = false;
            Point road_point;
            if (points_equal(animal_pos, p1, EPS)) {
                road_point = p2;
                matched = true;
            } else if (points_equal(animal_pos, p2, EPS)) {
                road_point = p1;
                matched = true;
            }

            if (matched) {
                ++line_count;
                auto it = point_to_node.find(road_point);
                if (it != point_to_node.end()) {
                    node_ids.push_back(it->second);
                } else {
                    std::cerr << "Предупреждение: конец линии для " << name << " не найден в графе\n";
                }
            }
        }

        // Удаляем дубликаты
        std::sort(node_ids.begin(), node_ids.end());
        node_ids.erase(std::unique(node_ids.begin(), node_ids.end()), node_ids.end());

        if (line_count > 0 && !node_ids.empty()) {
            result.push_back({name, node_ids});
            std::cout << "Для " << name << " найдено " << line_count << " линий, уникальных вершин: " << node_ids.size() << "\n";
        } else {
            size_t nearest = 0;
            double min_dist = std::numeric_limits<double>::max();
            for (size_t i = 0; i < graph.nodes.size(); ++i) {
                double d = haversine(animal_pos, graph.nodes[i]);
                if (d < min_dist) {
                    min_dist = d;
                    nearest = i;
                }
            }
            result.push_back({name, {nearest}});
            std::cerr << "Предупреждение: для " << name << " не найдена линия-привязка, привязан к ближайшей вершине (" << min_dist << " м)\n";
        }
    }
    return result;
}