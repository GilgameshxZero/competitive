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
#include <climits>
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
using ld = long double;

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

	ll N;
	cin >> N;

	vector<pair<ll, ll>> crit, sv;
	for (ll i = 0; i < N; i++) {
		ll X, Y;
		cin >> X >> Y;
		crit.push_back({X - 1, Y});
		crit.push_back({X, Y - 1});
		sv.push_back({X, Y});
	}
	sort(
		crit.begin(), crit.end(), [](pair<ll, ll> const &L, pair<ll, ll> const &R) {
			static ld const EPS = 1e-12;
			return (L.second + EPS) / (L.first + EPS) <
				(R.second + EPS) / (R.first + EPS);
		});

	map<pair<ll, ll>, ll> cm;
	ll cmx;
	cm[crit[0]] = cmx = 0;
	for (ll i = 1; i < crit.size(); i++) {
		cm[crit[i]] = cmx = cmx +
			(crit[i].first * crit[i - 1].second !=
			 crit[i].second * crit[i - 1].first);
	}

	vector<pair<ll, ll>> sb;
	for (ll i = 0; i < N; i++) {
		sb.push_back(
			{cm[{sv[i].first - 1, sv[i].second}],
			 cm[{sv[i].first, sv[i].second - 1}]});
	}
	sort(sb.begin(), sb.end());

	ll top = 0, ans = 0;
	for (ll i = 0; i < sb.size(); i++) {
		if (sb[i].second >= top) {
			top = sb[i].first;
			ans++;
		}
	}
	cout << ans;

	return 0;
}
