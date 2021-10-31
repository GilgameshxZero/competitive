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

// Fast I/O setup and utilities.
class IO {
	public:
	IO() {
		// Redirect I/O to/from files if running locally.
#ifndef ONLINE_JUDGE
		std::freopen("in.txt", "r", stdin);
		std::freopen("out.txt", "w", stdout);
#endif

		// Untie C I/O from C++ I/O. Do not intersperse printf/scanf with cin/cout.
		std::ios_base::sync_with_stdio(false);

		// Untie std::cin. Remember to flush std::cout manually on interactive
		// problems!
		std::cin.tie(nullptr);
	}
};
IO io;

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

// Shorthand for common types.
using ZU = std::size_t;
using LL = long long;
using ULL = unsigned long long;
using LD = long double;
template <typename First, typename Second>
using PR = std::pair<First, Second>;
template <typename Type>
using VR = std::vector<Type>;

// Shorthand for loop in range [from, to).
#define RF(x, from, to) \
	for (long long x = from, _rfDir = from < to ? 1 : -1; x != to; x += _rfDir)

// Imports std scope into global scope; care for name conflicts. Also imports
// literals in std::literals.
using namespace std;

// Most significant 1-bit for unsigned integral types of at most long long in
// size. Undefined result if x = 0.
template <typename Integer>
inline std::size_t mostSignificant1BitIdx(Integer const x) {
#ifdef __has_builtin
#if __has_builtin(__builtin_clzll)
	return 8 * sizeof(unsigned long long) - __builtin_clzll(x);
#endif
#endif
	for (std::size_t bit = 8 * sizeof(Integer) - 1;
			 bit != std::numeric_limits<std::size_t>::max();
			 bit--) {
		if (x & (static_cast<Integer>(1) << bit)) {
			return bit;
		}
	}
	return std::numeric_limits<std::size_t>::max();
}

// Least significant 1-bit for unsigned integral types of at most long long in
// size. Undefined result if x = 0.
template <typename Integer>
inline std::size_t leastSignificant1BitIdx(Integer const x) {
#ifdef __has_builtin
#if __has_builtin(__builtin_ctzll)
	return __builtin_ctzll(x);
#endif
#endif
	for (std::size_t bit = 0; bit != 8 * sizeof(Integer); bit++) {
		if (x & (static_cast<Integer>(1) << bit)) {
			return bit;
		}
	}
	return std::numeric_limits<std::size_t>::max();
}

// Count of 1-bits in unsigned integral types of at most long long in size.
template <typename Integer>
inline std::size_t bitPopcount(Integer const x) {
#ifdef __has_builtin
#if __has_builtin(__builtin_popcountll)
	return __builtin_popcountll(x);
#endif
#endif
	std::size_t count = 0;
	for (std::size_t bit = 0; bit != 8 * sizeof(Integer); bit++) {
		count += !!(x & (static_cast<Integer>(1) << bit));
	}
	return count;
}

// GCD using Euclidean algorithm.
template <typename Integer>
inline Integer greatestCommonDivisor(Integer x, Integer y) {
	if (x > y) {
		std::swap(x, y);
	}
	while (x != 0) {
		y %= x;
		std::swap(x, y);
	}
	return y;
}

// LCM. Integer type must be large enough to store product.
template <typename Integer>
inline Integer leastCommonMultiple(Integer const x, Integer const y) {
	return x * y / greatestCommonDivisor(x, y);
}

// Segment values with lazy propagation, supporting range queries and range
// updates in O(ln N) and O(N) memory.
//
// RVO helps with heavier Value types, but is not guaranteed. Value must be
// zero-initialized to be valid. Update must be light-copyable.
//
// For parent i, 2i + 1 is the left child, and 2i + 2 is the right child. All
// left children are odd and all right children are even.
template <typename Value, typename Update>
class SegmentTree {
	private:
	// Aggregate values at each node.
	std::vector<Value> values;

	// Size of underlying array.
	std::size_t size,
		// First leaf node.
		firstLeaf;

	// True iff node has a pending lazy update to propagate.
	std::vector<bool> lazy;

	// Lazily-stored updates.
	std::vector<Update> updates;

	public:
	// Segment tree for an underlying array of size size. Must be at least 2.
	SegmentTree(std::size_t const size)
			: values((1_zu << (mostSignificant1BitIdx(size - 1) + 1)) - 1 + size),
				size(size),
				firstLeaf(values.size() - size),
				lazy(firstLeaf, false),
				updates(firstLeaf) {}

	protected:
	// Aggregate values from two children. Aggregating with a
	// default-initialized Value should do nothing. The combined range of the
	// two children is supplied.
	//
	// The internal index of the parent is provided for convenience.
	virtual Value aggregate(
		Value const &left,
		Value const &right,
		std::pair<std::size_t, std::size_t> const &range,
		std::size_t node) = 0;

	// Propagate an update on a parent to its two children.
	//
	// The internal index of the parent is provided for convenience.
	virtual void propagate(
		Update const &update,
		Update &leftChild,
		Update &rightChild,
		std::pair<std::size_t, std::size_t> const &range,
		std::size_t node) = 0;

	// Apply an update to a node, with the range of the node supplied.
	//
	// The internal index of the parent is provided for convenience.
	virtual void apply(
		Value &value,
		Update const &update,
		std::pair<std::size_t, std::size_t> const &range,
		std::size_t node) = 0;

	public:
	// Queries a range, propagating if necessary then aggregating.
	Value query(std::size_t const left, std::size_t const right) {
		return this->query(left, right, 0, {0, this->firstLeaf});
	}

	// Lazy update a range.
	void update(
		std::size_t const left,
		std::size_t const right,
		Update const &update) {
		this->update(left, right, update, 0, {0, this->firstLeaf});
	}

	private:
	// Conditionally propagate a node if it is not a leaf and has an update to
	// propagate.
	void propagate(
		std::size_t node,
		std::pair<std::size_t, std::size_t> const &range) {
		if (node >= this->lazy.size() || !this->lazy[node]) {
			return;
		}

		// Perform the propagation, but take care not to go out of bounds.
		// Propagating to a leaf applies it immediately.
		std::vector<std::unique_ptr<Update>> tmpUpdates;
		Update *chUpdates[2];
		if (node * 2 + 1 < this->firstLeaf) {
			chUpdates[0] = &this->updates[node * 2 + 1];
			this->lazy[node * 2 + 1] = true;
		} else {
			tmpUpdates.emplace_back(new Update{});
			chUpdates[0] = tmpUpdates.back().get();
		}
		if (node * 2 + 2 < this->firstLeaf) {
			chUpdates[1] = &this->updates[node * 2 + 2];
			this->lazy[node * 2 + 2] = true;
		} else {
			tmpUpdates.emplace_back(new Update{});
			chUpdates[1] = tmpUpdates.back().get();
		}
		this->propagate(
			this->updates[node], *chUpdates[0], *chUpdates[1], range, node);
		if (node * 2 + 1 >= this->firstLeaf) {
			this->apply(
				this->values[node * 2 + 1],
				*chUpdates[0],
				{node * 2 + 1 - this->firstLeaf, node * 2 + 1 - this->firstLeaf},
				node * 2 + 1);
		}
		if (node * 2 + 2 >= this->firstLeaf) {
			this->apply(
				this->values[node * 2 + 2],
				*chUpdates[1],
				{node * 2 + 2 - this->firstLeaf, node * 2 + 2 - this->firstLeaf},
				node * 2 + 2);
		}

		// In any case, clear the update at this node so it doesn’t interfere with
		// later propagations.
		this->apply(this->values[node], this->updates[node], range, node);
		this->updates[node] = {};
		this->lazy[node] = false;
	}

	// Internal recursive query. range is the coverage range of the current node
	// and is inclusive.
	Value query(
		std::size_t const left,
		std::size_t const right,
		std::size_t const node,
		std::pair<std::size_t, std::size_t> const &range) {
		if (right < range.first || left > range.second) {
			return {};
		}
		this->propagate(node, range);

		// Base case.
		if (range.first >= left && range.second <= right) {
			return this->values[node];
		}

		std::size_t mid = (range.first + range.second) / 2;
		return this->aggregate(
			this->query(left, right, node * 2 + 1, {range.first, mid}),
			this->query(left, right, node * 2 + 2, {mid + 1, range.second}),
			range,
			node);
	}

	// Internal recursive update.
	void update(
		std::size_t const left,
		std::size_t const right,
		Update const &update,
		std::size_t const node,
		std::pair<std::size_t, std::size_t> const &range) {
		if (right < range.first || left > range.second) {
			return;
		}
		// Propagate even if this node is fully covered, since we don’t have a
		// function to combine two updates.
		this->propagate(node, range);

		// Base case.
		if (range.first >= left && range.second <= right) {
			if (node < this->firstLeaf) {
				this->updates[node] = update;
				this->lazy[node] = true;
			} else {
				this->apply(this->values[node], update, range, node);
			}
		} else {
			std::size_t mid = (range.first + range.second) / 2;
			this->update(left, right, update, node * 2 + 1, {range.first, mid});
			this->update(left, right, update, node * 2 + 2, {mid + 1, range.second});

			// Substitute parent value with aggregate after update has been
			// propagated. O(1).
			this->propagate(node * 2 + 1, {range.first, mid});
			this->propagate(node * 2 + 2, {mid + 1, range.second});
			this->values[node] = this->aggregate(
				this->values[node * 2 + 1], this->values[node * 2 + 2], range, node);
		}
	}
};

/* ---------------------------- End of template. ---------------------------- */

class Tree : public SegmentTree<LL, LL> {
	using SegmentTree<LL, LL>::SegmentTree;

	private:
	LL N;

	public:
	Tree(LL N) : SegmentTree(1LL << N), N(N) {}

	protected:
	// Aggregate values from two children. Aggregating with a
	// default-initialized Value should do nothing. The combined range of the
	// two children is supplied.
	//
	// The internal index of the parent is provided for convenience.
	virtual LL aggregate(
		LL const &left,
		LL const &right,
		std::pair<std::size_t, std::size_t> const &range,
		std::size_t node) override {
		LL depth = mostSignificant1BitIdx(node + 1);
		if ((this->N - depth) % 2) {
			return left | right;
		}
		return left ^ right;
	}

	// Propagate an update on a parent to its two children.
	//
	// The internal index of the parent is provided for convenience.
	virtual void propagate(
		LL const &update,
		LL &leftChild,
		LL &rightChild,
		std::pair<std::size_t, std::size_t> const &range,
		std::size_t node) override {
		// Never used, do nothing.
	}

	// Apply an update to a node, with the range of the update supplied. The
	// range of the update is a subset of the range of the node.
	//
	// The internal index of the parent is provided for convenience.
	virtual void apply(
		LL &value,
		LL const &update,
		std::pair<std::size_t, std::size_t> const &range,
		std::size_t node) override {
		// Only used on single nodes.
		value = update;
	}
};

int main(int argc, char const *argv[]) {
	LL N, M;
	cin >> N >> M;

	Tree tree(N);
	RF(i, 0, 1LL << N) {
		LL X;
		cin >> X;
		tree.update(i, i, X);
	}

	RF(i, 0, M) {
		LL X, Y;
		cin >> X >> Y;
		tree.update(X - 1, X - 1, Y);
		cout << tree.query(0, 1LL << N) << '\n';
	}

	return 0;
}
