# System Interaction
The system composes of 3 main pieces:
- Observation collection
- Display
- CLI

The central part is the observation collection subsystem which performs scans and processes them. It consists
of two threads: a scanning thread and a data processing thread.

The CLI and display subsystems interface with this observaiton collection subsystem to provide data to
the end user.

# Observation Collection
Zephyr provides a robust Bluetooth stack with support for Bluetooth 4.1. For the operation
of our device, we will be utilizing the observer GAP profile which is a low-energy
connection-less Bluetooth role. 

## Scanning
Scanning and processing scans are done in their own thread. This will allow for data operations
to not be blocked and ensure that the LCD is up to date.

### Configuration
At thread startup, the Bluetooth stack is enabled via `bt_enable()`. Once that is successful,
configuration for scanning is simple done by definining a `struct bt_le_scan_param` and
assigning each member of the struct the appropriate value.

The `type` value of the scan parameters indicates why type of scanning to do. The system starts
with that value set to `BT_LE_SCAN_TYPE_ACTIVE` and is then toggled via a user action button
described below. A `k_event` is configured and awaited during thread operation.
When a new event is received, the scan type is toggled between active or passive.
Then, scanning is restarted to load in the new configuration.

In order to enable events, the following Kconfig option must be defined:
```
CONFIG_EVENTS=y
```

The `interval` and `window` values of the scan parameters describe how often and how big of window
to collect scans for. To limit CPU usage, the interval is set to `BT_GAP_SCAN_SLOW_INTERVAL_2`
which indicates to scan every 2.56 seconds. The window is set to `BT_GAP_SCAN_FAST_WINDOW`
meaning that that the window is 30ms large.

The final member of the scan parameters is the `option` field which we have set to
`BT_LE_SCAN_OPT_FILTER_DUPLICATE` to avoid duplicate scan parameters.

### Scan Processing

When new scans are received, a callback is ran that extracts the device name from the
packet, if present, and posts a message to a message queue defined statically by
`K_MSGQ_DEFINE(bt_process_msgq, sizeof(struct bt_scan_result), 16, 1)`. This message
queue is processed by a separate data processing thread described below to process
observations and load them into the database.
 
### Button Configuration
The user action button is configured on GPIO port C pin 13 in a active low configuration. We can register
this with an ISR. On a button press, a simple event is sent using Zephyr events via `k_event_set` which
is ready by this scanning thread.

### Process Flow
The scan initialization and configuration is described by the following process flow:

![Scan Thread Process Flow](images/scan_thread_process.png)

## Data Processing
Data processing occurs in its own thread. The data processing waits on the message queue mentioned above
defined as `K_MSGQ_DEFINE(bt_process_msgq, sizeof(struct bt_scan_result), 16, 1)`. When a new message
is received, an upsert operation is executed to load the data into the in-memory database backed by a
singly linked list.

### Linked List
A very basic linked list is defined in `bt_db.h` and `bt_db.c`. The nodes on the linked list are
defined on the default system heap whose size is configured via the follwoing Kconfig option:
```
CONFIG_HEAP_MEM_POOL_SIZE=4096
```

The linked list has an invariant mandating that it is sorted by the RSSI value of each Bluetooth scan
observation. To achieve this, we use a merge sort operation which recursively sorts the list. The
choice of a merge sort was driven by needing to sort quickly at the expense of CPU cycles. Since we do
not have a computationally expensive system, we can afford the hit on CPU computations.

Each node in the linked list is defined by `struct datum` which contains a pointer to a scan
observation and a pointer to the next element in the list.

#### Supported Functionality
The public API of the linked list supports the following operations:
```C
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
```

The two important functions are `void upsert(struct datum **head, struct bt_scan_obsv *data)`
and `struct datum * get(struct datum *head, int n)`.

The `upsert` operation will insert new nodes if they do not exist in the list already. If a node does exist,
its RSSI value is updated instead of adding a new node. The predicate for checking if a node exists
is simply comparing device addresses since those should be sufficiently unique.

The `get` operation will retrieve a certain number of observations from the database. This is useful
both for the LCD display and the USART terminal which will issue a command to retrieve a certian number
of observations. If the provided value exceeds the number of observations in the database, then all
of the observations are returned. If a value is `<= 0`, then no observations are returned.

For the get operation, the pointer returned points to a new list that is a copy from the database. This is
important to avoid a client accidentally modifying any data or breaking internal tracking.

# LCD Display
The LCD display provides a readonly, no interaction, view of the current system state. The following
data points are displayed on the screen:
- Current scan mode
- Number of packets scanned
- Number of unique packets scanned
- 5 strongest packets scanned

The main thread is responsible for retrieving data and displaying it on the LCD screen. Current scan mode
is obtained via the `bool get_current_scan_mode()` function defined in `bluetooth.h`. A value of
`true` indicates that the current scan mode is active and `false` indicates passive. This value is then
displayed on the status bar at the top of the screen.

Observations are obtained through the `struct datum * get(struct datum *head, int n)` with `n` set to
a value of 5. These observations are then displayed below the status bar as rows in a table. Since
the in-memory database is sorted, the observations we receive will be the `n` strongest scan
packets.

After all data is collected, the state of the LCD screen is updated and then the thread waits for
10ms before refreshing. This will allow for us to meet the minimum update latency of 500ms described
in the [requirements](software_requirements) document.

## Process Flow Diagram
The following diagram details the superloop behavior of the main thread:

![Main superloop process flow diagram](images/main_superloop.png)

# CLI
The CLI will provide two commands part of a command set:
- `db get {n}`
- `db clear`

All commands are issued via USART terminal

## `db get {n}`
Retrieves `n` observations from the database. Will return an error if a value `< 1` is provided. This
command will output the collected observations in a CSV format to the terminal for easy viewing.

## `db clear`
The DB clear command will wipe the contents of the in-memory database as well as any tracked metrics
such as total scans and unique devices.