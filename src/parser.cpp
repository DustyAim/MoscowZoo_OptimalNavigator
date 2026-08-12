//
// Created by Sholota Dmitriy on 12.08.2026.
//

#include "parser.hpp"
#include <fstream>
#include <regex>
#include <stdexcept>
#include <string>


std::unordered_map<std::string, std::string> parse_other_tags(const std::string& other_tags) {
    std::unordered_map<std::string, std::string> result;
    std::regex pattern("\"([^\"]+)\"\\s*=>\\s*\"([^\"]*)\"");
    std::sregex_iterator iter(other_tags.begin(), other_tags.end(), pattern);
    std::sregex_iterator end;
    for (; iter != end; ++iter) {
        std::smatch match = *iter;
        if (match.size() == 3) {
            result[match[1].str()] = match[2].str();
        }
    }
    return result;
}

json load_json(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()){
        throw std::runtime_error("Couldn't open a data file:" + filename);
    }
    // Чтение теперь файла загруженного
    std::string content;
    std::string line;
    while (std::getline(file, line)) {  // While True
        content += line;
    }
    file.close();
    return json::parse(content);
}

void extract_roads_and_connections(const json& data,
                                   std::vector<Line>& roads,
                                   std::vector<Line>& connections){
    if (!data.contains("features"))   //Проверка на содержание файла
        return;

    //Теперь проходим по всем строчкам которые содержат features а далее ветвеление
    for (const auto& feature : data["features"]) {
        if (!feature.contains("geometry") || feature["geometry"]["type"] != "LineString")
            continue;

        const auto& coords = feature["geometry"]["coordinates"];
        Line line;
        line.reserve(coords.size());
        //Теперь нужно преобразовать набор координат в линии в точки.
        for (const auto& coord : coords) {
            double lon = coord[0].get<double>();
            double lat = coord[1].get<double>();
            line.push_back(Point(lon, lat));
        }

        //Заполнили вестор точками но теперь их надо поделить по их типам к чему они относятся
        const auto& props = feature["properties"];
        if (props.contains("highway") && props["highway"] == "footway")
            roads.push_back(std::move(line));
        else if (props.contains("other_tags")) {
            auto tags = parse_other_tags(props["other_tags"].get<std::string>());
            if (tags.find("purpose") != tags.end() && tags["purpose"] == "entry")
                connections.push_back(std::move(line));
        }
    }
}

void extract_polygons(const json& data){
    // Пока ничего не делаем
}

void extract_points(const json& data,
                    std::vector<std::pair<Point, std::string>>& animals,
                    std::vector<Point>& entrances_main,
                    std::vector<Point>& entrances_side,
                    std::vector<Point>& exits_main,
                    std::vector<Point>& exits_side){
    if (!data.contains("features"))   //Проверка на содержание файла
        return;

    for (const auto& feature : data["features"]) {
        if (!feature.contains("geometry") || feature["geometry"]["type"] != "Point")
            continue;

        const auto& coord = feature["geometry"]["coordinates"];
        Point point(coord[0].get<double>(), coord[1].get<double>());

        const auto& props = feature["properties"];

        // Вспомогательная лямбда для поиска тега (сначала в props, потом в other_tags)
        auto get_tag = [&](const std::string& key) -> std::string {
            if (props.contains(key)) {
                return props[key].get<std::string>();
            }
            if (props.contains("other_tags")) {
                auto tags = parse_other_tags(props["other_tags"].get<std::string>());
                auto it = tags.find(key);
                if (it != tags.end()) return it->second;
            }
            return "";
        };

        std::string attraction = get_tag("attraction");
        std::string entrance   = get_tag("entrance");
        std::string exit       = get_tag("exit");
        std::string name       = props.contains("name") ? props["name"].get<std::string>() : "";

        // Вольер
        if (attraction == "animal") {
            animals.push_back({point, name});
        }

        // Входы
        if (entrance == "main")
            entrances_main.push_back(point);
        else if (entrance == "side")
            entrances_side.push_back(point);

        // Выходы
        if (exit == "main")
            exits_main.push_back(point);
        else if (exit == "side")
            exits_side.push_back(point);
    }
}