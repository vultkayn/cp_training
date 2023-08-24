#include <iostream>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;

#include "../structs.hpp"

int main(){
    int n;
    cin >> n;
    for (int i =0; i < n; i++) {
        int x1, y1, x2, y2, x3, y3, x4, y4;
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;

        Line l1 {{x1, y1}, {x2, y2}};
        Line l3 {{x3, y3}, {x4, y4}};

        l1.distanceTo(l2);
    }

    return 0;
}
