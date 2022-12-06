# Comms/Flatbufs

Flatbuffers enable easier (de)serialisation for the comms protocol. It needs to be serialised before it goes onto the CAN bus.

Flatbuffs docs can be found [here](https://google.github.io/flatbuffers/index.html) and Flatcc is used to create the C code, the GitHub for that is [here](https://github.com/dvidelabs/flatcc).

We are the VCU-Comms team so if you have any questions message us on Discord (Toby Godfrey and George Peppard) or find us at one of the meetings (we'll probably be playing with a CANdapter).

## Supported Commands

### Start Communication

A connection must be established with the VCU to being the process of reading and writing to the VCU. This means that the VCU cannot be reprogrammed while the Ready-to-Drive system is active. To open a connection, the `open` command must be sent. This must be done only once and before any other commands are sent.

This must be answered with `suc` to show that the VCU is ready and in the correct state.

### Write a Parameter Value

To write a new value to a VCU parameter, the `set <var> <val>` command is used. `<var>` must be replaced with the name of the variable (this can be found in the enum in [`schema.fbs`](schema.fbs)) and `<val>` must be replaced with the new value.

This should be acknowledged with either `suc` for a successful execution of the `set` command, or `err` if the `set` command failed to execute successfully.

### Read a Parameter Value

To retrieve a value from the VCU, the `get <var>` command must be used. `<var>` must be replaced with the name of the variable (this can be found in the enum in [`schema.fbs`](schema.fbs)).

Every `get` command should be answered with either `ret` or `err`. If the read is successful, `ret <val>` is returned where `<val>` is the current value of the requested variable. If the read fails, then `err` should be returned.

### Save the Current Configuration

The current VCU parameter configuration can be saved by moving them from RAM to EEPROM. This is done by sending the `save <config_name> <config_version>` command. `<config_name>` must be replaced with a name for the config (check max length) and `<config_version>` must be provided. This is stored as a `struct` of three 8 bit integers and so is of the form `X.X.X`.

This should be answered with `suc` or `err` respectively if the save is successful or if the save fails.

### End Communication

Once all desired reprogramming has been done, the connection must be closed. To do this the `close` command must be sent.

This does not require a response.
