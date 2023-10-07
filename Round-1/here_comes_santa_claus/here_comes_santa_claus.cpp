#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

double solve() {
  int N;
  cin >> N;
  vector<int> X(N);
  for (int i = 0; i < N; i++) {
    cin >> X[i];
  }
  sort(X.begin(), X.end());
  double ans = X[N-1] + X[N-2] - (X[0] + X[1]);
  if (N == 5) {
    ans = max(
      X[N-1] + X[N-3] - (X[0] + X[1]),
      X[N-1] + X[N-2] - (X[0] + X[2])
    );
  }
 return ans / 2;
}

int main() {
  int T;
  cin >> T;
  cout.precision(1);
  for (int t = 1; t <= T; t++) {
    cout << "Case #" << t << ": " << fixed << solve() << endl;
  }
  return 0;
}
