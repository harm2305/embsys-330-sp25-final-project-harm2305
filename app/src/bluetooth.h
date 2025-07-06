// Device name cannot be longer than 30 due to the size of a packet
#define BT_DATA_REQ_CLEAR 1
#define BT_DATA_REQ_GET 2
#define BT_DATA_REQ_PRINT 3
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
};

/**
 * @brief A struct representing a data request to the Bluetooth data
 * processing request.
 * 
 * The get_count property should be the number of observations to be retrieved
 * from the database. This property is not looked at when request_type
 * is BT_DATA_REQ_CLEAR.
 * 
 * The result property should be a pointer that will hold the result of
 * a BT_DATA_REQ_GET operation. This property will be unchanged in cases
 * of BT_DATA_REQ_CLEAR.
 */
struct bt_data_req {
    int request_type;
    int get_count;
    struct datum * result_list;
};


/**
 * @brief Returns the current scan mode.
 * 
 * @returns True if currently in active scan mode, false if passive.
 */
 bool get_current_scan_mode();

 /**
  * @brief Returns the count of advertising packets
  * scanned
  */
 uint8_t get_total_scans();