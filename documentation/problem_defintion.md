# Problem Definition

# Problem Statement
As everyday users, we have many devices around us ranging all sorts of sizes. From Amazon Echo devices to phones to wearables.
The average person can own upwards of 3 bluetooth devices that they use in their daily lives. The question then becomes,
how many bluetooth devices are around a user on average?

# Hardware Used
- SPBTLE-RF Bluetooth module
- LCD shield
- SD card reader
- Onboard user action button

# User stories
- As a user of the device, I want to see the top 3 latest Bluetooth beacons detected on the LCD screen

- As a user of the device, I want to be able to press a button to recall recent Bluetooth beacon entries

- As a user of the device, I want to be able to control the function of the device via CLI, so that I can
alter the output shown on the screen such as number of devices shown at any time

- As a user of the device, I want to be able to clear the database of devices via CLI, so that I can reduce
memory footprint and gather new readings as needed

# CLI
The device should be configurable via a serial terminal and alter basic functionality such as how many
devices are shown at the screen at one time and the maximum number of devices stored. Additionally,
the CLI should be used to clear out previously stored devices.

# SD Card & SQLite Database
The SD card will be used to host a [SQLite](https://sqlite.org/index.html) database that will allow
for persistent storage of data points. This will allow for storage of more data than can be held in
RAM as well as recall of data between device boots. An onboard button will be provided that will print
out a listing of most recent devices. The number of most recent devices will be configurable through the CLI.

The use of SQLite is intentional as it is a low footprint database that does not rely on a client & server
architecture. It is written in C and should integrate with our application fairly easily.