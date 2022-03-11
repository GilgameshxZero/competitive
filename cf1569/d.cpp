// C++ template for coding competitions designed for C++11 support, primarily
// targeting MSVC++17.

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

int main(int argc, char const *argv[]) {
	// Redirect I/O to/from files if running locally.
#ifndef ONLINE_JUDGE
	std::freopen("d-in.txt", "r", stdin);
	std::freopen("d-out.txt", "w", stdout);
#endif

	// Untie C I/O from C++ I/O.
	std::ios_base::sync_with_stdio(false);

	// Untie std::cin. Remember to flush std::cout manually on interactive
	// problems!
	std::cin.tie(nullptr);

	ll T;
	cin >> T;

	set<ll> hs, vs;
	vector<pair<ll, ll>> ps;
	while (T--) {
		ll N, M, K;
		cin >> N >> M >> K;
		vs.clear();
		hs.clear();
		for (ll n = 0; n < N; n++) {
			ll v;
			cin >> v;
			vs.insert(v);
		}
		for (ll m = 0; m < M; m++) {
			ll h;
			cin >> h;
			hs.insert(h);
		}
		ps.resize(K);
		for (auto &p : ps) {
			cin >> p.first >> p.second;
		}

		ll ans = 0;
		sort(ps.begin(), ps.end());
		ll cno = 0;
		map<ll, ll> mno;
		auto j = vs.begin();
		for (ll i = 0; i < ps.size(); i++) {
			while (j != vs.end() && *j <= ps[i].first) {
				cno = 0;
				mno.clear();
				j++;
			}
			if (vs.find(ps[i].first) == vs.end()) {
				auto k = mno.find(ps[i].second);
				if (k == mno.end()) {
					mno[ps[i].second] = 0;
					k = mno.find(ps[i].second);
				}
				ans += cno - k->second;
				cno++;
				k->second++;
			}
		}

		sort(ps.begin(), ps.end(), [](pair<ll, ll> &x, pair<ll, ll> &y) {
			return x.second < y.second;
		});
		cno = 0;
		mno.clear();
		j = hs.begin();
		for (ll i = 0; i < ps.size(); i++) {
			while (j != hs.end() && *j <= ps[i].second) {
				cno = 0;
				mno.clear();
				j++;
			}
			if (hs.find(ps[i].second) == hs.end()) {
				auto k = mno.find(ps[i].first);
				if (k == mno.end()) {
					mno[ps[i].first] = 0;
					k = mno.find(ps[i].first);
				}
				ans += cno - k->second;
				cno++;
				k->second++;
			}
		}

		cout << ans << '\n';
	}

	return 0;
}
