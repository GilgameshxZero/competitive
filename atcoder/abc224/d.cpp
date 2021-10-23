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

	ll M;
	cin >> M;
	vector<set<ll>> E(9);
	for (ll i = 0; i < M; i++) {
		ll X, Y;
		cin >> X >> Y;
		E[X - 1].insert(Y - 1);
		E[Y - 1].insert(X - 1);
	}

	string P(9, 8);
	for (ll i = 0; i < 8; i++) {
		ll j;
		cin >> j;
		P[j - 1] = i;
	}

	string end(9, 0);
	for (ll i = 0; i < 9; i++) {
		end[i] = i;
	}

	queue<pair<string, ll>> q;
	set<string> visited;
	q.push({P, 0});
	while (!q.empty()) {
		auto front = q.front();
		q.pop();
		if (visited.find(front.first) != visited.end()) {
			continue;
		}
		visited.insert(front.first);
		if (front.first == end) {
			cout << front.second;
			return 0;
		}

		ll where;
		for (where = 0; where < 9; where++) {
			if (front.first[where] == 8) {
				break;
			}
		}

		for (auto &e : E[where]) {
			string s = front.first;
			swap(s[where], s[e]);
			q.push({s, front.second + 1});
		}
	}

	cout << "-1";
	return 0;
}
