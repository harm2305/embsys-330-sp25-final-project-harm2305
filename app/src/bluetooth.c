#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "bluetooth.h"

LOG_MODULE_DECLARE(app);

static void process_scan_result(void *, void *, void *);

K_MSGQ_DEFINE(bt_msgq, sizeof(struct bt_scan_result), 16, 1);

K_THREAD_DEFINE(bt_scan_result_tid, BT_THREAD_STACK_SIZE, process_scan_result, NULL, NULL, NULL, BT_THREAD_PRIORITY, 0, 0);

K_THREAD_STACK_DEFINE(bt_thread_stack_area, BT_THREAD_STACK_SIZE);

struct k_work_q bt_work_q;

void initialize_bt_work_q() {
    k_work_queue_init(&bt_work_q);
    k_work_queue_start(&bt_work_q, bt_thread_stack_area, K_THREAD_STACK_SIZEOF(bt_thread_stack_area), BT_THREAD_PRIORITY, NULL);
};

static void process_scan_result(void *, void *, void *) {

    while (1) {
        struct bt_scan_result scan_result;

        if (k_msgq_get(&bt_msgq, &scan_result, K_NO_WAIT) == 0) {
            LOG_INF("Device found: %s (RSSI %d), type %u, AD data len %u\n",
	           scan_result.addr_str, scan_result.rssi, scan_result.type, scan_result.ad->len);
        }

        k_msleep(100);
    }
}