#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int curr_sum, best_len;
vector<int> divisors, curr, best_ans;

int maxsteps = 0;
int steps = 0;

void rec(int P) {
  steps++;
  if (P == 1) {
    int len = curr.size() + (41 - curr_sum);
    if (len < best_len) {
      best_len = len;
      best_ans = curr;
    }
    return;
  }
  for (int d : divisors) {
    if (P % d == 0 && curr_sum + d <= 41) {
      curr.push_back(d);
      curr_sum += d;
      rec(P / d);
      curr_sum -= d;
      curr.pop_back();
    }
  }
}

void solve() {
  int P;
  cin >> P;
  if (P == 1) {
    best_ans.assign(41, 1);
  } else {
    divisors.clear();
    // Precompute all divisors.
    for (int i = 2; i <= 41; i++) {
      if (P % i == 0) {
        divisors.push_back(i);
      }
    }
    curr.clear();
    steps = 0;
    curr_sum = 0;
    best_ans.clear();
    best_len = (int)1e9;
    rec(P);
    if (steps > maxsteps) {
      maxsteps = steps;
      cerr << P << endl;
    }
  }
  if (!best_ans.empty()) {
    int best_sum = accumulate(best_ans.begin(), best_ans.end(), 0);
    for (; best_sum < 41; best_sum++) {
      best_ans.push_back(1);
    }
    cout << " " << best_ans.size();
    for (int f : best_ans) {
      cout << " " << f;
    }
  } else {
    cout << " -1";
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
  cerr << maxsteps;
  return 0;
}