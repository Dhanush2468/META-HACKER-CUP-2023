#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

void solve() {
  int P, k = 0;
  vector<int> a(41), ans;
  a[k] = 41;
  cin >> P;
  for (;;) {
    // Update ans with current partition a[0..k].
    int p = accumulate(a.begin(), a.begin() + k + 1, 1, multiplies<int>());
    if (p == P && (ans.empty() || k + 1 < (int)ans.size())) {
      ans.assign(a.begin(), a.begin() + k + 1);
    }
    // Generate the next partition.
    int rem_val = 0;
    while (k >= 0 && a[k] == 1) {
      rem_val += a[k];
      k--;
    }
    if (k < 0) {
      break;
    }
    a[k]--;
    rem_val++;
    while (rem_val > a[k]) {
      a[k + 1] = a[k];
      rem_val = rem_val - a[k];
      k++;
    }
    a[k + 1] = rem_val;
    k++;
  }
  if (ans.empty()) {
    cout << " -1";
  } else {
    for (int v : ans) {
      cout << " " << v;
    }
  }
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    cout << "Case #" << t << ":";
    solve();
    cout << endl;
  }
  return 0;
}