#include <bits/stdc++.h>
using namespace std;

namespace IO {
	const int BUFFER_SIZE = 1 << 25;

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

typedef long long ll;
#define mod1 (ll)1000000007
#define mod2 (ll)998244353
#define pll pair<ll, ll>
typedef long double lb;
#define eps (lb)(1e-9)
vector<pll> adj[500000];
ll dist[500000];
bool visited[500000];

void dijkstra() {
	dist[0] = 0;
	priority_queue<pll, vector<pll>, greater<pll>> pq;
	pq.push({0, 0});
	while (pq.size()) {
		auto cur = pq.top();
		pq.pop();
		if (visited[cur.second]) {
			continue;
		}
		for (auto j : adj[cur.second]) {
			if (dist[j.first] > (dist[cur.second] + j.second)) {
				dist[j.first] = (dist[cur.second] + j.second);
				pq.push({dist[j.first], j.first});
			}
		}
		visited[cur.second] = 1;
	}
}
class DisjSet {
	int *rank, *parent, n;

	public:
	DisjSet(int n) {
		rank = new int[n];
		parent = new int[n];
		this->n = n;
		makeSet();
	}

	void makeSet() {
		for (int i = 0; i < n; i++) {
			parent[i] = i;
		}
	}

	int find(int x) {
		if (parent[x] != x) {
			parent[x] = find(parent[x]);
		}

		return parent[x];
	}

	void Union(int x, int y) {
		int xset = find(x);
		int yset = find(y);

		if (xset == yset) return;

		if (rank[xset] < rank[yset]) {
			parent[xset] = yset;
		} else if (rank[xset] > rank[yset]) {
			parent[yset] = xset;
		}

		else {
			parent[yset] = xset;
			rank[xset] = rank[xset] + 1;
		}
	}
};
void solve();
int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	IO::init();

	ll t = 1, n = 1;
	// cin>>t;
	while (t--) {
		solve();
	}
	return 0;
}
void solve() {
	ll n, k, d;
	ll bil = (1e9) + (0.1);

	// cin >> n >> k >> d;
	IO::read_int(n);
	IO::read_int(k);
	IO::read_int(d);

	//**************
	// IMPORTANT PART
	//**************

	bool debug = n == 300000 && d == 2;

	set<ll> s;

	ll inputtime = 0, inserttime = 0;

	for (int i(0); i < k; ++i) {
		ll pp;
		if (debug && i % 50000 == 0) {
			cout << i << ":\n";
			cout << "Input Time: " << inputtime / 1000000 << "ms\n";
			cout << "Insert Time: " << inserttime / 1000000 << "ms\n";
			cout << "Set Size: " << s.size() << endl;
		}
		auto start = chrono::high_resolution_clock::now();
		IO::read_int(pp);
		// cin >> pp;
		auto mid = chrono::high_resolution_clock::now();
		inputtime +=
			chrono::duration_cast<chrono::nanoseconds>(mid - start).count();

		s.insert(pp);
		inserttime += chrono::duration_cast<chrono::nanoseconds>(
										chrono::high_resolution_clock::now() - mid)
										.count();
	}

	if (debug) {
		cout << "Final Input Time: " << inputtime / 1000000 << "ms\n";
		cout << "Final Insert Time: " << inserttime / 1000000 << "ms\n";
		cout << "Final Set Size: " << s.size() << '\n';
		return;
	}
	//*********************
	// END OF IMPORTANT PART
	//*********************

	vector<pll> v;
	for (int i(1); i < n; ++i) {
		ll a, b;
		IO::read_int(a);
		IO::read_int(b);
		// cin >> a >> b;
		adj[a].push_back({b, bil});
		adj[b].push_back({a, bil});
		if (a > b) {
			swap(a, b);
		}
		v.push_back({a, b});
	}
	k = s.size();
	cout << k - 1 << '\n';
	if (k == 1) {
		return;
	}
	for (int i(0); i <= n; ++i) {
		dist[i] = LLONG_MAX;
		visited[i] = 0;
	}
	for (auto l : s) {
		adj[0].push_back({l, l});
		adj[l].push_back({0, l});
	}
	dijkstra();
	DisjSet dsu(n + 1);
	for (int i(1); i <= n; ++i) {
		dsu.Union(i, dist[i] % bil);
	}
	for (int i(0); i < v.size(); ++i) {
		if (dsu.find(v[i].first) != dsu.find(v[i].second)) {
			cout << i + 1 << " ";
		}
	}
	cout << '\n';
	return;
}
