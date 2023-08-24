/**
 * @author benpr438
 * @brief Compute the area of a simple polygon, and the direction (clockwise or counterclockwise) in which its vertices are given.
 * For each given test case, CW is printed for clockwise, CCW for counter-clockwise, then the absolute area is given.
 * @date 2023-06-03
 * 
 */

#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

#include "../structs.hpp"
using namespace std;
using vvi = vector<vector<long>>;



int main(){

    int n;
    cin >> n;
    while (n) {
        Polygon poly;
        for (int i = 0; i < n; i++) {
            int x, y;
            cin >> x >> y;
            poly.push({x, y});
        }
        
        auto area {poly.area()};
        cout << (area < 0 ? "CW " : "CCW "); // area will be positive if points were given in counterclockwise order.
        cout << setprecision(1) << fixed << fabsf64 (area) << endl;

        cin >> n;
    }

    return 0;
}
