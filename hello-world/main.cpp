#include <cstdio>
#include <iostream>

#if defined(__APPLE__) || defined(__MACH__)
#define RAIN_PLATFORM_MACOS
#endif

int main() {
#ifndef ONLINE_JUDGE
#ifdef RAIN_PLATFORM_MACOS
	std::freopen("../build/i.default.txt", "r", stdin);
	std::freopen("../build/o.default.txt", "w", stdout);
#endif
#endif

	std::cout << "Hello world! Enter a number: ";
	int number;
	std::cin >> number;
	std::cout << "That number doubled is " << number * 2 << '.' << std::endl;
	return 0;
}
