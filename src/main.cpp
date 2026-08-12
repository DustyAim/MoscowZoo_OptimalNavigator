#include <iostream>
#include <vector>
#include "parser.hpp"
#include "types.hpp"

int main() {
    try {
        std::cout << "Загрузка lines.geojson...\n";
        json lines_data = load_json("data/lines.geojson");
        
        std::cout << "Загрузка points.geojson...\n";
        json points_data = load_json("data/points.geojson");

        std::vector<Line> roads, connections;
        extract_roads_and_connections(lines_data, roads, connections);

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

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << '\n';
        return 1;
    }
    return 0;
}