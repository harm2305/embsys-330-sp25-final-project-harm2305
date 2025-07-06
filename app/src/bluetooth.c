#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "bluetooth.h"
#include "bt_db.h"

LOG_MODULE_REGISTER(bt_scanner);

static bool extract_bt_device_name_cb(struct bt_data *data, void *user_data);
static void bt_process(void *, void *, void *);
static void bt_scan(void *, void *, void *);
static void bt_scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad);

K_EVENT_DEFINE(toggle_scan_type_event);
K_MSGQ_DEFINE(bt_process_msgq, sizeof(struct bt_scan_result), 16, 1);
K_MSGQ_DEFINE(bt_data_req_msgq, sizeof(struct bt_data_req), 16, 1);

K_THREAD_DEFINE(bt_scan_thread_tid, BT_SCAN_THREAD_STACK_SIZE, bt_scan, NULL, NULL, NULL, BT_SCAN_THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(bt_process_thread_tid, BT_PROCESS_THREAD_STACK_SIZE, bt_process, NULL, NULL, NULL, BT_PROCESS_THREAD_PRIORITY, 0, 0);

static struct datum *database = NULL;
static int enable_active = 1;
static uint8_t scan_count = 0;

/**
 * @brief Enables Bluetooth, initializes callbacks, and begins scanning for devices.
 * Scans will start in active mode and will update as needed.
 */
static void bt_scan(void *, void *, void *) {
    LOG_DBG("Beginning intialization of Bluetooth...");
	int err = bt_enable(NULL);

	if (err) {
		LOG_ERR("Unable to initialize Bluetooth (error %d)", err);
		return;
	}

	LOG_DBG("Successfully initialized Bluetooth");

	struct bt_le_scan_param bt_scan_params = {
		.type = BT_LE_SCAN_TYPE_ACTIVE,
		.options = BT_LE_SCAN_OPT_FILTER_DUPLICATE,
		.interval = BT_GAP_SCAN_SLOW_INTERVAL_2,
		.window = BT_GAP_SCAN_FAST_WINDOW
	};

	err = bt_le_scan_start(&bt_scan_params, bt_scan_cb);

    while(1) {
        if (k_event_wait(&toggle_scan_type_event, 1, true, K_FOREVER)) {
            enable_active = !enable_active;

            if (enable_active) {
                LOG_DBG("Received request to set BT scan type to active");
                bt_scan_params.type = BT_LE_SCAN_TYPE_ACTIVE;
            } else {
                LOG_INF("Received request to set BT scan type to passive");
                bt_scan_params.type = BT_LE_SCAN_TYPE_PASSIVE;
            }

            LOG_DBG("Restarting Bluetooth scanning due to scan type change");

            // Restart scan with new parameters
            err = bt_le_scan_stop();

            if (err) {
                LOG_ERR("Error %d: Failed to stop Bluetooth scanning", err);
                continue;
            }

            err = bt_le_scan_start(&bt_scan_params, bt_scan_cb);

            if (err) {
                LOG_ERR("Error %d: Failed to start Bluetooth scanning", err);
            }

            LOG_DBG("Successfully restarted Bluetooth scanning with new scan type");
        }
    }
}

/**
 * Callback that processes a Bluetooth scan result
 * and submits to a queue for processing later in a separate thread.
 */
static void bt_scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad) {
    scan_count++;
	char addr_str[BT_ADDR_LE_STR_LEN];
	char name[BT_MAX_DEVICE_NAME_LEN] = {0};

	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	bt_data_parse(ad, extract_bt_device_name_cb, name);

	struct bt_scan_result scan_result;
	
	scan_result.addr_str = addr_str;
	scan_result.rssi = rssi;
	scan_result.type = type;
	scan_result.ad = ad;

    strncpy(scan_result.device_name, name, BT_MAX_DEVICE_NAME_LEN - 1);

	LOG_DBG("Queuing an item to the Bluetooth item queue");

	int ret = k_msgq_put(&bt_process_msgq, &scan_result, K_FOREVER);

    if (ret) {
        LOG_ERR("Error %d: Failed to queue a scan result for processing later", ret);
    }
}

/**
 * @brief Processes all data operations for Bluetooth scan packets. This thread
 * will handle incoming data, data retrieval requests, and data clearing requests.
 * 
 * It is important to note that insertion of new data will always occur before a
 * data request is processed.
 */
static void bt_process(void *, void *, void *) {
    extern struct k_event data_print_rdy_event;

    while (1) {
        struct bt_scan_result scan_result;
        struct bt_data_req data_request;

        if (k_msgq_get(&bt_process_msgq, &scan_result, K_NO_WAIT) == 0) {
            LOG_INF("Device found: %s with address %s (RSSI %d), type %u, AD data len %u",
	           scan_result.device_name, scan_result.addr_str, scan_result.rssi, scan_result.type, scan_result.ad->len);

            struct bt_scan_obsv scan_observation = {
                .addr = scan_result.addr_str,
                .rssi = scan_result.rssi
            };

            strncpy(scan_observation.device_name, scan_result.device_name, BT_MAX_DEVICE_NAME_LEN);

            upsert(&database, &scan_observation);
        }

        if (k_msgq_get(&bt_data_req_msgq, &data_request, K_NO_WAIT) == 0) {
            if (data_request.request_type == BT_DATA_REQ_GET || data_request.request_type == BT_DATA_REQ_PRINT){
                data_request.result_list = get(database, data_request.get_count);
            } else if (data_request.request_type == BT_DATA_REQ_CLEAR) {
                clear(&database);
            }

            if (data_request.request_type == BT_DATA_REQ_PRINT) {
                k_event_set(&data_print_rdy_event, 1);
            }
        };

        k_msleep(100);
        
    }
}

/**
 * Processes the Bluetooth data buffer of a scan and
 * retrieves the name, if it is known.
 */
static bool extract_bt_device_name_cb(struct bt_data *data, void *user_data) {
    char *name = user_data;

    if (data->type != BT_DATA_NAME_COMPLETE && data->type != BT_DATA_NAME_SHORTENED) {
        strncpy(name, "unknown", BT_MAX_DEVICE_NAME_LEN - 1);
        return false;
    }

    size_t len = MIN(data->data_len, BT_MAX_DEVICE_NAME_LEN - 1);
    strncpy(name, data->data, len);

    return true;
}

/**
 * Returns the current scan mode. Can only ever either
 * BT_LE_SCAN_ACTIVE or BT_LE_SCAN_PASSIVE.
 */
bool get_current_scan_mode() {
    if (enable_active) {
        return true;
    }
    
    return false;
}


uint8_t get_total_scans() {
    return scan_count;
}