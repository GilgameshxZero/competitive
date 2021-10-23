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

	ll H, W, N;
	cin >> H >> W >> N;
	vector<ll> R(N), C(N), A(N), J(N);
	vector<pair<ll, ll>> sA;
	map<ll, set<pair<ll, ll>>> row, col;
	for (ll i = 0; i < N; i++) {
		cin >> R[i] >> C[i] >> A[i];
		if (row.find(R[i]) == row.end()) {
			row.insert({R[i], {}});
		}
		row[R[i]].insert({-A[i], i});
		if (col.find(C[i]) == col.end()) {
			col.insert({C[i], {}});
		}
		col[C[i]].insert({-A[i], i});

		sA.push_back({A[i], i});
	}
	sort(sA.begin(), sA.end());

	for (ll l = N - 1; l >= 0; l--) {
		ll i = sA[l].second;
		auto j = row[R[i]].lower_bound({-A[i] + 1, 0LL});
		if (j != row[R[i]].end()) {
			auto jt = j->first;
			while (j != row[R[i]].end() && j->first == jt) {
				J[j->second] = max(J[j->second], J[i] + 1);
				j++;
			}
		}
		auto k = col[C[i]].lower_bound({-A[i] + 1, 0LL});
		if (k != col[C[i]].end()) {
			auto kt = k->first;
			while (k != col[C[i]].end() && k->first == kt) {
				J[k->second] = max(J[k->second], J[i] + 1);
				k++;
			}
		}
	}

	for (auto &j : J) {
		cout << j << '\n';
	}

	return 0;
}
