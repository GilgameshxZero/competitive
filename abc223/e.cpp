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

	ll X, Y, A, B, C;
	cin >> X >> Y >> A >> B >> C;

	if ((X + A - 1) / X + (X + B - 1) / X + (X + C - 1) / X <= Y) {
		cout << "Yes";
		return 0;
	}
	if ((Y + A - 1) / Y + (Y + B - 1) / Y + (Y + C - 1) / Y <= X) {
		cout << "Yes";
		return 0;
	}
	ll rem = X - (Y + A - 1) / Y;
	if (rem > 0 && (rem + B - 1) / rem + (rem + C - 1) / rem <= Y) {
		cout << "Yes";
		return 0;
	}
	rem = X - (Y + B - 1) / Y;
	if (rem > 0 && (rem + A - 1) / rem + (rem + C - 1) / rem <= Y) {
		cout << "Yes";
		return 0;
	}
	rem = X - (Y + C - 1) / Y;
	if (rem > 0 && (rem + A - 1) / rem + (rem + B - 1) / rem <= Y) {
		cout << "Yes";
		return 0;
	}

	rem = Y - (X + A - 1) / X;
	if (rem > 0 && (rem + B - 1) / rem + (rem + C - 1) / rem <= X) {
		cout << "Yes";
		return 0;
	}
	rem = Y - (X + B - 1) / X;
	if (rem > 0 && (rem + A - 1) / rem + (rem + C - 1) / rem <= X) {
		cout << "Yes";
		return 0;
	}
	rem = Y - (X + C - 1) / X;
	if (rem > 0 && (rem + A - 1) / rem + (rem + B - 1) / rem <= X) {
		cout << "Yes";
		return 0;
	}

	cout << "No";
	return 0;
}
