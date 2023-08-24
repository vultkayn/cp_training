#include <iostream>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;



int solve (int N) {
    if (N == 1) return 1;
    long long prev3 = 1; // n == 1

    int two_exponent = 0;
    int five_exponent = 0;

    for (int n = 2; n <= N; n++) {
        prev3 *= n;
        while (prev3 % 10 == 0)
            prev3 /= 10;

        // issue detected from 49 to 50.
        // 49! ends with 1024, but then 24 * 50 = 1200 -> 012, when 1024 * 50 = 51200 -> 512 which is correct
        // we can see that if there is a "reaction" between prev3 and n, such as a factor 10 appear, then we can miss relevant digits
        // hence we remove all 5 and 2 !
        // by modulo's compatibility with scaling, we can readjust prev3 later (at the end)

        while (prev3 % 5 == 0)
            prev3 /= 5, five_exponent++;
        while (prev3 % 2 == 0)
            prev3 /= 2, two_exponent++;

        prev3 %= 1'000; // since it differs when we allow more digits, some to the far left are probably disappearing. might come from a shift by 10 ?
    }

    // readjust prev3. Obviously there are much more 2s than 5s (2, 4, 6, 8, 10 vs 5, 10)
    two_exponent -= five_exponent;
    while (two_exponent--) {
        prev3 *= 2;
        prev3 %= 1'000;
    }

    return prev3;
}

int main(){
    int n;
    cin >> n;

    auto d3 = solve(n);
    string buff = d3 < 10 and n >= 7 ? "00" : (d3 < 100 and n >= 7 ? "0": ""); 
    cout << buff << d3 << endl;
    return 0;
}
