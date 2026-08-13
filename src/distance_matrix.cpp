#include "distance_matrix.hpp"
#include "dijkstra.hpp"

std::vector<std::vector<double>> build_distance_matrix(const Graph& graph, const std::vector<size_t>& node_ids) {
    size_t n = node_ids.size();
    std::vector<std::vector<double>> distances(n, std::vector<double>(n, 0.0));

    // Для каждой вершин
    for (size_t i = 0; i < n; ++i) {
        DijkstraResult result = dijkstra(graph, node_ids[i]);
        for (size_t j = 0; j < n; ++j) {
            distances[i][j] = result.dist[node_ids[j]];
        }
    }
    return distances;
}