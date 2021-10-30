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

// Shorthand for common types.
using zu = std::size_t;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
template <typename First, typename Second>
using pr = std::pair<First, Second>;
template <typename Type>
using vr = std::vector<Type>;

#define rf(X, F, T) for (ll X = F; X != T; X += (F < T) * 2 - 1)

ll gcd(ll X, ll Y) {
	if (X == 0) {
		return Y;
	}
	return gcd(Y % X, X);
}

ll lcm(ll X, ll Y) {
	return X * Y / gcd(X, Y);
}

int main(int argc, char const *argv[]) {
	// Redirect I/O to/from files if running locally.
#ifndef ONLINE_JUDGE
	std::freopen("in.txt", "r", stdin);
	std::freopen("out.txt", "w", stdout);
#endif

	// Untie C I/O from C++ I/O. Do not intersperse printf/scanf with cin/cout.
	std::ios_base::sync_with_stdio(false);

	// Untie std::cin. Remember to flush std::cout manually on interactive
	// problems!
	std::cin.tie(nullptr);

	vr<ll> lcms;
	lcms.push_back(1);
	lcms.push_back(1);
	rf(i, 2, 500000) {
		if (lcms.back() >= 1000000000) {
			break;
		}
		lcms.push_back(lcm(lcms.back(), i));
	}

	ll T;
	cin >> T;
	while (T--) {
		ll N;
		cin >> N;

		bool done = false;
		rf(i, 0, N) {
			ll A;
			cin >> A;

			if (!done && lcms.size() > i + 2) {
				if (A % lcms[i + 2] == 0) {
					cout << "NO\n";
					done = true;
				}
			}
		}

		if (!done) {
			cout << "YES\n";
		}
	}

	return 0;
}
