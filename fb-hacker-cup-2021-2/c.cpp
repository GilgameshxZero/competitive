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
	std::freopen("c-in.txt", "r", stdin);
	std::freopen("c-out.txt", "w", stdout);
#endif

	// Untie C I/O from C++ I/O.
	std::ios_base::sync_with_stdio(false);

	// Untie std::cin. Remember to flush std::cout manually on interactive
	// problems!
	std::cin.tie(nullptr);

	ll T;
	cin >> T;
	for (ll t = 0; t < T; t++) {
		ll R, C, K;
		cin >> R >> C >> K;

		vector<set<ll>> cp(C);
		for (ll r = 0; r < R; r++) {
			string s;
			cin >> s;
			for (ll c = 0; c < C; c++) {
				if (s[c] == 'X') {
					cp[c].insert(r);
				}
			}
		}

		auto tcp = cp;

		vector<ll> stt(C);
		for (ll c = 0; c < C; c++) {
			while (cp[c].find(stt[c]) != cp[c].end()) {
				cp[c].erase(stt[c]);
				stt[c]++;
			}
		}

		ll ans = C;
		for (ll sh = 0; sh < min(ans, R); sh++) {
			ll tans = 0;
			for (ll c = 0; c < C; c++) {
				if (stt[c] >= K || cp[c].find(K - 1 + sh) != cp[c].end()) {
					tans++;
				}
			}
			for (ll c = 0; c < C; c++) {
				while (cp[c].find(sh + stt[c]) != cp[c].end()) {
					cp[c].erase(sh + stt[c]);
					stt[c]++;
				}
			}
			ans = min(ans, tans + sh);
		}

		stt.clear();
		stt.resize(C);
		for (ll c = 0; c < C; c++) {
			while (tcp[c].find(R - 1 - stt[c]) != tcp[c].end()) {
				tcp[c].erase(R - 1 - stt[c]);
				stt[c]++;
			}
		}

		for (ll sh = 0; sh < min(ans, R); sh++) {
			ll tans = 0;
			for (ll c = 0; c < C; c++) {
				if (R - stt[c] < K || tcp[c].find(K - 1 - sh) != tcp[c].end()) {
					tans++;
				}
			}
			for (ll c = 0; c < C; c++) {
				while (tcp[c].find(R - 1 - sh - stt[c]) != tcp[c].end()) {
					tcp[c].erase(R - 1 - sh - stt[c]);
					stt[c]++;
				}
			}
			ans = min(ans, tans + sh);
		}

		cout << "Case #" << t + 1 << ": " << ans << '\n';
	}

	return 0;
}
