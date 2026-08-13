#include "vizualize_graph.hpp"
#include <fstream>
#include <iostream>

void visualize_raw_data(
    const std::vector<Line>& roads,
    const std::vector<Line>& connections,
    const std::vector<std::pair<Point, std::string>>& animals,
    const std::vector<Point>& entrances_main,
    const std::vector<Point>& entrances_side,
    const std::vector<Point>& exits_main,
    const std::vector<Point>& exits_side,
    const std::string& filename)
{
    std::ofstream html(filename);
    if (!html.is_open()) {
        std::cerr << "Не удалось создать файл " << filename << "\n";
        return;
    }

    html << R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <title>Зоопарк - сырые данные</title>
    <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
    <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
    <style>
        #map { height: 90vh; }
        body { margin: 0; padding: 0; }
    </style>
</head>
<body>
    <div id="map"></div>
    <script>
        var map = L.map('map').setView([55.76194, 37.57722], 16);
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            attribution: '© OpenStreetMap'
        }).addTo(map);
)";

    // ---- 1. Дороги (footway) - серые линии ----
    html << "        // Дороги (footway)\n";
    for (const auto& line : roads) {
        if (line.size() < 2) continue;
        html << "        L.polyline([";
        for (size_t i = 0; i < line.size(); ++i) {
            html << "[" << line[i].lat << ", " << line[i].lon << "]";
            if (i < line.size() - 1) html << ", ";
        }
        html << "], {color: '#888888', weight: 2, opacity: 0.7}).addTo(map);\n";
    }

    // ---- 2. Линии-привязки (purpose=entry) - зелёные ----
    html << "        // Линии-привязки (purpose=entry)\n";
    for (const auto& line : connections) {
        if (line.size() < 2) continue;
        html << "        L.polyline([";
        for (size_t i = 0; i < line.size(); ++i) {
            html << "[" << line[i].lat << ", " << line[i].lon << "]";
            if (i < line.size() - 1) html << ", ";
        }
        html << "], {color: '#00aa00', weight: 2, opacity: 0.8}).addTo(map);\n";
    }

    // ---- 3. Вольеры (животные) - синие точки с подписями ----
    html << "        // Вольеры\n";
    for (const auto& animal : animals) {
        const Point& p = animal.first;
        const std::string& name = animal.second;
        html << "        L.circleMarker([" << p.lat << ", " << p.lon << "], {radius: 5, color: '#0066ff', fillColor: '#0066ff', fillOpacity: 0.8}).addTo(map).bindPopup('" << name << "');\n";
    }

    // ---- 4. Входы (main) - красные ----
    html << "        // Входы main\n";
    for (const auto& p : entrances_main) {
        html << "        L.circleMarker([" << p.lat << ", " << p.lon << "], {radius: 6, color: '#ff0000', fillColor: '#ff0000', fillOpacity: 0.8}).addTo(map).bindPopup('Вход main');\n";
    }
    // Входы side - оранжевые
    html << "        // Входы side\n";
    for (const auto& p : entrances_side) {
        html << "        L.circleMarker([" << p.lat << ", " << p.lon << "], {radius: 6, color: '#ff8800', fillColor: '#ff8800', fillOpacity: 0.8}).addTo(map).bindPopup('Вход side');\n";
    }
    // Выходы main - красные с крестиком (но можно просто другие цвета)
    html << "        // Выходы main\n";
    for (const auto& p : exits_main) {
        html << "        L.circleMarker([" << p.lat << ", " << p.lon << "], {radius: 6, color: '#cc0000', fillColor: '#cc0000', fillOpacity: 0.8, stroke: true}).addTo(map).bindPopup('Выход main');\n";
    }
    // Выходы side - оранжевые
    html << "        // Выходы side\n";
    for (const auto& p : exits_side) {
        html << "        L.circleMarker([" << p.lat << ", " << p.lon << "], {radius: 6, color: '#cc8800', fillColor: '#cc8800', fillOpacity: 0.8}).addTo(map).bindPopup('Выход side');\n";
    }

    html << R"(
    </script>
</body>
</html>
)";
    html.close();
    std::cout << "Визуализация сырых данных сохранена в " << filename << "\n";
}