#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

using int64 = long long;
const int64 MOD = (int)1e9 + 7;

struct Node {
  int64 maxval, maxflip;
  int idx_of_maxval, idx_of_maxflip;

  Node() {}
  Node(int i, int64 v):
    maxval(v), idx_of_maxval(i),
    maxflip(((long long)v * (MOD - 1)) % MOD), idx_of_maxflip(i) {}
};

template<class T, class D>
class segment_tree {
  static T join_values(const T &a, const T &b) {
    Node res;
    if (a.maxval > b.maxval ||
        (a.maxval == b.maxval && a.idx_of_maxval < b.idx_of_maxval)) {
      res.maxval = a.maxval;
      res.idx_of_maxval = a.idx_of_maxval;
    } else {
      res.maxval = b.maxval;
      res.idx_of_maxval = b.idx_of_maxval;
    }
    if (a.maxflip > b.maxflip ||
        (a.maxflip == b.maxflip && a.idx_of_maxflip < b.idx_of_maxflip)) {
      res.maxflip = a.maxflip;
      res.idx_of_maxflip = a.idx_of_maxflip;
    } else {
      res.maxflip = b.maxflip;
      res.idx_of_maxflip = b.idx_of_maxflip;
    }
    return res;
  }

  static void join_value_with_delta(T &v, const D &d, int len) {
    if (d) {  // Flip all values in v.lo .. v.hi.
      swap(v.maxval, v.maxflip);
      swap(v.idx_of_maxval, v.idx_of_maxflip);
    }
  }

  static D join_deltas(const D &d1, const D &d2) {
    return d1 ^ d2;
  }

  int len;
  std::vector<T> value;
  std::vector<D> delta;
  std::vector<bool> pending;

  template<class It>
  void build(int i, int lo, int hi, It arr) {
    if (lo == hi) {
      value[i] = Node(lo, *(arr + lo));
      return;
    }
    int mid = lo + (hi - lo)/2;
    build(i*2 + 1, lo, mid, arr);
    build(i*2 + 2, mid + 1, hi, arr);
    value[i] = join_values(value[i*2 + 1], value[i*2 + 2]);
  }

  void push_delta(int i, int lo, int hi) {
    if (pending[i]) {
      join_value_with_delta(value[i], delta[i], hi - lo + 1);
      if (lo != hi) {
        int l = 2*i + 1, r = 2*i + 2;
        delta[l] = pending[l] ? join_deltas(delta[l], delta[i]) : delta[i];
        delta[r] = pending[r] ? join_deltas(delta[r], delta[i]) : delta[i];
        pending[l] = pending[r] = true;
      }
      pending[i] = false;
    }
  }

  T query(int i, int lo, int hi, int tgt_lo, int tgt_hi) {
    push_delta(i, lo, hi);
    if (lo == tgt_lo && hi == tgt_hi) {
      return value[i];
    }
    int mid = lo + (hi - lo)/2;
    if (tgt_lo <= mid && mid < tgt_hi) {
      return join_values(
                query(i*2 + 1, lo, mid, tgt_lo, std::max(tgt_hi, mid)),
                query(i*2 + 2, mid + 1, hi, std::max(tgt_lo, mid + 1), tgt_hi));
    }
    if (tgt_lo <= mid) {
      return query(i*2 + 1, lo, mid, tgt_lo, std::min(tgt_hi, mid));
    }
    return query(i*2 + 2, mid + 1, hi, std::max(tgt_lo, mid + 1), tgt_hi);
  }

  void update(int i, int lo, int hi, int tgt_lo, int tgt_hi, const D &d) {
    push_delta(i, lo, hi);
    if (hi < tgt_lo || lo > tgt_hi) {
      return;
    }
    if (tgt_lo <= lo && hi <= tgt_hi) {
      delta[i] = d;
      pending[i] = true;
      push_delta(i, lo, hi);
      return;
    }
    update(2*i + 1, lo, (lo + hi)/2, tgt_lo, tgt_hi, d);
    update(2*i + 2, (lo + hi)/2 + 1, hi, tgt_lo, tgt_hi, d);
    value[i] = join_values(value[2*i + 1], value[2*i + 2]);
  }

 public:
  template<class It>
  segment_tree(It lo, It hi)
      : len(hi - lo), value(4*len), delta(4*len), pending(4*len, false) {
    build(0, 0, len - 1, lo);
  }

  int size() const { return len; }
  T query(int lo, int hi) { return query(0, 0, len - 1, lo, hi); }
  void update(int i, const D &d) { update(0, 0, len - 1, i, i, d); }
  void update(int lo, int hi, const D &d) { update(0, 0, len - 1, lo, hi, d); }
};

long long solve() {
  int N, Q;
  cin >> N;
  vector<int64> A(N);
  for (int i = 0; i < N; i++) {
    cin >> A[i];
  }
  segment_tree<Node, bool> S(A.begin(), A.end());
  long long ans = 0;
  cin >> Q;
  for (int i = 0, l, r; i < Q; i++) {
    cin >> l >> r;
    l--;
    r--;
    S.update(l, r, 1);
    Node res = S.query(0, N - 1);
    // assert(res.idx_of_maxval >= 0 && res.idx_of_maxval < N);
    // cerr << ">> " << res.idx_of_maxval+1 << " " << res.maxval << endl;
    ans += res.idx_of_maxval+1;
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