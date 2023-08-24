#include <iostream>
#include <type_traits>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;

enum class cell_t : char {
    Trap = 'T',
    Wall = '#',
    Player = 'P',
    Free = '.',
    Gold = 'G'
};

struct cell {
    cell_t type;
    bool visited = false;
};

cell_t cast(char c) {
    return static_cast<cell_t>(c);
}

using map_t = vector<vector<cell>>;

bool feelTrap(short x, short y, map_t const& map) {
    auto l = max(x-1, 0);
    auto r = min((short) map[0].size()-1, x+1);
    auto u = max(y-1, 0);
    auto d = min((short) map.size()-1, y+1);

    return map[y][l].type == cell_t::Trap or map[y][r].type == cell_t::Trap
        or map[u][x].type == cell_t::Trap or map[d][x].type == cell_t::Trap;
}

int collect(map_t & map, short x, short y ) {
    auto l = max(x-1, 0);
    auto r = min((short) map[0].size()-1, x+1);
    auto u = max(y-1, 0);
    auto d = min((short) map.size()-1, y+1);

    if (map[y][x].visited) return 0;

    int coins = (int) (map[y][x].type == cell_t::Gold);
    map[y][x].type = cell_t::Free;
    map[y][x].visited = true;
    if (feelTrap (x, y, map))
    {
        return coins;
    }

    // go left
    if (l != x and map[y][l].type != cell_t::Wall)
        coins += collect(map, l, y);
    if (r != x and map[y][r].type != cell_t::Wall)
        coins += collect(map, r, y);
    if (u != y and map[u][x].type != cell_t::Wall)
        coins += collect(map, x, u);
    if (d != y and map[d][x].type != cell_t::Wall)
        coins += collect(map, x, d);
    
    return coins;
}

int main(){
    int W, H;
    cin >> W >> H;
    map_t map (H, vector<cell>(W));
    int Px, Py;
    for (int h = 0; h < H; h++)
        for (int w = 0; w < W; w++) {
            char c;
            cin >> c;
            map[h][w].type = cast(c);
            if (cast(c) == cell_t::Player) Px = w, Py = h;
        }

    cout << collect(map, Px, Py) << endl;
    return 0;
}
