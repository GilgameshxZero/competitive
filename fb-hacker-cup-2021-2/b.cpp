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

		vector<set<ll>> chs(N);
		for (ll i = 0; i < N - 1; i++) {
			ll A, B;
			cin >> A >> B;
			chs[A - 1].insert(B - 1);
			chs[B - 1].insert(A - 1);
		}
		vector<ll> fr(N);
		for (ll i = 0; i < N; i++) {
			cin >> fr[i];
			fr[i]--;
		}
		vector<ll> dep(N), par(N);
		par[0] = -1;
		queue<ll> q;
		q.push(0);
		while (!q.empty()) {
			ll v = q.front();
			q.pop();

			chs[v].erase(par[v]);
			for (auto &ch : chs[v]) {
				dep[ch] = dep[v] + 1;
				par[ch] = v;
				q.push(ch);
			}
		}

		vector<set<ll>> frst(N);
		set<ll> prot;
		vector<ll> frh(N, -1);
		for (ll i = 0; i < N; i++) {
			ll f = fr[i];
			if (frh[f] == -1) {
				frh[f] = i;
				frst[i].insert(f);
				continue;
			}

			ll frhd = dep[frh[f]];
			ll cur = i;
			while (cur != -1 && frhd < dep[cur] &&
						 frst[cur].find(f) == frst[cur].end()) {
				prot.insert(cur);
				frst[cur].insert(f);
				cur = par[cur];
			}
			if (cur != -1 && frst[cur].find(f) == frst[cur].end()) {
				frst[cur].insert(f);

				// Catch-up.
				while (dep[frh[f]] > dep[cur]) {
					prot.insert(frh[f]);
					frh[f] = par[frh[f]];
					frst[frh[f]].insert(f);
				}

				while (frh[f] != cur) {
					prot.insert(frh[f]);
					prot.insert(cur);
					frh[f] = par[frh[f]];
					cur = par[cur];
					frst[frh[f]].insert(f);
					frst[cur].insert(f);
				}
			}
		}

		cout << "Case #" << t + 1 << ": " << N - 1 - prot.size() << '\n';
	}

	return 0;
}
