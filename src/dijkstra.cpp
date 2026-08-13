#include "dijkstra.hpp"
#include <queue>
#include <algorithm>

DijkstraResult dijkstra(const Graph& graph, size_t start) {
    const size_t n = graph.nodes.size();
    DijkstraResult res;
    res.dist.assign(n, std::numeric_limits<double>::infinity());
    res.prev.assign(n, n);

    using NodeDist = std::pair<double, size_t>;
    std::priority_queue<NodeDist, std::vector<NodeDist>, std::greater<NodeDist>> pq;

    res.dist[start] = 0.0;
    pq.push({0.0, start});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > res.dist[u]) continue;

        for (const auto& edge : graph.adj[u]) {
            double nd = d + edge.distance;
            if (nd < res.dist[edge.to]) {
                res.dist[edge.to] = nd;
                res.prev[edge.to] = u;
                pq.push({nd, edge.to});
            }
        }
    }
    return res;
}

std::vector<size_t> reconstruct_path(const DijkstraResult& result, size_t target) {
    std::vector<size_t> path;
    if (!result.reachable(target)) return path;
    for (size_t v = target; v != result.prev.size(); v = result.prev[v]) {
        path.push_back(v);
        if (v == result.prev[v]) break;
    }
    std::reverse(path.begin(), path.end());
    return path;
}