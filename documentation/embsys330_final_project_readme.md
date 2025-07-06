# EMBSYS 330 Final Project

## Description of Program
My final project idea was to develop a Bluetooth beacon observor. It uses the Bluetooth stack
available on the Zephyr RTOS. The use of a connection-less profile allows for low-energy
observations of advertising packets send to the device.

## Instructions For Use Of The Program
Starting the device is simple as all you need to do is plug in the USB cable onto the board.

The user action button available on the board can be used to toggle between active and
passive scanning modes with the different scan qualities at the detriment of power
and CPU usage.

A terminal connection (via TeraTerm or other methods) will allow for an end user to interact
with the system via a shell. The two functionalities available via the shell are
retrieval of observations and clearing of the in-memory database.

The LCD screen is for display only and does not provide any interaction.