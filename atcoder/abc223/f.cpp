// C++ template for coding competitions designed for C++11 support.

// Disable security/deprecation warnings on MSVC++.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <algorithm>
#include <atomic>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cinttypes>
#include <cmath>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <system_error>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// Automatic-duration execution-time logger conditionally defined at execution
// start.
class WallTimeGuard {
	private:
	std::chrono::steady_clock::time_point executionBegin;

	public:
	WallTimeGuard() : executionBegin(std::chrono::steady_clock::now()) {}
	~WallTimeGuard() {
		std::cout << "\n\n-------- Wall time: "
							<< std::chrono::duration_cast<std::chrono::milliseconds>(
									 std::chrono::steady_clock::now() - this->executionBegin)
									 .count()
							<< "ms. --------" << std::endl;
	}
};

// If running locally, log execution time.
#ifndef ONLINE_JUDGE
WallTimeGuard wallTimeGuard;
#endif

// User-defined literals.
constexpr std::size_t operator"" _zu(unsigned long long value) {
	return static_cast<std::size_t>(value);
}
std::regex operator"" _re(char const *value, std::size_t) {
	return std::regex(value);
}

// Imports std scope into global scope; care for name conflicts. Also imports
// literals in std::literals.
using namespace std;

using zu = std::size_t;
using ll = long long;
using ull = unsigned long long;

pair<ll, ll> query(
	vector<pair<ll, ll>> &seg,
	vector<pair<ll, ll>> &ran,
	ll l,
	ll r,
	ll cur) {
	if (ran[cur].first >= l && ran[cur].second <= r) {
		return seg[cur];
	}
	if (ran[cur].first > r || ran[cur].second < l) {
		return {0, 0};
	}
	auto res1 = query(seg, ran, l, r, cur * 2),
			 res2 = query(seg, ran, l, r, cur * 2 + 1);
	return {
		res1.first + max(0LL, res2.first - res1.second),
		res2.second + max(0LL, res1.second - res2.first)};
}

int main(int argc, char const *argv[]) {
	// Redirect I/O to/from files if running locally.
#ifndef ONLINE_JUDGE
	std::freopen("in.txt", "r", stdin);
	std::freopen("out.txt", "w", stdout);
#endif

	// Untie C I/O from C++ I/O.
	std::ios_base::sync_with_stdio(false);

	// Untie std::cin. Remember to flush std::cout manually on interactive
	// problems!
	std::cin.tie(nullptr);

	ll N, Q;
	string S;
	cin >> N >> Q >> S;

	vector<pair<ll, ll>> seg(1 << 19, {0, 0}), ran(1 << 19);
	for (ll i = 0; i < N; i++) {
		seg[(1 << 18) + i].first = S[i] == ')';
		seg[(1 << 18) + i].second = S[i] == '(';
	}
	for (ll i = (1 << 18); i < ran.size(); i++) {
		ran[i] = {i - (1 << 18), i - (1 << 18)};
	}
	for (ll i = (1 << 18) - 1; i > 0; i--) {
		seg[i].first =
			seg[i * 2].first + max(0LL, seg[i * 2 + 1].first - seg[i * 2].second);
		seg[i].second = seg[i * 2 + 1].second +
			max(0LL, seg[i * 2].second - seg[i * 2 + 1].first);
		ran[i] = {ran[i * 2].first, ran[i * 2 + 1].second};
	}
	for (ll i = 0; i < Q; i++) {
		ll T, L, R;
		cin >> T >> L >> R;
		if (T == 1) {
			ll cur = (1 << 18) + L - 1;
			seg[cur].first = S[R - 1] == ')';
			seg[cur].second = S[R - 1] == '(';
			cur /= 2;
			while (cur > 0) {
				seg[cur].first = seg[cur * 2].first +
					max(0LL, seg[cur * 2 + 1].first - seg[cur * 2].second);
				seg[cur].second = seg[cur * 2 + 1].second +
					max(0LL, seg[cur * 2].second - seg[cur * 2 + 1].first);
				cur /= 2;
			}

			cur = (1 << 18) + R - 1;
			seg[cur].first = S[L - 1] == ')';
			seg[cur].second = S[L - 1] == '(';
			cur /= 2;
			while (cur > 0) {
				seg[cur].first = seg[cur * 2].first +
					max(0LL, seg[cur * 2 + 1].first - seg[cur * 2].second);
				seg[cur].second = seg[cur * 2 + 1].second +
					max(0LL, seg[cur * 2].second - seg[cur * 2 + 1].first);
				cur /= 2;
			}

			swap(S[L - 1], S[R - 1]);
		} else {
			pair<ll, ll> target = {0, 0};
			cout << (query(seg, ran, L - 1, R - 1, 1) == target ? "Yes" : "No")
					 << '\n';
		}
	}

	return 0;
}
