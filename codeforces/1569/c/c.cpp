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

ll expm(ll b, ll e) {
	if (e == 0) {
		return 1;
	} else if (e % 2 == 1) {
		return (expm(b, e - 1) * b) % 998244353;
	} else {
		ll res = expm(b, e / 2);
		return (res * res) % 998244353;
	}
}

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

	vector<ll> pm{1};
	for (ll i = 1; i < 200001; i++) {
		pm.push_back((pm.back() * i) % 998244353);
	}

	ll T;
	cin >> T;
	map<ll, ll> c;
	while (T--) {
		ll N;
		cin >> N;
		ll n = N;
		c.clear();
		while (n--) {
			ll a;
			cin >> a;
			if (c.find(a) == c.end()) {
				c[a] = 0;
			}
			c[a]++;
		}

		ll h1 = c.rbegin()->first, h2, c2;
		if (c.rbegin()->second == 1) {
			h2 = (++c.rbegin())->first;
			c2 = (++c.rbegin())->second;
		} else {
			h2 = c.rbegin()->first;
			c2 = c.rbegin()->second;
		}
		if (h1 >= h2 + 2) {
			cout << "0\n";
		} else if (h1 == h2) {
			cout << pm[N] << '\n';
		} else {
			cout << (((pm[N] * c2) % 998244353) * expm(c2 + 1, 998244351)) % 998244353
					 << '\n';
		}
	}

	return 0;
}
