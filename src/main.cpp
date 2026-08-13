#include <iostream>
#include <vector>
#include "parser.hpp"
#include "graph.hpp"
#include "attachments.hpp"

int main() {
    try {
        // 1. Загружаем файлы
        std::cout << "Загрузка lines.geojson...\n";
        json lines_data = load_json("data/lines.geojson");
        std::cout << "Загрузка points.geojson...\n";
        json points_data = load_json("data/points.geojson");

        // 2. Извлекаем дороги и линии-привязки
        std::vector<Line> roads;
        std::vector<Line> connections;
        extract_roads_and_connections(lines_data, roads, connections);

        // 3. Извлекаем точки вольеров, входы и выходы
        std::vector<std::pair<Point, std::string>> animals;
        std::vector<Point> entrances_main, entrances_side, exits_main, exits_side;
        extract_points(points_data, animals, entrances_main, entrances_side, exits_main, exits_side);

        // 4. Выводим статистику
        std::cout << "Дорог (footway): " << roads.size() << '\n';
        std::cout << "Линий-привязок: " << connections.size() << '\n';
        std::cout << "Вольеров: " << animals.size() << '\n';
        std::cout << "Входов (main): " << entrances_main.size() << '\n';
        std::cout << "Входов (side): " << entrances_side.size() << '\n';
        std::cout << "Выходов (main): " << exits_main.size() << '\n';
        std::cout << "Выходов (side): " << exits_side.size() << '\n';

        // 5. Строим граф из дорог и добавляем вершины из линий-привязок
        Graph graph = build_graph(roads, connections);

        std::cout << "\n=== ГРАФ ===\n";
        std::cout << "Узлов: " << graph.nodes.size() << '\n';
        size_t edge_count = 0;
        for (const auto& adj : graph.adj) edge_count += adj.size();
        edge_count /= 2; // каждое ребро добавлено дважды
        std::cout << "Рёбер: " << edge_count << '\n';

        // 6. Привязываем животных к вершинам графа через линии-привязки
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


        // Здесь можно будет добавить тест Дейкстры или TSP позже

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << '\n';
        return 1;
    }
    return 0;
}