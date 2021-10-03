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

void dfs(vector<ll> &trav, vector<set<ll>> &chs, ll cur, ll par) {
	trav.push_back(cur);
	for (auto &ch : chs[cur]) {
		if (ch == par) {
			continue;
		}
		dfs(trav, chs, ch, cur);
	}
}

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

	ll T;
	cin >> T;
	while (T--) {
		ll N, K;
		cin >> N >> K;

		vector<set<ll>> chs(N);
		vector<ll> A(N);
		for (auto &a : A) {
			cin >> a;
		}
		for (ll i = 0; i < N - 1; i++) {
			ll x, y;
			cin >> x >> y;
			chs[x - 1].insert(y - 1);
			chs[y - 1].insert(x - 1);
		}

		vector<ll> trav;
		dfs(trav, chs, 0, -1);

		ll cand = 0;
		for (auto &t : trav) {
			cand ^= A[t];
		}

		if (cand == 0) {
			cout << "YES\n";
			continue;
		}

		ll k = 0, test = 0;
		for (ll i = 0; i < trav.size(); i++) {
			auto t = trav[i];
			test ^= A[t];
			if (i != trav.size() - 1 && test == cand) {
				k++;
				test = 0;
			}
		}
		cout << (test == cand && k <= K - 1 && k > 0 ? "YES" : "NO") << '\n';
	}

	return 0;
}
