#include <iostream>
#include "gtest/gtest.h"

#include "OpenCLOPS.h"


// IndependentMethod is a test case - here, we have 2 tests for this 1 test case
/*TEST(IndependentMethod, ResetsToZero) {
	int i = 3;
	independentMethod(i);
	EXPECT_EQ(0, i);

	i = 12;
	independentMethod(i);
	EXPECT_EQ(0,i);
}

TEST(IndependentMethod, ResetsToZero2) {
	int i = 0;
	independentMethod(i);
	EXPECT_EQ(0, i);
}*/

// The fixture for testing class Project1. From google test primer.
class OpenCLOPSTest : public ::testing::Test {
protected:
	// You can remove any or all of the following functions if its body
	// is empty.

	OpenCLOPSTest() {
		// You can do set-up work for each test here.
	}

	virtual ~OpenCLOPSTest() {
		// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:
	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Project1.
	OpenCLOPS clop;
};

// Test case must be called the class above
// Also note: use TEST_F instead of TEST to access the test fixture (from google test primer)
TEST_F(OpenCLOPSTest, MethodBarDoesAbc) {
	const size_t N = 1 << 20;

	std::vector<double> a(N, 1);
	std::vector<double> b(N, 2);
	std::vector<double> c(N);

	c = clop.vec_add(a, b); // we have access to clops, declared in the fixture
	EXPECT_EQ(3, c[42]);
}

// }  // namespace - could surround Project1Test in a namespace
