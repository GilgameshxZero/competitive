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
	std::freopen("c-in.txt", "r", stdin);
	std::freopen("c-out.txt", "w", stdout);
#endif

	// Untie C I/O from C++ I/O.
	std::ios_base::sync_with_stdio(false);

	// Untie std::cin. Remember to flush std::cout manually on interactive
	// problems!
	std::cin.tie(nullptr);

	ll N;
	cin >> N;

	if (N == 1) {
		cout << 0;
		return 0;
	}

	vector<ll> seq(N);
	for (ll i = 0; i < N; i++) {
		cin >> seq[i];
	}

	struct ss {
		ll lext, l, r, rext, total;
	};
	vector<ss> sss(N / 2);
	for (ll i = 0; i < sss.size(); i++) {
		ll p = min(seq[i * 2], seq[i * 2 + 1]);
		sss[i] = {seq[i * 2] - p, 0, 0, seq[i * 2 + 1] - p, p};
		if (sss[i].lext == 0) {
			sss[i].l = 1;
		}
		if (sss[i].rext == 0) {
			sss[i].r = 1;
		}
	}

	while (sss.size() > 1) {
		ll i = 0;
		for (; i < sss.size() - 1; i++) {
			if (sss[i].rext == 0 && sss[i + 1].lext == 0) {
				break;
			}
		}
		if (i == sss.size() - 1) {
			i--;
		}

		ss ss2 = sss[i + 1];
		ss ss1 = sss[i];
		sss.erase(sss.begin() + i + 1);
		sss.erase(sss.begin() + i);

		ll wrap = min(ss1.lext, ss2.rext);
		auto it = sss.insert(
			sss.begin() + i,
			{ss1.lext - wrap,
			 0,
			 0,
			 ss2.rext - wrap,
			 ss1.total + ss2.total + ss1.r * ss2.l + wrap});

		if (wrap == 0) {
			if (it->lext == 0) {
				it->l = ss1.l + (ss1.lext + ss1.rext == 0) * ss2.l;
			}
			if (it->rext == 0) {
				it->r = ss2.r + (ss2.lext + ss2.rext == 0) * ss1.r;
			}
		} else {
			it->l = it->lext == 0;
			it->r = it->rext == 0;
		}
	}

	cout << sss.front().total;
	return 0;
}
