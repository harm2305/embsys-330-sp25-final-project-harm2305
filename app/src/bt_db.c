#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>

#include "bluetooth.h"
#include "bt_db.h"

static void merge_sort(struct datum **head);
static void split_list(struct datum *source, struct datum **front, struct datum **back);
static struct datum * sorted_merge(struct datum *a, struct datum *b);

void upsert(struct datum **head, struct bt_scan_obsv *data) {
    struct datum *new_node = k_malloc(sizeof(struct datum));

    new_node->data = data;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    struct datum *temp = *head;
    while(temp->next != NULL) {
        char *temp_dev_name = temp->data->device_name;

        // There is an existing node with a matching device name, update RSSI
        if (strcmp(temp_dev_name, data->device_name) == 0) {
            temp->data->rssi = data->rssi;
            merge_sort(head);
            return;
        }

        temp = temp->next;
    };

    temp->next = new_node; 

    merge_sort(head);
}

int len(struct datum *head) {
    int length = 0;

    while (head != NULL) {
        length++;
        head = head->next;
    }

    return length;
}

/**
 * @brief Performs a merge sort on the list of Bluetooth scan observations.
 * Observations are sorted by their RSSI values, with the strongest (lowest value) being at the
 * start of the list.
 * 
 * This sort occurs in place and will modify the original list.
 * 
 * @param head_ref Reference to the start of the list
 */
static void merge_sort(struct datum **head_ref) {
    struct datum *head = *head_ref;

    // Exit early if list is empty or of size 1
    if (!head || !head->next) {
        return;
    }

    struct datum *a;
    struct datum *b;

    split_list(head, &a, &b);

    merge_sort(&a);
    merge_sort(&b);

    *head_ref = sorted_merge(a, b);
}

/**
 * @brief Utility function to split the list in half for a merge sort.
 * 
 * @param source Source list to be split
 * @param front Reference that will hold the first half of the split list
 * @param back Reference that will hold the second half of the split list
 */
static void split_list(struct datum *source, struct datum **front, struct datum **back) {
    // Use two-pointer approach
    struct datum *slow = source;
    struct datum *fast = source->next;

    while (fast) {
        fast = fast->next;

        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

/**
 * @brief Merges two sorted lists based on their RSSI values. Lower RSSI
 * values indicate higher signal strength are placed first.
 * 
 * @param a Reference to first list
 * @param b Reference to second list
 * 
 * @return Reference to merged, sorted list
 */
static struct datum * sorted_merge(struct datum *a, struct datum *b) {
    if (!a) return b;
    if (!b) return a;

    struct datum *result = NULL;

    if (a->data->rssi >= b->data->rssi) {
        result = a;
        result->next = sorted_merge(a->next, b);
    } else {
        result = b;
        result->next = sorted_merge(a, b->next);
    }

    return result;
}

struct datum * get(struct datum *head, int n) {
    if (n < 0) {
        n = 0;
    }

    int num_nodes_get = MIN(n, len(head));

    struct datum *temp = head;
    struct datum *result = NULL;

    for (int i = 0; i < num_nodes_get; i++) {
        struct bt_scan_obsv *bt_data = temp->data;
        upsert(&result, bt_data);

        temp = temp->next;
    }

    return result;
}

void clear(struct datum **head) {
    struct datum *current = *head;
    struct datum *next;

    while (current != NULL) {
        next = current->next;
        k_free(current);
        current=next;
    }

    *head = NULL;
}