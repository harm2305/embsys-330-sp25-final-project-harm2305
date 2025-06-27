# Project Idea
The idea of my project is to develop a Bluetooth beacon detector. The detector should be
configurable via CLI to control and allow for recall of previous entries.

A [Bluetooth beacon](https://en.wikipedia.org/wiki/Bluetooth_Low_Energy_beacon) is a low energy Bluetooth
broadcast that sends out a device's identifier to other nearby electronic devices. Notable uses
of Bluetooth beacons are:
- Identification of location based action such as social media check-in
- Indoor positioning

The goal of my project is to keep a listing of nearby devices that are emitting Bluetooth beacons. I will be
utilizing the following sensors on my board:
- SPBTLE-RF Bluetooth module for beacon detection
- SD card reader for persistent data storage

The LCD screen on my board will be used to show nearest Bluetooth beacons.

Additionally, I will utilize an SD card with a SQLite database to keep a more comprehensive listing
of scanned beacons which can be displayed on the LCD screen. A database hosted on an SD card will
allow for persistence of data between device boots.