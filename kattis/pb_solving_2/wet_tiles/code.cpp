#include <iostream>
#include <vector>
#include <utility>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;
using vi = vector<long>;
using vvi = vector<vi>;
using pi = pair<int,int>;
using ppi = pair<pi, pi>;
using vpi = vector<pi>;

/*
0: not visited
-1: wall
1 visited

*/

void printv(vvi v) {
    reverse(begin(v), end(v));
    for (auto  row: v) {
        reverse(begin(row), end(row));
        for (auto e: row) {
            cout << e << ' ';
        }
        cout << endl;
    }
}


long bfs(int x, int y, int t, vpi const& leaks, vvi & map) {
    for (auto const& lk : leaks) {
        queue<pi> cells;
        cells.push(lk);
        auto workt = t;
        while (workt != 0) {
            auto s = cells.size();
            while (s != 0) {
                auto cur = cells.front(); cells.pop();
                map[cur.first][cur.second] = 1;
                int left, right, up, bot;
                left = (cur.first -1 >= 0) ? cur.first -1 : 0; 
                right = (cur.first +1 < x) ? cur.first +1 : 0; 
                up = (cur.second -1 >= 0) ? cur.second -1 : 0; 
                bot = (cur.second +1 < y) ? cur.second +1 : 0; 
                if (map[left][cur.second]==0){
                    // map[left][cur.second] = 1;
                    if (workt > 1)
                        cells.emplace(left, cur.second);
                }
                if (map[right][cur.second]==0){
                    // map[right][cur.second] = 1;
                    if (workt > 1)
                        cells.emplace(right, cur.second);
                }
                if (map[cur.first][up]==0){
                    // map[cur.first][up] = 1;
                    if (workt > 1)
                        cells.emplace(cur.first, up);
                }
                if (map[cur.first][bot]==0){
                    // map[cur.first][bot] = 1;
                    if (workt > 1)
                        cells.emplace(cur.first, bot);
                }
                --s;
            }
            --workt;
        }
    }

    int counter = 0;
    for (auto i = 0; i < x; i++) {
        for (auto j=0; j < y; j++) {
            counter += (int) (map[i][j] == 1);
        }
    }
    return counter;
}

int main(){
    size_t x,y,t,l,w;
    while(cin >> x and x != -1)
    {
        cin >> y >> t >> l >> w;
        vpi leaks;
        for (auto i =0; i < l; ++i){
            int x1, y1;
            cin >> x1 >> y1;
            --x1, --y1;
            leaks.emplace_back(x1,y1);
        }

        vvi map(x, vi(y, 0));
        for (auto i = 0; i < w; i++) {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            --x1, --y1, --x2, --y2;
            // cout  << "wall n" << i<< ' ' << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;

            if (x1 == x2){// straight line {
                int dj = (y1 > y2) ? -1 : 1;
                for (auto i = y1; i != y2 +dj; i += dj)
                    map[x1][i] = -1;
            }
            else if (y1 == y2){// straight line {
                int di = (x1 > x2) ? -1 : 1;
                for (auto i = x1; i != x2 +di; i += di)
                    map[i][y1] = -1;
            }
            else{
                int di = (x1 > x2) ? -1 : 1;
                int dj = (y1 > y2) ? -1 : 1;
                map[x2][y2] = -1;
                int i = x1, j = y1;
                while (i != x2 and j != y2) {
                    // cout << "ij: " << i << ' ' << j << endl;
                    map[i][j] = -1;
                    j += dj;
                    i += di;
                }
            }
        }

        cout << bfs(x,y,t,leaks, map) << endl;
    } 
    return 0;
}
