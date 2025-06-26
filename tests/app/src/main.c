/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <zephyr/ztest.h>
#include <counter.h>

/**
 * @brief Test Fixture Structure
 */
struct embsys330_fp_tests_fixture {
	uint32_t foo;
};

/**
 * @brief Setup at the beginning of the test suite.
 */
static void *embsys330_fp_tests_setup(void)
{
	struct embsys330_fp_tests_fixture *fixture =
		malloc(sizeof(struct embsys330_fp_tests_fixture));
	return fixture;
}

/**
 * @brief Called before the beginning of each test in the test suite.
 */
static void embsys330_fp_tests_before(void *f)
{
	struct embsys330_fp_tests_fixture *fixture = (struct embsys330_fp_tests_fixture *)f;
	fixture->foo = 0xdeadbeef;
}

/**
 * @brief Called before the beginning of each test in the test suite.
 */
static void embsys330_fp_tests_after(void *f)
{
	/* Add your own after code here if required */
}

/**
 * @brief Teardown at end of test suite.
 */
static void embsys330_fp_tests_teardown(void *f)
{
	free(f);
}

ZTEST_SUITE(embsys330_fp_tests, NULL, embsys330_fp_tests_setup, embsys330_fp_tests_before,
	    embsys330_fp_tests_after, embsys330_fp_tests_teardown);

/**
 * vTest Asserts
 *
 * This test verifies various assert macros provided by ztest.
 *
 */
ZTEST(embsys330_fp_tests, test_assert)
{
	zassert_true(1, "1 was false");
	zassert_false(0, "0 was true");
	zassert_is_null(NULL, "NULL was not NULL");
	zassert_not_null("foo", "\"foo\" was NULL");
	zassert_equal(1, 1, "1 was not equal to 1");
	zassert_equal_ptr(NULL, NULL, "NULL was not equal to NULL");
}

/**
 * @brief Test that the test fixture initialized correctly.
 */
ZTEST_F(embsys330_fp_tests, test_foo)
{
	zassert_equal(0xdeadbeef, fixture->foo, "Where's the beef?");
	fixture->foo = 0;
	zassert_equal(0, fixture->foo, "foo is invariant");
}

/**
 * @brief Test that embsys330_fp_tests_before reinitializes the fixture.
 */
ZTEST_F(embsys330_fp_tests, test_foo_reset)
{
	zassert_equal(0xdeadbeef, fixture->foo, "foo was not reset");
	fixture->foo = 0; /* Just in case the tests execute out of order */
}
/**
 * @brief Test Counter
 *
 * Tests the counter subsystem: reset and increment.
 *
 */
ZTEST(embsys330_fp_tests, test_counter)
{
	counter_reset();
	zassert_equal(0, counter_value());

	counter_increment();
	zassert_equal(1, counter_value());

	counter_increment();
	zassert_equal(2, counter_value());

	counter_reset();
	zassert_equal(0, counter_value());
}
