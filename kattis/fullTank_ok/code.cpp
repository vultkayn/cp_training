#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple>
#include <limits>
#include <utility>
using namespace std;
using vvi = vector<vector<long>>;

struct road_t {
    int to;
    int w;

    road_t (int to, int w) : to {to}, w{w} {}
};


struct city_cost_fuel {
    int city;
    int cost;
    int fuel;

    city_cost_fuel(int city, int cost, int fuel) : city{city}, cost{cost}, fuel{fuel} {}
};

bool operator>(city_cost_fuel const& lhs, city_cost_fuel const& rhs) {
    return lhs.cost > rhs.cost;
}

// V = 1000 at most so apsp not doable
// rather have to do a sssp (dijsktra) for each query


// using cost_city_fuel_t = tuple<int, int, int>;



int dijsktra(vector<vector<road_t>> const& adj_list, vector<int> const& prices, int V, int s, int c, int t) {
    vector<vector<int>> costs (V, vector<int>(c+1, numeric_limits<int>::max()));
    priority_queue<city_cost_fuel, vector<city_cost_fuel>, greater<city_cost_fuel>> prioQ;

    costs[s][0] = 0;
    prioQ.emplace(s, 0, 0);

    while (not prioQ.empty()) {
        auto [city, cost, fuel] {prioQ.top()};
        prioQ.pop();

        if (cost > costs[city][fuel]) continue; // current city with fuel cost has been updated in the meantime, do not proceed further, we are not dealing with local optimal.
        // is is better to buy one unit of fuel at current city ? or is it best to arrive with more fuel ?
        if (fuel + 1 <= c and cost + prices[city] < costs[city][fuel + 1])
        {
            prioQ.emplace(city, cost + prices[city], fuel+1);
            costs[city][fuel+1] = cost + prices[city];
        }

        for (auto road: adj_list[city]) {
            auto [neigh, dist] {road};

            if (dist > fuel) continue; // not enough fuel at the moment to go there.

            if (cost < costs[neigh][fuel - dist]) {
                costs[neigh][fuel - dist] = cost;
                prioQ.emplace(neigh, cost, fuel - dist);
            }
        }
    }

    auto m = min_element(costs[t].begin(), costs[t].end());
    return *m == numeric_limits<int>::max() ? -1 : *m;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int n, m;
    cin >> n >> m;
    
    vector<int> prices;
    for (int i =0; i < n; i++)
    {
        int p;
        cin >> p;
        prices.push_back(p);
    }


    vector<vector<road_t>> adj_list (n);
    for (int i=0; i < m; i++){
        int u, v, d;
        cin >> u >> v >> d;
        adj_list[u].emplace_back(v, d);
        adj_list[v].emplace_back(u, d);
    }

    int q;
    cin >> q;
    for (int i =0; i < q; i++) {
        int c, s, t;
        cin >> c >> s >> t;
        int cost = dijsktra(adj_list, prices, n, s, c, t);
        if (cost < 0)
            cout << "impossible\n";
        else
            cout << cost << "\n";
    }

    cout << flush;

    return 0;
}
