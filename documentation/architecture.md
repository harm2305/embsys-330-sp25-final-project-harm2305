# Product Architecture

This product consists of several key components:
- The observation collection subsystem
- The configuration subsystem
- The data storage subsystem

These components work together to provide a device that meets the defined [requirements](software_requirements.md).

# The Observation Collection Subsystem

# The Configuration Subsystem
The configuration subsystem stores user defined configuration of the device. These configurations 
are defined and changed via a CLI exposed througha serial terminal. The CLI configuration allows for the following:
- Clearing the persistent storage
- Configuring the amount of observations to show on recall
- Put the device into sleep mode, disabling observation collection and data retrieval

## CLI Command Sets
The configuration subsystem should provide the following root commands:
- mem
- recall
- sys

### mem
The `mem` command provides an interface to manage the persistent data storage of the device. This command
provides the following feature set:
- Clearing all stored observations
- Altering the maximum number of stored observations
- Searching for device information based on an identifier
    - The output of this command will provide more information than what can be shown the LCD screen

### recall
The `recall` root command will control the configuration of how Bluetooth beacons are displayed on the
LCD screen. At a minimum, this command will provide the following functionality:
- Configuration of how many "closest" beacons are shown on the main screen
- Change the number of beacons recalled from memory in recall state, with an upper limit of 20

### sys
`sys` alters the state of the device. This command should provide a way to make the device enter a low
power state.

# Data Storage Subsystem
The data storage subsystem is responsible for persistent storage of data between boots. A user configurable
number of observations (default is 1000) is stored within a SQLite database that is backed physically
by an SD card.