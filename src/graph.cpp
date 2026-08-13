#include "graph.hpp"
#include <map>
#include <cmath>

Graph build_graph(const std::vector<Line>& roades, const std::vector<Line>& connections) {
    Graph g;
    std::map<Point, size_t> point_to_node;

    // получить или создать вершину
    auto get_or_add = [&](const Point& p) -> size_t {
        auto it = point_to_node.find(p);
        if (it != point_to_node.end()) {
            return it->second;
        }
        size_t id = g.nodes.size();
        g.nodes.push_back(p);
        g.adj.push_back({});          // пустой список рёбер
        point_to_node[p] = id;
        return id;
    };

    //1 Собирать вершины из дорожек
    for (const auto& line : roades) {
        if (line.size() < 2) continue;
        size_t prev_id = get_or_add(line[0]);
        for (size_t i = 1; i < line.size(); ++i) {
            size_t curr_id = get_or_add(line[i]);
            double dist = haversine(g.nodes[prev_id], g.nodes[curr_id]);
            g.adj[prev_id].push_back({curr_id, dist});
            g.adj[curr_id].push_back({prev_id, dist});
            prev_id = curr_id;
        }
    }

    //2 Собирать вершины из линий взгляда
    for (const auto& line : connections) {
        if (line.empty()) continue;
        get_or_add(line.front());
        get_or_add(line.back());
    }

    return g;
}