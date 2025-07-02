#define BT_THREAD_STACK_SIZE 1024
#define BT_THREAD_PRIORITY 5

struct bt_scan_result {
    char *addr_str;
    int8_t rssi;
    uint8_t type;
    struct net_buf_simple *ad;
};