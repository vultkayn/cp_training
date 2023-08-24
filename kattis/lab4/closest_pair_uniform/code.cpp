/**
 * @author benpr438
 * @brief Given a set of points (a scatter) in a 2D space [-1e5, 1e5]², find the closest pair,
 * i.e. find any two points whose distance to each other is minimal.
 * The assumption is made that the points are uniformly distributed in the available space.
 * @date 2023-06-03
 * 
 */

#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <cmath>
using namespace std;
using vvi = vector<vector<long>>;

#include "../structs.hpp"



int main()
{
    int n;
    std::cin >> n;
    cout.precision(3);
    while (n > 0)
    {
        scatter_t points(n);
        for (int i = 0; i < n; i++)
        {
            double x, y;
            std::cin >> x >> y;
            points[i] = {x, y};
        }

        auto [p1, p2] = closest_pair(points);

        std::cout << fixed << p1 << " " << p2 << endl;
        n = 0;
        std::cin >> n;
    }

    return 0;
}
