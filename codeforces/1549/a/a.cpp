// C++ template for coding competitions designed for C++11 support, primarily
// targeting MSVC++17.
//
// TODO: Investigate PCH support conditioned on ONLINE_JUDGE for faster
// local compilation.

// Disable security/deprecation warnings on MSVC++.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifndef _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

// Disable Unicode/enable multi-byte strings on Windows libraries.
#ifdef UNICODE
#undef UNICODE
#endif

#ifdef _UNICODE
#undef _UNICODE
#endif

// Prevents double declarations of min/max in the global scope from <algorithm>
// and libraries like <Windows.h>.
#ifndef NOMINMAX
#define NOMINMAX
#endif

// Preprocessors for platform/build detection.
#if defined(_WIN32) || defined(_WIN64)
#define PLATFORM_WINDOWS
#endif

#if defined(__APPLE__) || defined(__MACH__)
#define PLATFORM_MACOS
#endif

#if defined(__linux__) || defined(linux) || defined(__linux)
#define PLATFORM_LINUX
#endif

#if !defined(PLATFORM_WINDOWS) && !defined(PLATFORM_MACOS) && \
	!defined(PLATFORM_LINUX)
#define PLATFORM_OTHER
#endif

// Preprocessor stringification macro.
#ifndef STRINGIFY_INNER
#define STRINGIFY_INNER(x) #x
#endif

#ifndef STRINGIFY
#define STRINGIFY(x) STRINGIFY_INNER(x)
#endif

// Identify location in code.
#ifndef CALLER_LOCATION
#define CALLER_LOCATION ":" STRINGIFY(__LINE__)
#endif

#include <algorithm>
// Enabled in C++17.
// #include <any>
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
// Enabled with linker flags.
// #include <filesystem>
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
// Enabled in C++17.
// #include <optional>
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
// Enabled in C++17.
// #include <variant>
#include <vector>

// User-defined literals.
constexpr std::size_t operator"" _zu(unsigned long long value) {
	return static_cast<std::size_t>(value);
}
std::regex operator"" _re(char const *value, std::size_t) {
	return std::regex(value);
}

// Stream operator overloads for template template iterables.
template <
	template <typename, typename...>
	class Container,
	typename Contained,
	typename... ContainerArgs>
std::ostream &operator<<(
	std::ostream &stream,
	Container<Contained, ContainerArgs...> const &container) {
	auto it = container.begin();
	stream << '{' << *it;
	for (it++; it != container.end(); it++) {
		stream << ", " << *it;
	}
	return stream << '}';
}

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

// Imports std scope into global scope; care for name conflicts. Also imports
// literals in std::literals.
using namespace std;

int main(int argc, char const *argv[]) {
	// If running locally, log execution time.
#ifndef ONLINE_JUDGE
	WallTimeGuard wallTimeGuard;
#endif

	// Untie C I/O from C++ I/O.
	std::ios_base::sync_with_stdio(false);

	// Untie std::cin. Remember to flush std::cout manually on interactive
	// problems!
	std::cin.tie(nullptr);

	// Redirect I/O to/from files if running locally.
#ifndef ONLINE_JUDGE
#pragma warning(disable : 6031)
	std::freopen("a-in.txt", "r", stdin);
	std::freopen("a-out.txt", "w", stdout);
#pragma warning(default : 6031)
#endif

	size_t T;
	cin >> T;
	while (T--) {
		size_t P;
		cin >> P;
		cout << "2 " << P - 1 << '\n';
	}

	return 0;
}
