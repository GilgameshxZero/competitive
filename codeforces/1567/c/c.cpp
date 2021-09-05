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

pair<ll, bool> calc(string &s, ll cur) {
	if (cur == s.length() - 2) {
		bool canZero = s[cur] <= '9' && s[cur + 1] <= '9';
		ll opt1 = s[cur] - '0', opt2 = s[cur + 1] - '0';
		if (opt1 < 10) {
			opt1++;
		} else {
			opt1 = 19 - opt1;
		}
		if (opt2 < 10) {
			opt2++;
		} else {
			opt2 = 19 - opt2;
		}
		return {opt1 * opt2, canZero};
	}

	auto res1 = calc(s, cur + 1);
	ll opt = s[cur] - '0';
	if (opt < 10) {
		opt++;
	} else {
		opt = 19 - opt;
	}
	res1.first *= opt;

	pair<ll, bool> res2;
	if (s[cur] == '0') {
		res2 = {0, false};
	} else {
		string sc = s;
		sc[cur]--;
		sc[cur + 2] += 10;
		res2 = calc(sc, cur + 1);

		ll opt = sc[cur] - '0';
		if (opt < 10) {
			opt++;
		} else {
			opt = 19 - opt;
		}
		res2.first *= opt;
	}

	return {res1.first + res2.first, res1.second || res2.second};
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

	ll T;
	cin >> T;
	while (T--) {
		string s;
		cin >> s;
		if (s.length() == 1) {
			cout << s[0] - '0' - 1 << '\n';
			continue;
		}
		auto res = calc(s, 0);
		if (res.second) {
			res.first -= 2;
		}
		cout << res.first << '\n';
	}

	return 0;
}
