# Requirements

## Architectural
- Must use an LCD shield to provide a user interface
- Must use a Bluetooth sensor to gather beacon advertising packets
- Must use a USART terminal to interface with the device
- Must use an onboard user action button to toggle between scan types
- Must limit hardware to what is available on the board to reduce costs

## Functional Requirements
- Device should boot up in active scanning mode
- User button should toggle between active and passive scanning modes
    - Active consumes more power but allows for better scan readings
        - Active scan readings typically include device names whereas passive only includes
        address type
- Current scan mode should be visible on the LCD screen
- The product must allow the retrieval of previous observations and display on a USART terminal
- Repeat devices detected should not result in repeat observations
- LCD screen should display the closest Bluetooth beacons detected
- LCD screen readings should include address, device name (if available), and RSSI signal strength
- Must allow for clearing of the in memory database to reduce memory footprint

## Non-functional Requirements
- The 5 closest Bluetooth devices should be shown on the LCD screen
- Data access should not block collection observations
    - Reading/writing data should not block the system preventing new observations
- LCD screen should be updated with new data in a near-real time fashion, with at most 500ms latency
- Proximity of devices must be measured via [RSSI (received signal strength indicator)](https://en.wikipedia.org/wiki/Received_signal_strength_indicator) values

## Regulatory Compliance
- The use of wireless communication implies compliance with the FCC
    - As described in FCC part 15 rules:
        - Device must not cause harmful interference
        - Device must accept harmful interference received, even if it results in malfunction