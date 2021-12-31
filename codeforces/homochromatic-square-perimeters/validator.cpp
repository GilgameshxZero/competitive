#include <iostream>
#include <string>
#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {
	registerValidation(argc, argv);
	int N = inf.readInt(1, 5000, "N");
	inf.readEoln();
	string pattern{"[0-1]{" + to_string(N) + "," + to_string(N) + "}"};
	for (int i = 0; i < N; i++) {
		inf.readToken(pattern);
		inf.readEoln();
	}
	inf.readEof();
}
