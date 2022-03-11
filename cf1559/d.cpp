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

ll dsuFind(vector<ll> &par, ll x) {
	if (par[x] < 0) return x;
	return par[x] = dsuFind(par, par[x]);
}

void dsuUnion(vector<ll> &par, ll x, ll y) {
	ll i = dsuFind(par, x), j = dsuFind(par, y);

	if (i == j) return;
	par[i] += par[j], par[j] = i;
}

int main(int argc, char const *argv[]) {
	// Redirect I/O to/from files if running locally.
#ifndef ONLINE_JUDGE
	std::freopen("d-in.txt", "r", stdin);
	std::freopen("d-out.txt", "w", stdout);
#endif

	// Untie C I/O from C++ I/O.
	std::ios_base::sync_with_stdio(false);

	// Untie std::cin. Remember to flush std::cout manually on interactive
	// problems!
	std::cin.tie(nullptr);

	srand(time(nullptr));

	struct LLH {
		zu operator()(ll const &k) const {
			static auto r = rand(), r2 = rand(), r3 = rand(), r4 = rand();
			return (k * r * r2 * r3 + r4) % 1000000009;
		}	 // ? ;)
	};

	ll N, M1, M2;
	cin >> N >> M1 >> M2;

	vector<ll> p1(N, -1), p2(N, -1);
	for (ll i = 0; i < M1; i++) {
		ll x, y;
		cin >> x >> y;
		dsuUnion(p1, x - 1, y - 1);
	}
	for (ll i = 0; i < M2; i++) {
		ll x, y;
		cin >> x >> y;
		dsuUnion(p2, x - 1, y - 1);
	}

	ll cc1 = N - M1, cc2 = N - M2, ans = min(cc1, cc2) - 1;
	cout << ans << '\n';

	unordered_map<ll, unordered_set<ll, LLH>, LLH> scc1, scc2;
	for (ll i = 0; i < N; i++) {
		ll j = dsuFind(p1, i);
		if (scc1.find(j) == scc1.end()) {
			scc1[j] = unordered_set<ll, LLH>();
		}
		scc1[j].insert(i);
		j = dsuFind(p2, i);
		if (scc2.find(j) == scc2.end()) {
			scc2[j] = unordered_set<ll, LLH>();
		}
		scc2[j].insert(i);
	}

	while (scc1.size() > 1 && scc2.size() > 1) {
		auto itP = scc1.end();
		bool done = false;
		do {
			auto otherP = itP;
			otherP++;
			do {
				auto it = itP->second.begin();
				do {
					auto other = otherP->second.begin();
					do {
						auto i = dsuFind(p2, *it), j = dsuFind(p2, *other);
						if (i != j) {
							dsuUnion(p1, *it, *other);
							itP->second.merge(otherP->second);
							scc1.erase(otherP->first);
							dsuUnion(p2, *it, *other);
							auto &ii = scc2[i], &jj = scc2[j];
							ii.merge(jj);
							scc2.erase(j);
							cout << *it + 1 << ' ' << *other + 1 << '\n';
							done = true;
							break;
						}
					} while (++other != otherP->second.end());

					if (done) {
						break;
					}
				} while (++it != itP->second.end());

				if (done) {
					break;
				}
			} while (++otherP != scc1.end());

			if (done) {
				break;
			}
		} while (++itP != scc1.end());
	}

	return 0;
}
