#include <iostream>
#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {
	registerGen(argc, argv, 1);
	int C{17}, N{11733}, K{12983};
	cout << C << endl;
	for (int i = 0; i < C; i++) {
		cout << N << endl << K;
		for (int i = 2; i <= N; i++) {
			cout << ' ' << i * K;
		}
		cout << endl;
	}
}
