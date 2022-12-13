# CANdapter

The CANdapter by Ewert Energy Systems is a USB to CAN bus converter that supports both standard and extended CAN messages. The computer side of the CANdapter connects via USB (with a USB type B connector) and the CAN bus side of the CANdapter connects via RS232 serial (although with a strange pinout).

The primary use of the CANdapter is to program Ewert's Orion BMS (which is why we have two of them) but it can be used to send any messages onto a CAN bus. The CANdapter does have drivers for Windows but they aren't very good, so it is easier to interact with the CANdapter using the virtual RS232 serial port it creates (this works for any platform).

**The full CANdapter manual can be found [here](https://www.ewertenergy.com/products/candapter/downloads/candapter_manual.pdf)**, although much of the information there is reproduced here anyway (and this is probably more useful if I do say so myself...)

## Caveats

Before working with the CANdapter it is important to note the following information.

1. The CANdapter pinout doesn't follow the conventions you might think given it is a serial port.
2. The manual says that the CANdapter is rated for a CAN bus up to 1 Mbps, but in our testing we managed to get it only as high as 500 Kbps.
3. When the CANdapter is connected it uses a FIFO queue to communicate with the computer. The consumer must actively poll the CANdapter to prevent the FIFO queue from filling up. Even if you do not want to read from the CAN bus, if the FIFO queue is full no messages can be transmitted.
4. The CANdapter software (available for Windows only) is not very good.

## Manually using the CANdapter

To manually use the CANdapter (that is to send messages to the connected CAN bus from the terminal, or to inspect the outputs) you can follow these instructions. These instructions are for Linux. The same instructions can be applied to any UNIX-like operating system (including macOS and BSD) and can be slightly modified to work on Windows.

Note that some commands depend on your user having the appropriate permissions to access the virtual serial port the CANdapter creates. On some systems, this is controlled by membership to the `dialout` or similar group but this varies per system.