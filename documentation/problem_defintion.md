# Problem Definition

## Problem Statement
As everyday users, we have many devices around us ranging all sorts of sizes. From Amazon Echo devices to phones to wearables.
The average person can own upwards of 3 bluetooth devices that they use in their daily lives. The question then becomes,
how many bluetooth devices are around a user on average?

## Approach
The solve our problem, we will be building a product that will scan for Bluetooth advertising
packets, capture some device information, and then store that information in memory. Metrics
will be regularly pulled from the in memory "database" and shown to the end user.

## Hardware Used
- SPBTLE-RF Bluetooth module
- LCD shield
- Onboard user action button

## User stories
- As a user of the device, I want to see the latest Bluetooth beacons detected on the LCD screen

- As a user of the device, I want to be able to press a button to toggle between Bluetooth scan modes

- As a user of the device, I want to be able to recall a number of devices through a CLI interface, so
that I may view more data than what can appear on the LCD screen

- As a user of the device, I want to be able to clear the database of devices via CLI, so that I can reduce
memory footprint

## CLI
The CLI should provide the capability to retrieve Bluetooth readings and display them on screen. The end user
should pass in a number and that many readings should be returned, in order from strongest device strength
to weakest.

Additionally, the CLI should allow for the clearing of the in memory database.