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