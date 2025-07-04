struct datum {
    struct bt_scan_obsv *data;
    struct datum *next;
};


/**
 * @brief Returns the length of the list
 */
int len(struct datum *head);

/**
 * @brief Appends a new datum node to the end of a list. The list is
 * sorted after every append operation to ensure observations with the
 * lowest RSSI values are at the front.
 * 
 * @param head Reference to the head of the list
 * @param data Data to be included in the node.
 */
void append(struct datum **head, struct bt_scan_obsv *data);

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
