/**
 * bt_db represents an internal database backed by a singly linked
 * list. Each node represents a particular reading from a Bluetooth advertising
 * packet. 
 * 
 * The database is sorted by RSSI values, with lowest appearing at the start of the
 * list. Lower RSSI values correlate to higher signal strength.
 */

struct datum {
    struct bt_scan_obsv *data;
    struct datum *next;
};


/**
 * @brief Returns the length of the list
 */
int len(struct datum *head);

/**
 * @brief Upserts a new datum node into the list. If the device does not
 * exist in the list, it is added to the list. If it does exist, its
 * RSSI value is updated. Devices are uniquely identified by their address.
 * 
 * The list is sorted after every upsert operation to ensure observations with
 * the lowest RSSI values are at the front.
 * 
 * @param head Reference to the head of the list
 * @param data Data to be included in the node.
 */
void upsert(struct datum **head, struct bt_scan_obsv *data);

/**
 * @brief Retrieves upto a certain number of nodes from the list. If the
 * requested number exceeds the length of the list, only the number of nodes
 * match the length are returned.
 * 
 * @param head Reference to the start of the list
 * @param n Number of elements requested
 * 
 * @return A reference to a new list node. This new list is separate from the list,
 * holding no reference to entire list. This is to avoid accidental data manipulation.
 */
struct datum * get(struct datum *head, int n);

/**
 * @brief Clears all data stored in the in-memory database list.
 * 
 * @param head Pointer to a pointer to start of the list
 */
void clear(struct datum **head);