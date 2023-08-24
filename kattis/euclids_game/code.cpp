#include <iostream>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;

int pgcd (int lhs, int rhs) {
    int a = lhs;
    int b = rhs;
    int nsteps;
    while (b != 0) {
        if (b <= a)
            nsteps++;
        int t = b;
        b = a % b;
        a = t;
    }
    return nsteps;
}

// idea: do pgcd and count number of iterations, if odd Stan wins, if even its Obbie's win!
int main(){
    int lhs, rhs;
    while (cin >> lhs >> rhs) {        
        if (lhs == 0 and rhs == 0)
            break;
        auto nsteps = pgcd(lhs, rhs);
        cout << ((nsteps & 1) ? "Stan" : "Ollie")  << " wins" << endl;
    }

    return 0;
}
