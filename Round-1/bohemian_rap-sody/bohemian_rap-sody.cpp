#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <tuple>
#include <vector>
using namespace std;

using int64 = long long;

int64 N, Q, sqrtsz;
vector<int64> freq, ff;

void inc(int64 x) {
  freq[x]++;
  ff[freq[x]]++;
}

void dec(int64 x) {
  ff[freq[x]]--;
  freq[x]--;
}

int64 get_ans() {
  int64 ans = ff[1];
  for (int i = 2; i <= ans; i++) {
    int64 need = ans - (i - 1);
    if (ff[i] < need) {
      ans -= need - ff[i];
    }
  }
  return ans;
}

int64 solve() {
  cin >> N;

  vector<string> vs(N);
  for (int i = 0; i < N; i++) {
    cin >> vs[i];
  }
  int64 sumc = 0;
  int maxt = 0;
  for (int i = 0; i < N; i++) {
    reverse(vs[i].begin(), vs[i].end());
    sumc += vs[i].size();
    maxt = max(maxt, (int)vs[i].size());
  }

  ff = vector<int64>(N + 10);
  freq = vector<int64>(sumc + 10);

  int idt = 0;
  vector<vector<int>> prox(sumc + 10, vector<int>(26));

  for (auto &s : vs) {
    int est = 0;
    for (auto &c : s) {
      int ic = c - 'a';
      if (!prox[est][ic]) {
        idt++;
        prox[est][ic] = idt;
      }
      est = prox[est][ic];
    }
  }

  vector<int64> vest(N);
  set<int> active;
  for (int i = 0; i < N; i++) {
    active.insert(i);
  }
  vector<vector<tuple<int64, int64, int64>>> vq(maxt + 1);

  cin >> Q;
  vector<int64> ans(Q);
  for (int i = 0; i < Q; i++) {
    int64 l, r, k;
    cin >> l >> r >> k;
    l--, r--;
    if (k <= maxt) {
      vq[k].emplace_back(l, r, i);
    }
  }

  // Reset ff only by the number of active! (in reality you only need sqrt).
  // No need to reset freq, because they are all different states!
  for (int k = 1; k <= maxt; k++) {
    for (int i = 0; i <= (int)active.size(); i++) {
      ff[i] = 0;
    }

    for (auto it = active.begin(); it != active.end();) {
      int i = *it;
      if (k > (int)vs[i].size()) {
        it = active.erase(it);
      } else {
        int ic = vs[i][k - 1] - 'a';
        vest[i] = prox[vest[i]][ic];
        it++;
      }
    }
    vector<int> vid(active.begin(), active.end());

    vector<tuple<int64, int64, int64>> vqq;
    vqq.reserve(vq[k].size());
    for (auto &[lc, rc, ia] : vq[k]) {
      lc = lower_bound(vid.begin(), vid.end(), lc) - vid.begin();
      rc = upper_bound(vid.begin(), vid.end(), rc) - vid.begin();
      rc--;
      if (lc <= rc) {
        vqq.emplace_back(lc, rc, ia);
      }
    }
    vq[k] = vqq;
    sqrtsz = sqrt(vid.size()) + 1;
    sort(vq[k].begin(), vq[k].end(),
      [](tuple<int64, int64, int64> &a, tuple<int64, int64, int64> &b) {
        int64 la, ra, ia;
        tie(la, ra, ia) = a;
        int64 lb, rb, ib;
        tie(lb, rb, ib) = b;
        if (la / sqrtsz != lb / sqrtsz) {
          return la < lb;
        }
        return ra < rb;
      });

    int l = 0, r = -1;
    for (auto &[lc, rc, ia] : vq[k]) {
      while (r < rc) {
        r++;
        inc(vest[vid[r]]);
      }
      while (l > lc) {
        l--;
        inc(vest[vid[l]]);
      }
      while (l < lc) {
        dec(vest[vid[l]]);
        l++;
      }
      while (r > rc) {
        dec(vest[vid[r]]);
        r--;
      }
      ans[ia] = get_ans();
    }
  }
  int64 res = 0;
  for (int i = 0; i < Q; i++) {
    res += ans[i];
  }
  return res;
}

int main() {
  int T;
  cin >> T;
  for (int t = 1; t <= T; t++) {
    cout << "Case #" << t << ": " << solve() << endl;
  }
  return 0;
}
