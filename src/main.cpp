#include <iostream>
#include <vector>
#include "parser.hpp"
#include "graph.hpp"
#include "attachments.hpp"
#include "vizualize_graph.hpp"
#include "dijkstra.hpp"

int main() {
    try {
        std::cout << "Загрузка lines.geojson...\n";
        json lines_data = load_json("data/lines.geojson");
        std::cout << "Загрузка points.geojson...\n";
        json points_data = load_json("data/points.geojson");

        //Извлекаем дороги и линии-привязки
        std::vector<Line> roads;
        std::vector<Line> connections;
        extract_roads_and_connections(lines_data, roads, connections);

        // Извлекаем точки вольеров, входы и выходы
        std::vector<std::pair<Point, std::string>> animals;
        std::vector<Point> entrances_main, entrances_side, exits_main, exits_side;
        extract_points(points_data, animals, entrances_main, entrances_side, exits_main, exits_side);


        std::cout << "Дорог (footway): " << roads.size() << '\n';
        std::cout << "Линий-привязок: " << connections.size() << '\n';
        std::cout << "Вольеров: " << animals.size() << '\n';
        std::cout << "Входов (main): " << entrances_main.size() << '\n';
        std::cout << "Входов (side): " << entrances_side.size() << '\n';
        std::cout << "Выходов (main): " << exits_main.size() << '\n';
        std::cout << "Выходов (side): " << exits_side.size() << '\n';

        visualize_raw_data(roads, connections, animals, entrances_main, entrances_side, exits_main, exits_side, "zoo_raw.html");
        Graph graph = build_graph(roads, connections);

        std::cout << "\n=== ГРАФ ===\n";
        std::cout << "Узлов: " << graph.nodes.size() << '\n';
        size_t edge_count = 0;
        for (const auto& adj : graph.adj) edge_count += adj.size();
        edge_count /= 2; // каждое ребро добавлено дважды
        std::cout << "Рёбер: " << edge_count << '\n';
        auto attached = attach_animals(animals, connections, graph);

        std::cout << "\n=== ПРИВЯЗКА ЖИВОТНЫХ ===\n";
        std::cout << "Привязано животных: " << attached.size() << '\n';
        for (size_t i = 0; i < attached.size(); ++i) {
            std::cout << attached[i].name << " -> вершины: ";
            for (size_t node : attached[i].node_ids) {
                std::cout << node << " ";
            }
            std::cout << '\n';
        }

        if (attached.size() >= 2) {
            size_t start_id = attached[0].node_ids[0];
            size_t target_id = attached[1].node_ids[0];

            auto result = dijkstra(graph, start_id);
            if (result.reachable(target_id)) {
                std::cout << "\n=== ТЕСТ ДЕЙКСТРЫ ===\n";
                std::cout << "Расстояние от " << attached[0].name
                          << " до " << attached[1].name
                          << ": " << result.dist[target_id] << " м\n";

                auto path = reconstruct_path(result, target_id);
                std::cout << "Путь: ";
                for (size_t node : path) {
                    std::cout << node << " ";
                }
                std::cout << "\n";
            } else {
                std::cout << "Нет пути между " << attached[0].name << " и " << attached[1].name << "\n";
            }
        }

    } catch (const std::exception& e) { return 1; }
    return 0;
}