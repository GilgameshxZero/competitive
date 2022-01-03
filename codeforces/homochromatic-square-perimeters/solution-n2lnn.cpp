#define _CRT_SECURE_NO_WARNINGS
#pragma GCC target("avx2")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")

#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>

// Shorthand for common types.
template <typename Type, std::size_t Size>
using AR = std::array<Type, Size>;
using namespace std;

short const MAX_N = 5000;
// Reusing arrays for memory conservation.
AR<AR<short, MAX_N>, MAX_N> L, U, &L_ = L, &U_ = U, R__, &R_ = L;
AR<AR<char, MAX_N>, MAX_N> G;
AR<short, MAX_N + 1> _S__, _S_;
short *S__ = &_S__[1], *S_ = &_S_[1];
AR<pair<short, short>, MAX_N> rs;

short find(short i) {
	if (S_[i] < 0) {
		return i;
	}
	return S_[i] = find(S_[i]);
}

short join(short i, short j) {
	auto pI = find(i), pJ = find(j);
	if (pI == pJ) {
		return pI;
	}
	if (S_[pI] < S_[pJ]) {
		swap(pI, pJ);
	}
	S_[pJ] += S_[pI];
	return S_[pI] = pJ;
}

int main(int argc, char const *argv[]) {
#ifndef ONLINE_JUDGE
	std::freopen("in.txt", "r", stdin);
	std::freopen("out.txt", "w", stdout);
#endif
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);

	short N;
	cin >> N;
	cin.get();
	for (short i = 0; i < N; i++) {
		cin.read(&G[i][0], N);
		cin.get();
	}
	for (short i = 0; i < N; i++) {
		L[i][0] = G[i][0] - '0';
		U[0][i] = G[0][i] - '0';
	}
	for (short i = 0; i < N; i++) {
		for (short j = 1; j < N; j++) {
			L[i][j] = G[i][j] == '0' ? 0 : L[i][j - 1] + 1;
		}
	}
	for (short i = 1; i < N; i++) {
		for (short j = 0; j < N; j++) {
			U[i][j] = G[i][j] == '0' ? 0 : U[i - 1][j] + 1;
		}
	}
	for (short i = 0; i < N; i++) {
		for (short j = 0; j < N; j++) {
			R__[i][j] = min(L[i][j], U[i][j]) - i;
		}
	}
	for (short i = 0; i < N; i++) {
		L_[i][N - 1] = G[i][N - 1] - '0';
		U_[N - 1][i] = G[N - 1][i] - '0';
	}
	for (short i = 0; i < N; i++) {
		for (short j = N - 2; j > -1; j--) {
			L_[i][j] = G[i][j] == '0' ? 0 : L_[i][j + 1] + 1;
		}
	}
	for (short i = N - 2; i > -1; i--) {
		for (short j = 0; j < N; j++) {
			U_[i][j] = G[i][j] == '0' ? 0 : U_[i + 1][j] + 1;
		}
	}
	for (short i = 0; i < N; i++) {
		for (short j = 0; j < N; j++) {
			R_[i][j] = min(L_[i][j], U_[i][j]);
		}
	}

	short ans = 0, rsim;
	for (short D = -N + 1; D < N; D++) {
		memset(&S_[-1], -1, sizeof(short) * (N + 1));
		memset(&S__[-1], 0, sizeof(short) * (N + 1));

		// Radix sort R'' on D.
		short iMax = min(N + D - 1, N - 1), iMin = max(D - 1, -1);
		rsim = 0;
		for (short i = iMax; i > iMin; i--) {
			short j = i - D;
			rs[rsim++] = {1 - R__[i][j], i};
		}
		sort(rs.begin(), rs.begin() + rsim);
		short rsi = rsim - 1;

		// Process.
		for (short i = iMax; i > iMin; i--) {
			short j = i - D, t = 1 - i, t_ = R_[i][j] + i - 1;

			// All R'' on D less than t must be joined in S'.
			while (rsi >= 0 && rs[rsi].first >= 2 - t) {
				auto i_ = rs[rsi].second, dS1 = find(i_ - 1), dS2 = join(i_, i_ - 1);
				S__[dS2] = S__[dS1] + dS1 - dS2;
				rsi--;
			}

			// Update ans.
			auto dS = find(t_);
			ans = max(ans, (short)(S__[dS] + dS - i + 1));
		}
	}
	cout << ans << '\n';

	return 0;
}
