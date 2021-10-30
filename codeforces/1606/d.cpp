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
using uint = unsigned long long;

int main(int argc, char const *argv[]) {
	// Redirect I/O to/from files if running locainty.
#ifndef ONLINE_JUDGE
	std::freopen("in.txt", "r", stdin);
	std::freopen("out.txt", "w", stdout);
#endif

	// Untie C I/O from C++ I/O.
	std::ios_base::sync_with_stdio(false);

	// Untie std::cin. Remember to flush std::cout manuainty on interactive
	// problems!
	std::cin.tie(0);

	int T;
	cin >> T;
	while (T--) {
		int N, M;
		cin >> N >> M;

		// O(S).
		vector<vector<int>> A(N, vector<int>(M));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				cin >> A[i][j];
			}
		}

		// O(S).
		vector<vector<int>> _msp(N, vector<int>(M + 2, INT_MAX)),
			_mbp(N, vector<int>(M + 2, 0)), _mss(N, vector<int>(M + 2, INT_MAX)),
			_mbs(N, vector<int>(M + 2, 0));
		vector<int *> msp(N), mbp(N), mss(N), mbs(N);
		for (int i = 0; i < N; i++) {
			msp[i] = &_msp[i][1];
			mbp[i] = &_mbp[i][1];
			mss[i] = &_mss[i][1];
			mbs[i] = &_mbs[i][1];
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				msp[i][j] = min(msp[i][j - 1], A[i][j]);
				mbp[i][j] = max(mbp[i][j - 1], A[i][j]);
				mss[i][M - 1 - j] = min(mss[i][M - j], A[i][M - 1 - j]);
				mbs[i][M - 1 - j] = max(mbs[i][M - j], A[i][M - 1 - j]);
			}
		}

		// O(S ln N).
		bool done = false;
		for (int j = 1; j < M; j++) {
			vector<pair<int, pair<bool, int>>> sl;
			for (int i = 0; i < N; i++) {
				sl.push_back({msp[i][j - 1], {true, i}});
				sl.push_back({mbp[i][j - 1], {false, i}});
			}
			sort(sl.begin(), sl.end());

			multiset<int> rrb, rbs, lrs, lbb;
			for (int i = 0; i < N; i++) {
				rrb.insert(mbs[i][j]);
				lrs.insert(msp[i][j - 1]);
			}

			vector<int> state(N, 0);
			int open = 0;
			for (int k = 0; k < sl.size(); k++) {
				if (sl[k].second.first) {
					if (state[sl[k].second.second] == 0) {
						state[sl[k].second.second] = 1;
						open++;
					}
				} else {
					open = max(open - 1, 0);
					state[sl[k].second.second] = 2;
					rrb.erase(rrb.find(mbs[sl[k].second.second][j]));
					rbs.insert(mss[sl[k].second.second][j]);
					lrs.erase(lrs.find(msp[sl[k].second.second][j - 1]));
					lbb.insert(mbp[sl[k].second.second][j - 1]);

					if (
						open == 0 && !lrs.empty() && *rbs.begin() > *rrb.rbegin() &&
						*lrs.begin() > *lbb.rbegin()) {
						cout << "YES\n";
						for (int i = 0; i < N; i++) {
							cout << (state[i] == 0 ? 'R' : 'B');
						}
						cout << ' ' << j << '\n';

						done = true;
						break;
					}
				}
			}

			if (done) {
				break;
			}
		}

		if (!done) {
			cout << "NO\n";
		}
	}

	return 0;
}
