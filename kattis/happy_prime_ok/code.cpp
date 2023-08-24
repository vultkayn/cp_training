#include <iostream>
#include <cmath>
#include <bitset>
using ui = unsigned;
using namespace std;

#define sqdigits(digits) (digits[0]*digits[0] + digits[1]*digits[1] + digits[2]*digits[2] + digits[3]*digits[3] + digits[4]*digits[4])

bool isPrime(short m) {
    if ((!(m & 1) and m != 2) or !m or m == 1) return false;

    unsigned sqm = sqrt(m);
    for (ui i =3; i <= sqm ; i+=2)
        if (!(m%i)) return false;
    
    return true;
}

bool isHappy(short m) {
    if (m == 1 or m == 10 or m == 100 or m == 1000 or m == 10000) return true;
    bitset<10000> found;
    found.set(false);
    uint8_t digits[8];
    while (not found[m]) {
        found[m] = true;
        ((uint64_t *) digits)[0] = 0; // one instruction on 64 bits cpu
        digits[4] = m == 10000;
        digits[3] = m / 1000 - digits[4];
        digits[2] = (m % 1000) / 100;
        digits[1] = (m % 100) / 10;
        digits[0] = m % 10;
        m = sqdigits(digits);
        if (m == 1) return true;
    }
    return false;
}

int main(){
    short P;
    cin >> P;
    for (auto i =0; i < P; i++) {
        short nb, m;
        cin >> nb >> m;
        cout << nb << ' ' << m << ((isPrime(m) and isHappy(m)) ? " YES" : " NO") << endl;
    } 

    return 0;
}
