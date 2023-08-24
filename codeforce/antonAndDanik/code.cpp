  #include <iostream>
  #include <vector>
  using namespace std;
  using vvi = vector<vector<long>>;

  int main(){
    int n;
    cin >> n;
    char winner;
    int wins = 0;
    while (cin >> winner)
      wins += winner == 'A' ? 1 : -1;
    
    if (wins == 0)
      cout << "Friendship" << endl;
    else if (wins < 0)
      cout << "Danik" << endl;
    else
      cout << "Anton" << endl;
    return 0;
  }
