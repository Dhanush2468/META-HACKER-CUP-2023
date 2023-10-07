#include <iostream>
#include <vector>
using namespace std;

long long solve() {
  int N, Q;
  cin >> N >> Q;
  vector<bool> white(N + 1);
  for (int i = 1; i <= N; i++) {
    char c;
    cin >> c;
    white[i] = c - '0';
  }
  vector<bool> press(N + 1);
  int num_presses = 0;
  for (int i = 1; i <= N; i++) {
    if (white[i]) {
      num_presses++;
      press[i] = 1;
      for (int j = i; j <= N; j += i) {
        white[j] = white[j] ^ 1;
      }
    }
  }
  long long ans = 0;
  for (int i = 0, b; i < Q; i++) {
    cin >> b;
    if (press[b]) {
      num_presses--;
    } else {
      num_presses++;
    }
    press[b] = press[b] ^ 1;
    ans += num_presses;
  }
  return ans;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    cout << "Case #" << t << ": " << solve() << endl;
  }
  return 0;
}
