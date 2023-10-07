#include <iostream>
#include <vector>
using namespace std;

int solve() {
  int N, Q;
  cin >> N >> Q;
  vector<bool> white(N + 1);
  for (int i = 1; i <= N; i++) {
    char c;
    cin >> c;
    white[i] = c - '0';
  }
  vector<bool> press(N + 1);
  for (int i = 0, b; i < Q; i++) {
    cin >> b;
    press[b] = press[b] ^ 1;
  }
  // Apply presses.
  for (int i = 1; i <= N; i++) {
    if (press[i]) {
      for (int j = i; j <= N; j += i) {
        white[j] = white[j] ^ 1;
      }
    }
  }
  // Convert to black greedily.
  int num_presses = 0;
  for (int i = 1; i <= N; i++) {
    if (white[i]) {
      num_presses++;
      for (int j = i; j <= N; j += i) {
        white[j] = white[j] ^ 1;
      }
    }
  }
  return num_presses;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    cout << "Case #" << t << ": " << solve() << endl;
  }
  return 0;
}
