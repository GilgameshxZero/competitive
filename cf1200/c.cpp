// C++ template for coding competitions designed for C++11 support.

// GCC-specific optimizations.
#pragma GCC target("avx2", "bmi", "bmi2", "popcnt", "lzcnt")
#pragma GCC optimize("Ofast", "unroll-loops")

// Disable security/deprecation warnings on MSVC++ for freopen.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#if defined(__APPLE__) || defined(__MACH__)
#define RAIN_PLATFORM_MACOS
#endif

// Unfortunately MSVC does not support bits/stc++.h.
#include <algorithm>
#include <array>
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

// "Fast" I/O setup and utilities.
class IO {
	public:
	IO() {
		// On local MacOS, due to a bug in VSCode’s implementation of LLDB
		// attachment and the difficulty of codesigning GDB, redirect stdio
		// manually.
#ifndef ONLINEJUDGE
#ifdef RAIN_PLATFORM_MACOS
		std::freopen("../build/i.default.txt", "r", stdin);
		std::freopen("../build/o.default.txt", "w", stdout);
#endif
#endif

		// Untie C I/O from C++ I/O. Do not intersperse printf/scanf with cin/cout.
		std::ios_base::sync_with_stdio(false);

		// Untie std::cin. Remember to flush std::cout manually on interactive
		// problems!
		std::cin.tie(nullptr);
	}
} io;

// If running locally, log execution time.
#ifndef ONLINE_JUDGE
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
} wallTimeGuard;
#endif

// User-defined literals.
inline constexpr std::size_t operator"" _zu(unsigned long long value) {
	return static_cast<std::size_t>(value);
}
inline std::regex operator"" _re(char const *value, std::size_t) {
	return std::regex(value);
}

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

// Least-recently-used cache implemented with a linked list + hashmap. O(1)
// average access. Not thread-safe.
//
// Value will be moved constructed if passed as rvalue-reference. Same with
// key. Otherwise, they will be copy-constructed. Ensure the relevant
// constructors are defined.
template <
	typename Key,
	typename Value,
	typename Hash = std::hash<Key>,
	typename KeyEqual = std::equal_to<Key>>
class LruCache {
	private:
	typedef std::list<std::pair<Key const &, Value>> InternalList;
	typedef std::
		unordered_map<Key, typename InternalList::iterator, Hash, KeyEqual>
			InternalHashMap;

	// List stores key/value pairs in LRU order; hashMap looks up list iterators
	// based on key. Key reference in list points to key owned by hashMap.
	InternalList lruList;
	InternalHashMap hashMap;

	public:
	// Key/value pair capacity of the cache. Cannot be 0.
	std::size_t const capacity;

	LruCache(std::size_t capacity) : capacity(capacity) {}

	// Simple getters.
	std::size_t size() const noexcept { return this->lruList.size(); }
	bool empty() const noexcept { return this->lruList.empty(); }
	auto begin() const noexcept { return this->lruList.begin(); }
	auto end() const noexcept { return this->lruList.end(); }
	auto begin() noexcept { return this->lruList.begin(); }
	auto end() noexcept { return this->lruList.end(); }

	// Returns an iterator to the hashmap object if available, otherwise
	// this->end(). Updates LRU ordering.
	typename InternalList::iterator find(Key const &key) {
		typename InternalHashMap::iterator const findIt = this->hashMap.find(key);

		if (findIt == this->hashMap.end()) {
			return this->end();
		}

		this->lruList.splice(this->lruList.begin(), this->lruList, findIt->second);
		return this->lruList.begin();
	}

	// Access the value at a key. Throws if doesn't exist.
	Value &at(Key const &key) {
		auto findIt = this->find(key);
		if (findIt == this->end()) {
			throw std::out_of_range("Key does not exist in cache.");
		}
		return findIt->second;
	}

	// Insert a new key/value pair, or update an existing pair. In either case,
	// move the pair to the front of the cache.
	//
	// Arguments are forwarded to relevant constructors with universal
	// forwarding. std::move them in to move construct; otherwise, they will be
	// copy-constructed.
	template <typename KeyType, typename ValueType>
	std::pair<typename InternalList::iterator, bool> insertOrAssign(
		KeyType &&key,
		ValueType &&value) {
		typename InternalHashMap::iterator const findIt = this->hashMap.find(key);

		if (findIt != this->hashMap.end()) {
			// If key exists, simply move it to the front of the list.
			this->lruList.splice(
				this->lruList.begin(), this->lruList, findIt->second);

			// Move-assign value in; key does not need to be changed.
			this->lruList.begin()->second = std::move(value);
			return {this->lruList.begin(), false};
		}

		// If over capacity, evict.
		if (this->hashMap.size() >= this->capacity) {
			this->hashMap.erase(this->lruList.back().first);
			this->lruList.pop_back();
		}

		// Insert into hashMap first, with default value iterator.
		typename InternalHashMap::iterator mapIt;
		std::tie(mapIt, std::ignore) =
			this->hashMap.emplace(std::forward<KeyType>(key), this->lruList.end());

		// Insert value with key reference.
		this->lruList.emplace_front(mapIt->first, std::forward<ValueType>(value));

		// Give hashMap the correct iterator.
		mapIt->second = this->lruList.begin();
		return {this->lruList.begin(), true};
	}
};

// Compute partial match table (also known as the failure function) (used in
// KMP) for a string.
//
// The partial match table specifies where to "rewind" the matching process to
// if it failes on a given character.
inline std::vector<std::size_t> computeKmpPartialMatch(
	char const *const cStr,
	std::size_t const cStrLen) {
	std::vector<std::size_t> partialMatch(cStrLen + 1);

	// This represents -1, i.e. we can resume matching for the first character
	// of the string at the next position in the text.
	partialMatch[0] = SIZE_MAX;

	// How far into search string s we must begin (to our best knowledge) if we
	// mismatch.
	std::size_t candidate = 0;

	for (std::size_t a = 1; a < cStrLen; a++) {
		if (cStr[a] == cStr[candidate]) {
			partialMatch[a] = partialMatch[candidate];
		} else {
			partialMatch[a] = candidate;
			while (candidate != SIZE_MAX && cStr[a] != cStr[candidate]) {
				candidate = partialMatch[candidate];
			}
		}
		candidate++;
	}

	partialMatch[cStrLen] = candidate;
	return partialMatch;
}
inline std::vector<std::size_t> computeKmpPartialMatch(std::string const &s) {
	return computeKmpPartialMatch(s.c_str(), s.length());
}

// KMP search for needle in haystack, in O(N+M). Optionally pass a candidate
// parameter if want to continue from a previous search. Returns a pair of
// (match, candidate). The match is nullptr if no match found, otherwise
// returns a char * to the first character of the first match found. The
// candidate is the updated candidate at the termination of the search.
inline std::pair<char *, std::size_t> kmpSearch(
	char const *const haystack,
	std::size_t const haystackLen,
	char const *const needle,
	std::size_t const needleLen,
	std::vector<std::size_t> const &partialMatch = {},
	std::size_t candidate = 0) {
	// Use these variables, which are set based on whether or not default
	// partialMatch and candidate were passed in.
	std::vector<std::size_t> const &partialMatchProxy =
		partialMatch.size() == 0 ? computeKmpPartialMatch(needle) : partialMatch;

	char const *searchResult = nullptr;
	for (std::size_t a = 0; a < haystackLen;) {
		if (haystack[a] == needle[candidate]) {
			a++;
			candidate++;
			if (candidate == needleLen) {
				searchResult = haystack + a - candidate;
			}
		} else {
			// Use the partial match table to resume if a match fails.
			candidate = partialMatchProxy[candidate];
			if (candidate == SIZE_MAX) {
				a++;
				candidate = 0;
			}
		}
	}

	return std::make_pair(const_cast<char *>(searchResult), candidate);
}
inline std::pair<char *, std::size_t> kmpSearch(
	char const *const haystack,
	std::size_t haystackLen,
	std::string const &needle,
	std::vector<std::size_t> const &partialMatch = {},
	std::size_t candidate = 0) {
	return kmpSearch(
		haystack,
		haystackLen,
		needle.c_str(),
		needle.length(),
		partialMatch,
		candidate);
}
inline std::pair<std::size_t, std::size_t> kmpSearch(
	std::string const &haystack,
	char const *const needle,
	std::size_t const needleLen,
	std::vector<std::size_t> const &partialMatch = {},
	std::size_t candidate = 0) {
	std::pair<char *, std::size_t> searchRes = kmpSearch(
		haystack.c_str(),
		haystack.length(),
		needle,
		needleLen,
		partialMatch,
		candidate);
	return std::make_pair(searchRes.first - haystack.c_str(), searchRes.second);
}
inline std::pair<std::size_t, std::size_t> kmpSearch(
	std::string const &haystack,
	std::string const &needle,
	std::vector<std::size_t> const &partialMatch = {},
	std::size_t candidate = 0) {
	return kmpSearch(
		haystack, needle.c_str(), needle.length(), partialMatch, candidate);
}

// Fixed-size Fenwick/Binary-Indexed Tree implementation. O(ln N) point
// updates and range queries. Not thread-safe.
//
// Value must support commutative addition. In addition, default
// initialization should be equivalent to "empty".
template <typename Value = long long>
class FenwickTree {
	private:
	std::vector<Value> tree;

	public:
	// Creates a Fenwick tree, which may be resized by operations.
	FenwickTree(std::size_t const size) : tree(size) {}

	// Computes prefix sum up to and including idx.
	Value sum(std::size_t const idx) const {
		Value aggregate{};
		for (std::size_t i = idx; i != SIZE_MAX; i &= i + 1, i--) {
			aggregate += this->tree[i];
		}
		return aggregate;
	}

	// Modify index by a delta.
	void modify(std::size_t const idx, Value const &delta) {
		for (std::size_t i = idx; i < this->tree.size(); i |= i + 1) {
			this->tree[i] += delta;
		}
	}
};

// Segment tree with lazy propagation, supporting range queries and range
// updates in O(ln N) and O(N) memory.
//
// RVO helps with heavier Result types, but is not guaranteed. Value must be
// zero-initialized to be valid. Update must be light-copyable.
//
// Index 0 is unused. For parent i, 2i is the left child and 2i + 1 is the
// right child.
//
// Further performance can be had with manual inlining of the five functions,
// which are typically not inlined by the compiler due to their virtual-ness.
// Forced initialization of underlying vectors can be replaced with
// std::array, and on GCC, loop unrolling may be turned on for further
// optimization.
template <typename _Value, typename _Update, typename _Result = _Value>
class SegmentTree {
	protected:
	using Value = _Value;
	using Update = _Update;
	using Result = _Result;

	// Aggregate values at each node.
	std::vector<Value> values;

	// True iff node has a pending lazy update to propagate.
	std::vector<bool> lazy;

	// Lazily-stored updates.
	std::vector<Update> updates;

	// Aggregate values from two children while retracing an update. Aggregating
	// with a default-initialized Value or Update should do nothing.
	virtual void aggregate(
		std::size_t const node,
		typename std::vector<Value>::reference value,
		Value const &left,
		Value const &right,
		std::pair<std::size_t, std::size_t> const &range) = 0;

	// Aggregate two results from queries on children. Aggregating with a
	// default-initialized Result should do nothing.
	virtual Result aggregate(
		std::size_t const node,
		Result const &left,
		Result const &right,
		std::pair<std::size_t, std::size_t> const &range) = 0;

	// Propagate an update on a parent to its two children. Lazy bits for the
	// children are set beforehand, but can be unset in the function.
	virtual void split(
		std::size_t const node,
		Update const &update,
		typename std::vector<Update>::reference left,
		typename std::vector<Update>::reference right,
		std::pair<std::size_t, std::size_t> const &range) = 0;

	// Convert a Value at a leaf node to a Result for base case queries.
	virtual Result convert(std::size_t const node, Value const &value) = 0;

	// Apply an update fully to a lazy node.
	virtual void apply(
		std::size_t const node,
		typename std::vector<Value>::reference value,
		Update const &update,
		std::pair<std::size_t, std::size_t> const &range) = 0;

	public:
	// Segment tree for a segment array of size size.
	SegmentTree(std::size_t const size)
			: values(1_zu << (mostSignificant1BitIdx(size - 1) + 2)),
				lazy(values.size(), false),
				updates(values.size()) {}

	// Queries a range, propagating if necessary then aggregating.
	Result query(std::size_t const left, std::size_t const right) {
		return this->query(1, left, right, {0, this->values.size() / 2 - 1});
	}

	// Lazy update a range.
	void update(
		std::size_t const left,
		std::size_t const right,
		Update const &update) {
		this->update(1, left, right, update, {0, this->values.size() / 2 - 1});
	}

	private:
	// Conditionally propagate a node if it is not a leaf and has an update to
	// propagate.
	void propagate(
		std::size_t const node,
		std::pair<std::size_t, std::size_t> const &range) {
		if (!this->lazy[node]) {
			return;
		}

		// Propagating on a leaf applies it immediately. Otherwise, split the
		// update to children.
		if (node < this->values.size() / 2) {
			this->lazy[node * 2] = this->lazy[node * 2 + 1] = true;
			this->split(
				node,
				this->updates[node],
				this->updates[node * 2],
				this->updates[node * 2 + 1],
				range);
		}

		// Clear the update at this node so it doesn’t interfere with later
		// propagations.
		this->apply(node, this->values[node], this->updates[node], range);
		this->updates[node] = {};
		this->lazy[node] = false;
	}

	// Internal recursive query. range is the coverage range of the current node
	// and is inclusive.
	Result query(
		std::size_t const node,
		std::size_t const left,
		std::size_t const right,
		std::pair<std::size_t, std::size_t> const &range) {
		if (right < range.first || left > range.second) {
			return {};
		}
		this->propagate(node, range);

		// Base case.
		if (range.first >= left && range.second <= right) {
			return this->convert(node, this->values[node]);
		}

		std::size_t mid = (range.first + range.second) / 2;
		return this->aggregate(
			node,
			this->query(node * 2, left, right, {range.first, mid}),
			this->query(node * 2 + 1, left, right, {mid + 1, range.second}),
			range);
	}

	// Internal recursive update.
	void update(
		std::size_t const node,
		std::size_t const left,
		std::size_t const right,
		Update const &update,
		std::pair<std::size_t, std::size_t> const &range) {
		if (right < range.first || left > range.second) {
			return;
		}
		// Propagate even if this node is fully covered, since we don’t have a
		// function to combine two updates.
		this->propagate(node, range);

		// Base case.
		if (range.first >= left && range.second <= right) {
			// This node is already non-lazy since it was just propagated.
			this->updates[node] = update;
			this->lazy[node] = true;
		} else {
			std::size_t mid = (range.first + range.second) / 2;
			this->update(node * 2, left, right, update, {range.first, mid});
			this->update(node * 2 + 1, left, right, update, {mid + 1, range.second});

			// Substitute parent value with aggregate after update has been
			// propagated. O(1). Guaranteed at least one child, or else the base
			// case would have triggered.
			this->propagate(node * 2, {range.first, mid});
			this->propagate(node * 2 + 1, {mid + 1, range.second});
			this->aggregate(
				node,
				this->values[node],
				this->values[node * 2],
				this->values[node * 2 + 1],
				range);
		}
	}
};

// Union-Find/Disjoint-Set-Union implementation. Near-constant time amortized
// union and find.
//
// Implements path compression and union by rank.
class DisjointSetUnion {
	private:
	// A pair of (is_root, X). If node is root, X stores the size of the
	// cluster. Otherwise, X stores the index of the node’s parent.
	mutable std::vector<std::pair<bool, std::size_t>> nodes;

	public:
	DisjointSetUnion(std::size_t const size) : nodes(size, {true, 1}) {}

	std::size_t find(std::size_t const i) const {
		if (this->nodes[i].first) {
			return i;
		}
		return this->nodes[i].second = this->find(this->nodes[i].second);
	}
	std::size_t rank(std::size_t const i) const {
		return this->nodes[this->find(i)].second;
	}
	void join(std::size_t const i, std::size_t const j) {
		std::size_t pI = this->find(i), pJ = this->find(j);
		if (pI == pJ) {
			return;
		}
		if (this->nodes[pI].second > this->nodes[pJ].second) {
			std::swap(pI, pJ);
		}
		this->nodes[pJ].second += this->nodes[pI].second;
		this->nodes[pI] = {false, pJ};
	}
};

// Implementation for a prime modulus ring over the integers, supporting basic
// operations add, subtract, multiply in O(1) and divide in O(ln N).
//
// Integer must be large enough to store (primeModulus - 1)^2.
//
// For O(1) division, cache multiplicative inverses and multiply with those.
template <typename Integer, Integer primeModulus>
class ModRing {
	public:
	Integer value;

	// Add primeModulus first to wrap back around in the case of "negative"
	// unsigned Integer.
	ModRing(Integer const value = 0)
			: value((primeModulus + value) % primeModulus) {}

	// O(ln N) exponentiation.
	static ModRing<Integer, primeModulus> power(
		ModRing<Integer, primeModulus> const base,
		Integer const exponent) {
		if (exponent == 0) {
			return {1};
		}
		auto half = power(base, exponent / 2);
		if (exponent % 2 == 0) {
			return half * half;
		} else {
			return half * half * base;
		}
	}

	ModRing<Integer, primeModulus> operator+(Integer const other) const {
		return *this + ModRing<Integer, primeModulus>(other);
	}
	ModRing<Integer, primeModulus> operator+(Integer const other) {
		return *this + ModRing<Integer, primeModulus>(other);
	}
	ModRing<Integer, primeModulus> operator+(
		ModRing<Integer, primeModulus> const other) {
		return {this->value + other.value};
	}
	ModRing<Integer, primeModulus> operator+=(
		ModRing<Integer, primeModulus> const other) {
		return *this = *this + other;
	}
	ModRing<Integer, primeModulus> operator++() { return *this += 1; }
	ModRing<Integer, primeModulus> operator++(int) {
		auto tmp(*this);
		*this += 1;
		return tmp;
	}
	ModRing<Integer, primeModulus> operator-(Integer const other) const {
		return *this - ModRing<Integer, primeModulus>(other);
	}
	ModRing<Integer, primeModulus> operator-(Integer const other) {
		return *this - ModRing<Integer, primeModulus>(other);
	}
	ModRing<Integer, primeModulus> operator-(
		ModRing<Integer, primeModulus> const other) {
		return {this->value - other.value};
	}
	ModRing<Integer, primeModulus> operator-=(
		ModRing<Integer, primeModulus> const other) {
		return *this = *this - other;
	}
	ModRing<Integer, primeModulus> operator--() { return *this -= 1; }
	ModRing<Integer, primeModulus> operator--(int) {
		auto tmp(*this);
		*this -= 1;
		return tmp;
	}
	ModRing<Integer, primeModulus> operator*(Integer const other) const {
		return *this * ModRing<Integer, primeModulus>(other);
	}
	ModRing<Integer, primeModulus> operator*(Integer const other) {
		return *this * ModRing<Integer, primeModulus>(other);
	}
	ModRing<Integer, primeModulus> operator*(
		ModRing<Integer, primeModulus> const other) {
		return {this->value * other.value};
	}
	ModRing<Integer, primeModulus> operator*=(
		ModRing<Integer, primeModulus> const other) {
		return *this = *this * other;
	}
	ModRing<Integer, primeModulus> operator/(Integer const other) const {
		return *this / ModRing<Integer, primeModulus>(other);
	}
	ModRing<Integer, primeModulus> operator/(Integer const other) {
		return *this / ModRing<Integer, primeModulus>(other);
	}
	ModRing<Integer, primeModulus> operator/(
		ModRing<Integer, primeModulus> const other) {
		return *this * power(other, primeModulus - 2);
	}
	ModRing<Integer, primeModulus> operator/=(
		ModRing<Integer, primeModulus> const other) {
		return *this = *this / other;
	}

	bool operator==(Integer const other) {
		return *this == ModRing<Integer, primeModulus>(other);
	}
	bool operator==(ModRing<Integer, primeModulus> const other) {
		return this->value == other.value;
	}
};

template <typename LeftInteger, typename Integer, Integer primeModulus>
ModRing<Integer, primeModulus> operator+(
	LeftInteger const left,
	ModRing<Integer, primeModulus> const right) {
	return ModRing<Integer, primeModulus>(left) + right;
}

template <typename LeftInteger, typename Integer, Integer primeModulus>
ModRing<Integer, primeModulus> operator-(
	LeftInteger const left,
	ModRing<Integer, primeModulus> const right) {
	return ModRing<Integer, primeModulus>(left) - right;
}

template <typename LeftInteger, typename Integer, Integer primeModulus>
ModRing<Integer, primeModulus> operator*(
	LeftInteger const left,
	ModRing<Integer, primeModulus> const right) {
	return ModRing<Integer, primeModulus>(left) * right;
}

template <typename LeftInteger, typename Integer, Integer primeModulus>
ModRing<Integer, primeModulus> operator/(
	LeftInteger const left,
	ModRing<Integer, primeModulus> const right) {
	return ModRing<Integer, primeModulus>(left) / right;
}

// Ease-of-use streaming operator.
template <typename Integer, Integer primeModulus>
inline std::ostream &operator<<(
	std::ostream &stream,
	ModRing<Integer, primeModulus> const right) {
	return stream << right.value;
}

// Compute the `index`-th Fibonacci matrix in $O(\log N)$ time. Helper
// function to fibonacciNumber. `index` must be positive.
template <typename Integer = std::size_t>
inline std::pair<std::pair<Integer, Integer>, std::pair<Integer, Integer>>
fibonacciMatrix(std::size_t const index) {
	if (index == 1) {
		return {{1, 1}, {1, 0}};
	}

	if (index % 2 == 0) {
		auto sub{fibonacciMatrix<Integer>(index / 2)};
		return {
			{sub.first.first * sub.first.first + sub.first.second * sub.second.first,
			 sub.first.first * sub.first.second +
				 sub.first.second * sub.second.second},
			{sub.second.first * sub.first.first +
				 sub.second.second * sub.second.first,
			 sub.second.first * sub.first.second +
				 sub.second.second * sub.second.second}};
	} else {
		auto sub{fibonacciMatrix<Integer>(index - 1)};
		return {
			{sub.first.first + sub.first.second, sub.first.first},
			{sub.second.first + sub.second.second, sub.second.first}};
	}
}

// Compute the `index`-th Fibonacci number in $O(\log N)$ time with repeated
// exponentiation on the 2-by-2 matrix. `index` must be non-negative.
// fibonacciNumber(0) is defined as 0.
template <typename Integer = std::size_t>
inline Integer fibonacciNumber(std::size_t const index) {
	return fibonacciMatrix<Integer>(index + 1).second.second;
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
template <typename Type, std::size_t Size>
using AR = std::array<Type, Size>;

// Shorthand for loop in range [from, to).
#define RF(x, from, to) \
	for (long long x = from, rfDir = from < to ? 1 : -1; x != to; x += rfDir)

// Imports std scope into global scope; care for name conflicts. Also imports
// literals in std::literals.
using namespace std;

/* ---------------------------- End of template. ---------------------------- */

int main(int, char const *[]) {
	LL N, M, Q;
	cin >> N >> M >> Q;
	LL g{greatestCommonDivisor(N, M)}, x{N / g}, y{M / g};
	RF(i, 0, Q) {
		LL sx, sy, ex, ey;
		cin >> sx >> sy >> ex >> ey;
		LL secs{(sy - 1) / (sx == 1 ? x : y)}, sece{(ey - 1) / (ex == 1 ? x : y)};
		cout << (secs == sece ? "YES" : "NO") << '\n';
	}

	return 0;
}
