/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <zephyr/ztest.h>
#include "bluetooth.h"
#include "bt_db.h"

/**
 * @brief Test Fixture Structure
 */
struct embsys330_fp_tests_fixture {
	struct datum *head;
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
	fixture->head = NULL;
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
ZTEST_F(embsys330_fp_tests, test_head)
{
	zassert_equal(NULL, fixture->head, "Head is not initialized to NULL");
}

/**
 * @brief Test that embsys330_fp_tests_before reinitializes the fixture.
 */
ZTEST_F(embsys330_fp_tests, test_head_reset)
{
	zassert_equal(NULL, fixture->head, "Fixture was not reset");
}

/**
 * @brief Tests the length of an empty list
 */
ZTEST_F(embsys330_fp_tests, test_empty_list) {
	struct datum *head = fixture->head;
	int length = len(head);

	zassert_equal(length, 0, "Length of an empty list is not 0");
}

/**
 * @brief Test insert behavior and verify length after
 */
ZTEST_F(embsys330_fp_tests, test_list_insert) {
	struct datum *head = fixture->head;

	struct bt_scan_obsv *node1 = malloc(sizeof(struct bt_scan_obsv));
	node1->addr = "addr1";
	node1->rssi = -10;
	strncpy(node1->device_name, "node1", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node2 = malloc(sizeof(struct bt_scan_obsv));
	node2->addr = "addr2";
	node2->rssi = -20;
	strncpy(node2->device_name, "node2", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node3 = malloc(sizeof(struct bt_scan_obsv));
	node3->addr = "addr3";
	node3->rssi = -30;
	strncpy(node3->device_name, "node3", BT_MAX_DEVICE_NAME_LEN);

	upsert(&head, node1);
	upsert(&head, node2);
	upsert(&head, node3);

	int length = len(head);

	zassert_equal(length, 3, "List of length %d is not 3 after 3 inserts", length);
}

/**
 * Insert elements in an unsorted order and the check to see if they are sorted.
 * Sort order is based on RSSI values, with the smallest being at the start of the
 * list.
 */
ZTEST_F(embsys330_fp_tests, test_list_sort) {
	struct datum *head = fixture->head;

	struct bt_scan_obsv *node1 = malloc(sizeof(struct bt_scan_obsv));
	node1->addr = "addr1";
	node1->rssi = -30;
	strncpy(node1->device_name, "node1", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node2 = malloc(sizeof(struct bt_scan_obsv));
	node2->addr = "addr2";
	node2->rssi = -10;
	strncpy(node2->device_name, "node2", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node3 = malloc(sizeof(struct bt_scan_obsv));
	node3->addr = "addr3";
	node3->rssi = -20;
	strncpy(node3->device_name, "node3", BT_MAX_DEVICE_NAME_LEN);

	upsert(&head, node1);
	upsert(&head, node2);
	upsert(&head, node3);

	int length = len(head);

	zassert_equal(length, 3, "List of length %d is not 3 after 3 inserts", length);

	struct datum *temp = head;
	zassert_equal(temp->data->rssi, -10, "First RSSI value of %d does not match expected -10", temp->data->rssi);

	temp = temp->next;
	zassert_equal(temp->data->rssi, -20, "Second RSSI value of %d does not match expected -20", temp->data->rssi);

	temp = temp->next;
	zassert_equal(temp->data->rssi, -30, "Third RSSI value of %d does not match expected -30", temp->data->rssi);
}

/**
 * Test the update behavior of a node
 */
ZTEST_F(embsys330_fp_tests, test_update) {
	struct datum *head = fixture->head;

	struct bt_scan_obsv *node1 = malloc(sizeof(struct bt_scan_obsv));
	node1->addr = "addr1";
	node1->rssi = -10;
	strncpy(node1->device_name, "node1", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node2 = malloc(sizeof(struct bt_scan_obsv));
	node2->addr = "addr2";
	node2->rssi = -20;
	strncpy(node2->device_name, "node2", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node3 = malloc(sizeof(struct bt_scan_obsv));
	node3->addr = "addr3";
	node3->rssi = -20;
	strncpy(node3->device_name, "node3", BT_MAX_DEVICE_NAME_LEN);

	upsert(&head, node1);
	upsert(&head, node2);
	upsert(&head, node3);

	zassert_equal(head->data->rssi, -10, "First RSSI value of %d does not match expected -10", head->data->rssi);

	node1->rssi = -50;
	upsert(&head, node1);

	int length = len(head);
	zassert_equal(length, 3, "Length of %d does not match expected length of 3", length);

	zassert_equal(head->data->rssi, -20, "First RSSI value of %d does not match expected -20 after upsert operation", head->data->rssi);
}

/**
 * Test the retrieval of 0 nodes from a list with 1 or more nodes
 */
ZTEST_F(embsys330_fp_tests, test_zero_retrieval) {
	struct datum *head = fixture->head;

	struct bt_scan_obsv *node1 = malloc(sizeof(struct bt_scan_obsv));
	node1->addr = "addr1";
	node1->rssi = -10;
	strncpy(node1->device_name, "node1", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node2 = malloc(sizeof(struct bt_scan_obsv));
	node2->addr = "addr2";
	node2->rssi = -20;
	strncpy(node2->device_name, "node2", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node3 = malloc(sizeof(struct bt_scan_obsv));
	node3->addr = "addr3";
	node3->rssi = -20;
	strncpy(node3->device_name, "node3", BT_MAX_DEVICE_NAME_LEN);

	upsert(&head, node1);
	upsert(&head, node2);
	upsert(&head, node3);

	struct datum *retrieval_list = get(head, 0);

	zassert_equal(retrieval_list, NULL, "Retrieval of 0 nodes returns address %p instead of NULL", (void *)retrieval_list);
}

/**
 * Test the retrieval of a non-zero number of nodes from a list with 1 or more nodes
 */
ZTEST_F(embsys330_fp_tests, test_nonzero_retrieval) {
	struct datum *head = fixture->head;

	struct bt_scan_obsv *node1 = malloc(sizeof(struct bt_scan_obsv));
	node1->addr = "addr1";
	node1->rssi = -10;
	strncpy(node1->device_name, "node1", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node2 = malloc(sizeof(struct bt_scan_obsv));
	node2->addr = "addr2";
	node2->rssi = -20;
	strncpy(node2->device_name, "node2", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node3 = malloc(sizeof(struct bt_scan_obsv));
	node3->addr = "addr3";
	node3->rssi = -20;
	strncpy(node3->device_name, "node3", BT_MAX_DEVICE_NAME_LEN);

	upsert(&head, node1);
	upsert(&head, node2);
	upsert(&head, node3);

	struct datum *retrieval_list = get(head, 2);

	zassert_equal(len(retrieval_list), 2, "Retrieval of 2 nodes returns list of length %d", len(retrieval_list));
}

/**
 * Test the retrieval of a more nodes than exist from a list with 1 or more nodes
 */
ZTEST_F(embsys330_fp_tests, test_excessive_retrieval) {
	struct datum *head = fixture->head;

	struct bt_scan_obsv *node1 = malloc(sizeof(struct bt_scan_obsv));
	node1->addr = "addr1";
	node1->rssi = -10;
	strncpy(node1->device_name, "node1", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node2 = malloc(sizeof(struct bt_scan_obsv));
	node2->addr = "addr2";
	node2->rssi = -20;
	strncpy(node2->device_name, "node2", BT_MAX_DEVICE_NAME_LEN);

	struct bt_scan_obsv *node3 = malloc(sizeof(struct bt_scan_obsv));
	node3->addr = "addr3";
	node3->rssi = -20;
	strncpy(node3->device_name, "node3", BT_MAX_DEVICE_NAME_LEN);

	upsert(&head, node1);
	upsert(&head, node2);
	upsert(&head, node3);

	struct datum *retrieval_list = get(head, 100);

	zassert_equal(len(head), 3, "Original list of length %d does not match expected length of 3", len(head));
	zassert_equal(len(retrieval_list), 3, "Retrieval of 100 nodes from a list of length 3 returns %d nodes, should return 3", len(retrieval_list));
}
