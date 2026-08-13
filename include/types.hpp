//
// Файл, который описывает типы объектов, которые будут применяться во всем проекте
// Точки, Линии, Графы
//
#pragma once
#include <vector>
#include <string>
#include <cmath>

struct Point {
    double lon;
    double lat;

    Point() : lon(0), lat(0) {}
    Point(double lon_, double lat_) : lon(lon_), lat(lat_) {}

    bool operator<(const Point& other) const {
        if (lon != other.lon) return lon < other.lon;
        return lat < other.lat;
    }
    bool operator==(const Point& other) const {
        return lon == other.lon && lat == other.lat;
    }
};

struct Edge {
    size_t to;          // индекс вершины
    double distance;    // длина ребра в метрах
};

// Функция сравнения точек с допуском
inline bool points_equal(const Point& a, const Point& b, double eps = 1e-8) {
    return std::abs(a.lon - b.lon) < eps && std::abs(a.lat - b.lat) < eps;
}

// Функция гаверсинуса для расстояния между двумя точками в метрах
inline double haversine(const Point& a, const Point& b) {
    const double R = 6371000.0;
    double dlat = (b.lat - a.lat) * M_PI / 180.0;
    double dlon = (b.lon - a.lon) * M_PI / 180.0;
    double lat1 = a.lat * M_PI / 180.0;
    double lat2 = b.lat * M_PI / 180.0;
    double sin_dlat = sin(dlat / 2);
    double sin_dlon = sin(dlon / 2);
    double a_ = sin_dlat * sin_dlat + cos(lat1) * cos(lat2) * sin_dlon * sin_dlon;
    double c = 2 * atan2(sqrt(a_), sqrt(1 - a_));
    return R * c;
}

using Line = std::vector<Point>;