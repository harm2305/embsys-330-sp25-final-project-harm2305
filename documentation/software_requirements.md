# Requirements

## Architectural
- Use of an LCD shield to provide a user interface
- Use of an external SD card for peristent data storage
- Limited use of external sensors, use what is available on the board to limit costs

## Functional Requirements
- In a default state, the device must show the closest Bluetooth beacons
    - The number of devices must be configurable throug the CLI, with an application defined limit
- Device must be able to etner a "sleep" state via a CLI command
- Device must be woken up from a "sleep" state by pressing a user action button
- The product must allow the retrieval of previous observations and display on LCD screen
- The product must llow configuration of device via a CLI through various commands with the minimum
functionality:
    - Clearing of previous observations
    - Enter sleep state
    - Deletion of previously stored observations
    - Searching of Bluetooth beacons via an identifier
- In an awake state, a user configurable amount of observations must be recalled with a user action button
    - There is an upper limit to the user configurable amount of observations (to avoid infinite paging)
- Repeat devices detected should not result in repeat observations
- Device configuration should persist across boots
- In a recall state, repeat button presses should page through more observations
- In a recall state, after all observations have been viewed, the device must return to 
the default observation state

## Non-functional Requirements
- In a default state, the device must 5 closest Bluetooth beacons unless otherwise specified by 
user configuration
- Must guarantee that at least 1000 observations persist across device boots
- Data access should not block collection observations
    - Reading/writing data should not block the system preventing new observations
- LCD screen should be updated with new data in a near-real time fashion, with at most 500ms latency
- Proximity of devices must be  measured via [RSSI (received signal strength indicator)](https://en.wikipedia.org/wiki/Received_signal_strength_indicator) values
- The upper limit of displayable observations should be 20
- In either default or recall state, a page must show at most 5 entries


## Regulatory Compliance
- The use of wireless communication implies compliance with the FCC
    - As described in FCC part 15 rules:
        - Device must not cause harmful interference
        - Devise must accept harmful interference received, even if it results in malfunction