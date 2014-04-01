AccessControlSystem
===================

About the project

A project focuses on providing a smart area control system.
The system allows to control accessing to particular doors in home, office and others.
The system consists of RFID card readers and control units(microcontrollers) that are linked 
together using CAN Bus.
The main control unit is built on STM32F4 Discovery board.
Remote unit that are placed next to doors, include Atmega 4313, NXP MFMC522 rfid card reader and
MCP 2515 CAN Controller with MCP 2551 CAN Transceiver.
Because the STM32 has built-in CAN Controller, it needs only a PHY like MCP2551.

Main control unit provides authentication services for slave units.
It's also a interface to communicate with users and supplies a configuration tools.


The project is still under development. This document will be updated.

---------------------

Actual issues and problems
There is a problem with voltage levels due to a STM is powered by 3,3V and MCP2551 requires 5V, but
STM has 5V tollerant pins, so that solved the problem.
But in case with Atmega, we need to apply a voltage level translator.
Atmega must to be powered by 5V(due to MCP2551), but rfid reader requires 3,3V.


