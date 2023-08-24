#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
using vvi = vector<vector<long>>;


struct Song {
    string name;
    unsigned long q;
    size_t date;
    Song (string name, unsigned long q, size_t idx) : name{name}, q{q}, date{idx} {}

};
ostream& operator<<(ostream& os, Song const& song) {
    os << song.name << " quality: " << song.q << " date: " << song.date;
    return os;
} 

int main(){
    int n,m;
    cin >> n >> m;
    unsigned long f;
    string song;
    cin >> f >> song;
    unsigned long f0 = f;
    vector<Song> songs {Song (song, f0, 0)};
    for (size_t i = 1; i < n; i++) {
        cin >> f >> song;
        // double z = ((double)f0) / ((double) i+1);
        // cerr << i << " z:" << z << " ";
        // cerr << "q:" << q << endl;
        unsigned long q = f * (i+1); // no need to divide by f0 if they all are
        songs.emplace_back(song, q, i);
    }

    auto comp = [](Song a, Song b) {return (a.q > b.q) or (a.q == b.q and a.date < b.date);};
    sort (songs.begin(), songs.end(), comp);

    for (int i=0; i < m; i++) {
        // cerr << songs[i] << endl;
        cout << songs[i].name << endl;
    }

    return 0;
}
