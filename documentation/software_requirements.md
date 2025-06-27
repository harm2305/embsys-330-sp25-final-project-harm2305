# Requirements

## Architectural
- Use of an LCD shield to provide a user interface
- Use of an external SD card for peristent data storage
- Limited use of external sensors, use what is available on the board to limit costs

## Functional Requirements
- Allow retrieval of observations and display on LCD screen
- Allow configuration of device via a CLI

## Non-functional Requirements
- Must guarantee that 1000 observations persist across device boots
- Data access should not block collection observations
    - Reading/writing data should not block the system preventing new observations
- LCD screen should be updated with new data in a near-real time fashion, with at most 500ms latency

## Regulatory Compliance
- The use of wireless communication invites compliance with the FCC
    - As described in FCC part 15 rules:
        - Device must not cause harmful interference
        - Devise must accept harmful interference received, even if it results in malfunction