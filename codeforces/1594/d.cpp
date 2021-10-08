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

vector<set<ll>> chsT, chsF;
vector<ll> bp;
bool flag;

void ffBP(ll cur, ll curBP) {
	if (!flag) {
		return;
	}
	bp[cur] = curBP;
	for (auto &ch : chsF[cur]) {
		if (bp[ch] == 0) {
			ffBP(ch, -curBP);
		} else if (bp[ch] != -curBP) {
			flag = false;
			return;
		}
	}
	for (auto &ch : chsT[cur]) {
		if (bp[ch] == 0) {
			ffBP(ch, curBP);
		} else if (bp[ch] != curBP) {
			flag = false;
			return;
		}
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
		ll N, M;
		cin >> N >> M;

		chsT.clear();
		chsT.resize(N);
		chsF.clear();
		chsF.resize(N);
		for (ll i = 0; i < M; i++) {
			ll X, Y;
			string S;
			cin >> X >> Y >> S;
			if (S[0] == 'i') {
				chsF[X - 1].insert(Y - 1);
				chsF[Y - 1].insert(X - 1);
			} else {
				chsT[X - 1].insert(Y - 1);
				chsT[Y - 1].insert(X - 1);
			}
		}

		ll cBP = 0;
		bp.clear();
		bp.resize(N, 0);
		flag = true;
		for (ll i = 0; i < N && flag; i++) {
			if (bp[i] == 0) {
				ffBP(i, ++cBP);
			}
		}
		if (!flag) {
			cout << "-1\n";
			continue;
		}

		vector<pair<ll, ll>> bpss(cBP);
		for (ll i = 0; i < N; i++) {
			if (bp[i] < 0) {
				bpss[-bp[i] - 1].first++;
			} else {
				bpss[bp[i] - 1].second++;
			}
		}

		ll ans = 0;
		for (auto &bps : bpss) {
			ans += max(bps.first, bps.second);
		}
		cout << ans << '\n';
	}

	return 0;
}
