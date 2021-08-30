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
	for (ll t = 0; t < T; t++) {
		ll N;
		cin >> N;
		vector<string> board(N);
		for (ll i = 0; i < N; i++) {
			cin >> board[i];
		}

		ll ans = LLONG_MAX;
		auto calc = [&](bool row, ll idx) -> pair<ll, set<pair<ll, ll>>> {
			ll cx = 0, co = 0;
			set<pair<ll, ll>> s;
			for (ll i = 0; i < N; i++) {
				auto &sq = board[row ? idx : i][row ? i : idx];
				cx += sq == 'X';
				co += sq == 'O';
				if (sq == '.') {
					s.insert({row ? idx : i, row ? i : idx});
				}
			}
			return {co == 0 ? N - cx : LLONG_MAX, s};
		};

		for (ll i = 0; i < N; i++) {
			ans = min(ans, min(calc(false, i).first, calc(true, i).first));
		}

		set<set<pair<ll, ll>>> ss;
		for (ll i = 0; i < N; i++) {
			auto res = calc(false, i);
			if (res.first == ans) {
				ss.insert(res.second);
			}
			res = calc(true, i);
			if (res.first == ans) {
				ss.insert(res.second);
			}
		}
		cout << "Case #" << t + 1 << ": "
				 << (ans == LLONG_MAX ? "Impossible"s
															: to_string(ans) + " " + to_string(ss.size()))
				 << '\n';
	}

	return 0;
}
