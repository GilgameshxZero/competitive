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
#include <climits>
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

// Shorthand for common types.
using zu = std::size_t;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
template <typename First, typename Second>
using pr = std::pair<First, Second>;
template <typename Type>
using vr = std::vector<Type>;

#define rf(X, F, T) for (ll X = F; X != T; X += (F < T) * 2 - 1)

namespace IO {
	const int BUFFER_SIZE = 1 << 16;

	char input_buffer[BUFFER_SIZE];
	int input_pos = 0, input_len = 0;

	char output_buffer[BUFFER_SIZE];
	int output_pos = 0;

	char number_buffer[100];
	uint8_t lookup[100];

	void _update_input_buffer() {
		input_len = fread(input_buffer, sizeof(char), BUFFER_SIZE, stdin);
		input_pos = 0;

		if (input_len == 0) input_buffer[0] = EOF;
	}

	inline char next_char(bool advance = true) {
		if (input_pos >= input_len) _update_input_buffer();

		return input_buffer[advance ? input_pos++ : input_pos];
	}

	template <typename T>
	inline void read_int(T &number) {
		bool negative = false;
		number = 0;

		while (!isdigit(next_char(false)))
			if (next_char() == '-') negative = true;

		do {
			number = 10 * number + (next_char() - '0');
		} while (isdigit(next_char(false)));

		if (negative) number = -number;
	}

	template <typename T, typename... Args>
	inline void read_int(T &number, Args &...args) {
		read_int(number);
		read_int(args...);
	}

	inline void read_double(double &number) {
		bool negative = false;
		number = 0;

		while (!isdigit(next_char(false)))
			if (next_char() == '-') negative = true;

		do {
			number = 10 * number + (next_char() - '0');
		} while (isdigit(next_char(false)));

		if (next_char(false) == '.') {
			next_char();

			for (double multiplier = 0.1; isdigit(next_char(false));
					 multiplier *= 0.1)
				number += multiplier * (next_char() - '0');
		}

		if (negative) number = -number;
	}

	void _flush_output() {
		fwrite(output_buffer, sizeof(char), output_pos, stdout);
		output_pos = 0;
	}

	inline void write_char(char c) {
		if (output_pos == BUFFER_SIZE) _flush_output();

		output_buffer[output_pos++] = c;
	}

	template <typename T>
	inline void write_int(T number, char after = '\0') {
		if (number < 0) {
			write_char('-');
			number = -number;
		}

		int length = 0;

		while (number >= 10) {
			uint8_t lookup_value = lookup[number % 100];
			number /= 100;
			number_buffer[length++] = (lookup_value & 15) + '0';
			number_buffer[length++] = (lookup_value >> 4) + '0';
		}

		if (number != 0 || length == 0) write_char(number + '0');

		for (int i = length - 1; i >= 0; i--) write_char(number_buffer[i]);

		if (after) write_char(after);
	}

	inline void write_double(double number, char after = '\0', int places = 6) {
		static double const negp[10] = {
			1, 1e-1, 1e-2, 1e-3, 1e-4, 1e-5, 1e-6, 1e-7, 1e-8, 1e-9};

		// Round up the number according to places.
		number += 0.5 * negp[places];
		int floored = number;
		write_int(floored);

		number -= floored;

		if (places > 0) {
			write_char('.');

			while (places >= 2) {
				number *= 100;
				int two = (int)number;
				number -= two;
				uint8_t lookup_value = lookup[two];
				write_char((lookup_value >> 4) + '0');
				write_char((lookup_value & 15) + '0');
				places -= 2;
				if (number < 1e-6) {
					break;
				}
			}

			if (places == 1) {
				number *= 10;
				int one = (int)number;
				write_char(one + '0');
			}
		}

		if (after) write_char(after);
	}

	void init() {
		// Make sure _flush_output() is called at the end of the program.
		bool exit_success = atexit(_flush_output) == 0;
		assert(exit_success);

		for (int i = 0; i < 100; i++) lookup[i] = (i / 10 << 4) + i % 10;
	}
}

ll const MOD = 998244353;

ll A[100000];

int main(int argc, char const *argv[]) {
	// Redirect I/O to/from files if running locally.
#ifndef ONLINE_JUDGE
	std::freopen("in.txt", "r", stdin);
	std::freopen("out.txt", "w", stdout);
#endif

	IO::init();

	ll T;
	IO::read_int(T);
	while (T--) {
		ll N;
		IO::read_int(N);
		rf(i, 0, N) { IO::read_int(A[i]); }

		ll total = 0, tt = 0, last = 0;
		rf(i, 1, N) {
			if (A[i] >= A[i - 1]) {
				total = (total + tt) % MOD;
			} else {
				last = tt = 0;
				ll limit = A[i], d;
				rf(j, i - 1, -1) {
					if (A[j] > limit) {
						d = (A[j] + limit - 1) / limit;
						last += d - 1;
						tt += last;
						limit = A[j] / d;
					} else {
						limit = A[j];
						tt += last;
					}
				}
				total = (total + tt) % MOD;
			}
		}
		IO::write_int(total, '\n');
	}

	return 0;
}
