// Device name cannot be longer than 30 due to the size of a packet
#define BT_MAX_DEVICE_NAME_LEN 30
#define BT_PROCESS_THREAD_STACK_SIZE 1024
#define BT_PROCESS_THREAD_PRIORITY 4
#define BT_SCAN_THREAD_STACK_SIZE 1024
#define BT_SCAN_THREAD_PRIORITY 5
#define MAX_SCAN_LABEL_SIZE 16

/**
 * @brief A struct representing Bluetooth advertisements from
 * the Bluetooth module
 */
struct bt_scan_result {
    char *addr_str;
    char device_name[BT_MAX_DEVICE_NAME_LEN];
    int8_t rssi;
    uint8_t type;
    struct net_buf_simple *ad;
};

/**
 * @brief A struct representing a singular Bluetooth observations.
 * 
 * surrogate_key should be a randomly generated uint8_t generated at
 * boot to function as a unqiue identifer for a series of observations.
 * 
 * surrogate_key and sys_tick_elapsed sufficiently identifies a unique slice
 * of time for an observation.
 */
struct bt_scan_obsv {
    char *addr;
    char device_name[BT_MAX_DEVICE_NAME_LEN];
    int8_t rssi;
    int64_t sys_tick_elapsed;
};


/**
 * @brief Returns the current scan mode.
 * 
 * @returns True if currently in active scan mode, false if passive.
 */
 bool get_current_scan_mode();