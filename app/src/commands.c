#include <stdlib.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>

#include "bluetooth.h"
#include "bt_db.h"

extern struct k_msgq bt_data_req_msgq;

// This event determines when data is ready to be printed out to the console
K_EVENT_DEFINE(data_print_rdy_event);

/**
 * Handles the get shell command, printing out an error if 
 * the requested number of observations is invalid
 */
static int cmd_get(const struct shell *sh, size_t argc, char** argv)
{
    int count = atoi(argv[1]);

    if (count <= 0) {
        shell_error(sh, "Requested number of records can be <= 0");
        return 1;
    }

    struct datum *results = k_malloc(sizeof(struct datum));
    struct bt_data_req request = {
        .request_type = BT_DATA_REQ_PRINT,
        .get_count = count,
        .result_list = results
    };

    k_msgq_put(&bt_data_req_msgq, &request, K_FOREVER);
    k_event_wait(&data_print_rdy_event, 1, true, K_FOREVER);

    while (results != NULL) {
        struct bt_scan_obsv *observation = results->data;
        shell_print(sh, "%s, %s, %d", observation->addr, observation->device_name, observation->rssi);

        results = results->next;
    }

    return 0;
}

/**
 * Handles the clear shell command
 */
static int cmd_clear(const struct shell *sh, size_t argc, char** argv) {
    struct bt_data_req request = {
        .request_type = BT_DATA_REQ_CLEAR
    };

    k_msgq_put(&bt_data_req_msgq, &request, K_FOREVER);

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
    sub_db,
    SHELL_CMD_ARG(get, NULL, "Retrieve a number of observations from the database", cmd_get, 2, 0),
    SHELL_CMD_ARG(clear, NULL, "Clear all observations from the database", cmd_clear, 1, 0)
);

SHELL_CMD_REGISTER(database, &sub_db, "Database commands", NULL);