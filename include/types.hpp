//
// Файл, которые описывает типы обьектов которые будут применяется во всем проекте
// Точки Линии путей Графы
#pragma once
#include <vector>
#include <string>

struct Point {   // Структура обработка координат (точки или конца вектора) Узлы графа
    double lon;  // долгота
    double lat;  // широта

    Point (): lon(0), lat(0) {}
    Point (double lon, double lat): lon(lon), lat(lat) {}

    bool operator<(const Point& other) const {
        if (lon != other.lon) return lon < other.lon;
        return lat < other.lat;
    }
    bool operator==(const Point& other) const {
        return lon == other.lon && lat == other.lat;
    }
};


using Line = std::vector<Point>;  //Псевдониим std::vector<Point>