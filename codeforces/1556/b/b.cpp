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
	std::freopen("b-in.txt", "r", stdin);
	std::freopen("b-out.txt", "w", stdout);
#endif

	// Untie C I/O from C++ I/O.
	std::ios_base::sync_with_stdio(false);

	// Untie std::cin. Remember to flush std::cout manually on interactive
	// problems!
	std::cin.tie(nullptr);

	ll T;
	cin >> T;

	vector<ll> xs;
	while (T--) {
		ll N;
		cin >> N;

		xs.resize(N);
		ll c[2] = {};
		set<ll> cp[2];
		for (ll i = 0; i < N; i++) {
			cin >> xs[i];
			xs[i] %= 2;
			c[xs[i]]++;
			cp[xs[i]].insert(i);
		}

		auto calc = [&](int fp) {
			ll ans = 0;
			auto it = cp[fp].begin();
			for (ll i = 0; i < N; i += 2) {
				ans += abs(*it++ - i);
			}
			return ans;
		};

		if (c[0] < c[1] - 1 || c[0] > c[1] + 1) {
			cout << "-1\n";
		} else if (c[0] == c[1] - 1) {
			cout << calc(1) << '\n';
		} else if (c[0] == c[1] + 1) {
			cout << calc(0) << '\n';
		} else {
			cout << min(calc(0), calc(1)) << '\n';
		}
	}

	return 0;
}
