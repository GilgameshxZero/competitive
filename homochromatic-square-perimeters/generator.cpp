#include <iostream>
#include "testlib.h"

using namespace std;

short const MAX_N = 5000;
array<array<bool, MAX_N>, MAX_N> grid;

int main(int argc, char *argv[]) {
	registerGen(argc, argv, 1);
	int N = atoi(argv[1]);
	cout << N << endl;

	// Draw N/2 squares.
	for (int l = 0; l * 2 < N; l++) {
		int k = rnd.next(1, N), I = rnd.next(0, N - k), J = rnd.next(0, N - k);
		for (int i = 0; i < k; i++) {
			grid[I][J + i] = grid[I + i][J] = grid[I + k - 1][J + i] =
				grid[I + i][J + k - 1] = true;
		}
	}

	// Flip other bits on 50% of the time.
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (grid[i][j] && rnd.next(0, N) < 1) {
				grid[i][j] = false;
			} else if (rnd.next(0, 99) < 50) {
				grid[i][j] = true;
			}
		}
	}

	// Output.
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << grid[i][j] ? '1' : '0';
		}
		cout << endl;
	}
}
