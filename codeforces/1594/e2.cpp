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

	ll K, N;
	cin >> K >> N;

	map<string, ll> S2i{
		{"white", 0},
		{"yellow", 1},
		{"green", 2},
		{"blue", 3},
		{"red", 4},
		{"orange", 5}};
	map<ll, ll> fix;
	for (ll i = 0; i < N; i++) {
		ll X;
		string S;
		cin >> X >> S;
		fix[X] = S2i[S];
	}

	ll const MOD = 1000000007;
	vector<ll> defs(K);
	defs[0] = 1;
	for (ll i = 1; i < K; i++) {
		defs[i] = (((defs[i - 1] * defs[i - 1]) % MOD) * 16) % MOD;
	}

	struct CS {
		ll c[6] = {0};
	};

	map<ll, CS> spec;
	ll bd = (1LL << (K - 1)) - 1, pbd = bd * 2 + 1;
	auto fit = fix.rbegin();
	while (fit != fix.rend() && fit->first > bd) {
		spec[fit->first].c[fit->second] = 1;
		fit++;
	}
	for (ll i = K - 2; i >= 0; i--) {
		map<ll, CS> spec2;
		pbd = bd;
		bd = (1LL << i) - 1;

		for (auto &sit : spec) {
			if (spec2.find(sit.first / 2) != spec2.end()) {
				continue;
			}
			CS par, ch1, ch2;
			auto it = spec.find(sit.first / 2 * 2);
			if (it != spec.end()) {
				ch1 = it->second;
			} else {
				ch1 = {
					defs[K - i - 2],
					defs[K - i - 2],
					defs[K - i - 2],
					defs[K - i - 2],
					defs[K - i - 2],
					defs[K - i - 2],
				};
			}
			it = spec.find(sit.first / 2 * 2 + 1);
			if (it != spec.end()) {
				ch2 = it->second;
			} else {
				ch2 = {
					defs[K - i - 2],
					defs[K - i - 2],
					defs[K - i - 2],
					defs[K - i - 2],
					defs[K - i - 2],
					defs[K - i - 2]};
			}
			auto it2 = fix.find(sit.first / 2);
			if (it2 != fix.end()) {
				par = {0, 0, 0, 0, 0, 0};
				par.c[it2->second] = 1;
			} else {
				par = {1, 1, 1, 1, 1, 1};
			}

			spec2[sit.first / 2] = {
				par.c[0] * (ch1.c[2] + ch1.c[3] + ch1.c[4] + ch1.c[5]) % MOD *
					(ch2.c[2] + ch2.c[3] + ch2.c[4] + ch2.c[5]),
				par.c[1] * (ch1.c[2] + ch1.c[3] + ch1.c[4] + ch1.c[5]) % MOD *
					(ch2.c[2] + ch2.c[3] + ch2.c[4] + ch2.c[5]),
				par.c[2] * (ch1.c[0] + ch1.c[1] + ch1.c[4] + ch1.c[5]) % MOD *
					(ch2.c[0] + ch2.c[1] + ch2.c[4] + ch2.c[5]),
				par.c[3] * (ch1.c[0] + ch1.c[1] + ch1.c[4] + ch1.c[5]) % MOD *
					(ch2.c[0] + ch2.c[1] + ch2.c[4] + ch2.c[5]),
				par.c[4] * (ch1.c[2] + ch1.c[3] + ch1.c[0] + ch1.c[1]) % MOD *
					(ch2.c[2] + ch2.c[3] + ch2.c[0] + ch2.c[1]),
				par.c[5] * (ch1.c[2] + ch1.c[3] + ch1.c[0] + ch1.c[1]) % MOD *
					(ch2.c[2] + ch2.c[3] + ch2.c[0] + ch2.c[1])};
		}

		while (fit != fix.rend() && fit->first > bd) {
			if (spec2.find(fit->first) != spec2.end()) {
				fit++;
				continue;
			}
			CS par;
			par = {0, 0, 0, 0, 0, 0};
			par.c[fit->second] = 1;

			spec2[fit->first] = {
				par.c[0] * 16 * defs[K - i - 2],
				par.c[1] * 16 * defs[K - i - 2],
				par.c[2] * 16 * defs[K - i - 2],
				par.c[3] * 16 * defs[K - i - 2],
				par.c[4] * 16 * defs[K - i - 2],
				par.c[5] * 16 * defs[K - i - 2]};

			fit++;
		}

		for (auto &it : spec2) {
			it.second.c[0] %= MOD;
			it.second.c[1] %= MOD;
			it.second.c[2] %= MOD;
			it.second.c[3] %= MOD;
			it.second.c[4] %= MOD;
			it.second.c[5] %= MOD;
		}
		swap(spec, spec2);
	}
	cout << (spec.begin()->second.c[0] + spec.begin()->second.c[1] +
					 spec.begin()->second.c[2] + spec.begin()->second.c[3] +
					 spec.begin()->second.c[4] + spec.begin()->second.c[5]) %
			MOD;

	return 0;
}
