#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

namespace IO {
	const int BUFFER_SIZE = 1 << 20;

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

	inline void write_double_repeated(
		double number,
		char after = '\0',
		int places = 6,
		int repeats = 1) {
		_flush_output();

		int const rep = 50000;
		while (repeats > rep) {
			auto prev = output_pos;
			write_double(number, after, places);
			auto diff = output_pos - prev;
			auto target = output_pos + diff * (rep - 1);
			for (; output_pos != target; output_pos += diff) {
				memcpy(output_buffer + output_pos, output_buffer + prev, diff);
			}
			repeats -= rep;
			_flush_output();
		}

		auto prev = output_pos;
		write_double(number, after, places);
		auto diff = output_pos - prev;
		auto target = output_pos + diff * (repeats - 1);
		for (; output_pos != target; output_pos += diff) {
			memcpy(output_buffer + output_pos, output_buffer + prev, diff);
		}
	}

	void init() {
		// Make sure _flush_output() is called at the end of the program.
		bool exit_success = atexit(_flush_output) == 0;
		assert(exit_success);

		for (int i = 0; i < 100; i++) lookup[i] = (i / 10 << 4) + i % 10;
	}
}

const int maxn = 1000000;

int n;
int a[maxn], dp[maxn];
long long _pre[maxn + 1];
long long *pre = _pre + 1;

long long subsum(int l, int r) {
	return pre[r - 1] - pre[l - 1];
}

int main() {
	IO::init();

	int n;
	IO::read_int(n);

	for (int i = 0; i < n; i++) {
		IO::read_int(a[i]);
		pre[i] = a[i] + pre[i - 1];
	}

	// [i, dp[i]) has the lowest average
	// out of all of segments with left endpoint i

	// compute dp array
	dp[n - 1] = n;
	for (int i = n - 2; i >= 0; i--) {
		dp[i] = i + 1;
		while (dp[i] < n &&
					 subsum(i, dp[i]) * (dp[dp[i]] - i) >
						 subsum(i, dp[dp[i]]) * (dp[i] - i)) {
			dp[i] = dp[dp[i]];
		}
	}

	// Print answer
	for (int i = 0; i < n;) {
		int end = dp[i];
		if (end - i == 1) {
			IO::write_int(a[i], '\n');
			i++;
			continue;
		}
		double avg = 1.0 * subsum(i, end) / (end - i);

		int times = end - i;
		if (times > 3) {
			IO::write_double_repeated(avg, '\n', 10, times);
			i = end;
		} else {
			for (; i < end; i++) {
				IO::write_double(avg, '\n', 10);
			}
		}
	}
}
