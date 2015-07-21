#define RANGES_SUPPRESS_IOTA_WARNING	// warning that interface to iota (and ints) has changed.
#include "range/v3/core.hpp"
#include "range/v3/to_container.hpp"
#include "range/v3/view/remove_if.hpp"
#include "range/v3/view/reverse.hpp"
#include "range/v3/view/take.hpp"

#include "container_facade.hpp"

#include <cassert>
#include <vector>
using namespace std;
using namespace ranges;

class MyArray : public container_facade<MyArray> {
	public:
		static int const N = 6;
	
	private:
		int v_[N] = {0,1,2,3,4,5};

		friend struct ranges::range_access;
		
		template <bool isConst>
		struct cursor {
			using It = typename std::conditional<isConst, int const*, int*>::type;
			using Ref = typename std::conditional<isConst, int const&, int&>::type;

			cursor() = default;
			cursor(It it)
				:	it_(it) {
			}
			
			Ref current() const { return *it_; }
			bool equal(cursor<isConst> const& that) const { return it_ == that.it_; }
			void next() { ++it_; }
			void prev() { --it_; }
			std::ptrdiff_t distance_to(cursor<isConst> const &that) const { return that.it_ - it_; }
			void advance(std::ptrdiff_t n) { it_ += n; }
			
			private:
				It it_;
		};

	public:
		MyArray();

		cursor<true> begin_cursor() const { return {v_}; }
		cursor<true> end_cursor() const { return {v_ + N}; }
		cursor<false> begin_cursor() { return {v_}; }
		cursor<false> end_cursor() { return {v_ + N}; }
};

inline MyArray::MyArray() = default; // http://stackoverflow.com/questions/21900237/do-i-really-need-to-implement-user-provided-constructor-for-const-objects

void TestNonConstMyArray() {
	MyArray myArray;
	myArray.back() = 10;
	assert(myArray.size() == MyArray::N);
	assert(!myArray.empty());
	vector<int> const v = myArray | view::reverse | view::take(4) | to_vector;
	assert((v == vector<int>{10,4,3,2}));
}

void TestConstMyArray() {
	MyArray const myArray;
	assert(myArray.size() == MyArray::N);
	vector<int> v = myArray | view::remove_if([](int x) { return x == 3; }) | view::reverse | to_vector;
	assert((v == vector<int>{5,4,2,1,0}));
}

int main() {
	TestConstMyArray();
	TestNonConstMyArray();
}
