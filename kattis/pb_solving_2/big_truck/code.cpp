#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
using namespace std;
constexpr long INF = 10'000'000;
struct road {
    int u,v,w,x;
    road(int u, int v, int w, int x) : u{u}, v{v}, w{w}, x{x} {}
    road(int u, int v, int w) : u{u}, v{v}, w{w}, x{0} {}
    road() : u{-1}, v{-1}, w{-1}, x{0} {}
    bool operator<(road o) const {return w < o.w;}
};

struct conn {
    int cost;
    int node;
    conn(int c, int n) : cost{c}, node{n} {}
    bool operator<(conn o) const {return cost < o.cost;}
};

using vvi = vector<vector<long>>;
using vi = vector<long>;
using ve = vector<road>;
using vve = vector<ve>;
using pii = pair<long,long>;
using pqc = priority_queue<conn>;



pii belmmanford_max(vve const& adj_list, size_t n, vi const& pickups) {
    ve parents(n);
    auto start = n-1;
    vi distances(n, INF);
    distances[start] = 0;
    for (auto i =0; i < n-1; i++) {
        auto updated = false;
        for (auto ev : adj_list) {
            for (auto edg: ev){
                if (distances[edg.u] < INF) {
                    auto offer = distances[edg.u] + edg.w;
                    if (offer > distances[edg.v]) {
                        updated = true;
                        distances[edg.v] = offer;
                        parents[edg.v] = edg;
                    }
                }
            }
        }
    }

    auto length = 0;
    for (auto cur = 0; cur != -1;) {
        auto prev = parents[cur].u;
        length += parents[cur].x;
        cur = prev;
    }


    return {length, distances[0] + pickups[n-1]};
}

pii dijkstra(vve const& adj_list, size_t n, vi const& pickups) {
    vve parents(n);
    pqc prioQ;
    prioQ.emplace(0, 0);
    vi costs(n, INF);
    bool visited[n];
    fill(visited, visited +n, false);
    while (!prioQ.empty()) {
        auto conn = prioQ.top(); prioQ.pop();
        auto c = conn.cost;
        auto n = conn.node;
        visited[n] = true;
        for (auto edg : adj_list[n]) {
            auto new_cost = edg.w + c;
            if (not visited[edg.v]) {
                if (new_cost < costs[edg.v]) {
                    prioQ.emplace(new_cost, edg.v);
                    costs[edg.v] = new_cost;
                    parents[edg.v].clear();
                    parents[edg.v].emplace_back(edg.u, edg.v, pickups[edg.u], edg.w);
                } else if (new_cost == costs[edg.v]) {
                    parents[edg.v].emplace_back(edg.u, edg.v, pickups[edg.u], edg.w);
                }
            }
        }
    }

    if (parents[n].empty()) return {-1, -1};

    return belmmanford_max(parents, n, pickups);

    // reconstruct dfs
/*    auto length = -1, cost = -1;
    auto cur = n -1;
    queue<edge> nodes;
    vi nb_parents(n);
    nb_parents[cur] = parents[cur].size(); */


/*     for (auto par : parents[cur])
        nodes.push(par);
    auto length = -1;
    auto picked = pickups[cur];
    auto computed = false;
    while (not nodes.empty()) {
        auto edg = nodes.front(); nodes.pop();
        auto cur = edg.u;
        nb_parents[edg.v]--;
        if (not computed) length += edg.w;

        if (not parents[cur].empty()){
            nb_parents[cur] = parents[cur].size();
            for (auto par : parents[cur])
                nodes.push(par);
        }    
    } */

}


int main(){
    size_t n;
    cin >> n;

    vi pickups(n);
    for (auto i =0, ti = 0; i < n and cin >> ti; ++i) {
        pickups[i] = ti;
    }
    size_t m;
    cin >> m;
    vve adj_list(n);
    for (auto i=0; i < m; i++) {
        long a,b,d;
        cin >> a >> b >> d;
        --a, --b;
        adj_list[a].emplace_back(a,b,d);
        adj_list[b].emplace_back(b,a,d);
    }

    auto res = dijkstra(adj_list, n, pickups);
    auto len = res.first;
    auto items = res.second;

    if (len == -1) {
        cout << "impossible" << endl;
    } else {
        cout << len << ' ' << items << endl;
    }

    return 0;
}
