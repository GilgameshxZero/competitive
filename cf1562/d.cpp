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
	std::freopen("d-in.txt", "r", stdin);
	std::freopen("d-out.txt", "w", stdout);
#endif

	// Untie C I/O from C++ I/O.
	std::ios_base::sync_with_stdio(false);

	// Untie std::cin. Remember to flush std::cout manually on interactive
	// problems!
	std::cin.tie(nullptr);

	ll T;
	cin >> T;
	vector<ll> posts;
	vector<pair<pair<ll, ll>, ll>> qs;
	vector<pair<ll, string>> ans;
	unordered_multimap<ll, ll> decpos;
	while (T--) {
		ll N, Q;
		cin >> N >> Q;
		string S;
		cin >> S;

		posts.resize(N + 1);
		posts[N] = 0;
		for (ll i = N - 1; i >= 0; i--) {
			posts[i] = posts[i + 1] + (i % 2 == 0 ? 1 : -1) * ((S[i] == '+') * 2 - 1);
		}
		/*for (auto &post : posts) {
			cout << post << ' ';
		}
		cout << '\n';*/

		qs.clear();
		for (ll i = 0; i < Q; i++) {
			qs.push_back({{}, i});
			cin >> qs.back().first.first >> qs.back().first.second;
		}
		sort(qs.begin(), qs.end());

		decpos.clear();
		ans.clear();
		ll prevl = 0, prevr = 0;
		for (auto const &q : qs) {
			stringstream ss;
			ll discount = posts[q.first.second],
				 head = posts[q.first.first - 1] - discount;
			if (head == 0) {
				ss << "0\n";
				ans.push_back({q.second, ss.str()});
				continue;
			}
			ss << (head % 2 == 0 ? '2' : '1') << '\n';

			// one
			if (q.first.second == q.first.first) {
				ss << q.first.first << '\n';
				ans.push_back({q.second, ss.str()});
				continue;
			}

			// two even
			if (q.first.first + 1 == q.first.second && head % 2 == 0) {
				ss << q.first.first << ' ' << q.first.second << '\n';
				ans.push_back({q.second, ss.str()});
				continue;
			}

			// expand
			while (++prevr <= q.first.second - 1) {
				if (posts[prevr] + 1 == posts[prevr - 1]) {
					decpos.insert({posts[prevr], prevr});
				}
			}
			prevr--;

			// shrink
			while (++prevl <= q.first.first - 1) {
				if (posts[prevl] + 1 == posts[prevl - 1]) {
					decpos.erase(decpos.find(posts[prevl]));
				}
			}
			prevl--;

			if (head % 2 == 0) {
				ll tail = posts[q.first.second - 1] - discount;
				if (tail == 1) {
					auto it = decpos.find(head / 2 + discount);
					ss << it->second << ' ' << q.first.second << '\n';
				} else {
					auto it = decpos.find(head / 2 - 1 + discount);
					ss << it->second << ' ' << q.first.second << '\n';
				}
			} else {
				auto it = decpos.find(head / 2 + discount);
				ss << it->second << '\n';
			}
			ans.push_back({q.second, ss.str()});
		}

		sort(ans.begin(), ans.end());
		for (auto &a : ans) {
			cout << a.second;
		}
	}

	return 0;
}
