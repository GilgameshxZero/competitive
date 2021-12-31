#include <iostream>
#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {
	registerGen(argc, argv, 1);
	int N = atoi(argv[1]);
	cout << N << endl;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << (char)(1 - rnd.next(0, 100) / 90 + '0');
		}
		cout << endl;
	}
}
