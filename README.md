# ritw_2d

Re-Invent The Wheel, a wanna be 2d game engine designed for microcontrollers. Currently using `pi pico` as standard MCU, can be simply ported to other plattforms.

# Setup
If you were on Windows, just use the Raspberry Pi Pico Extention. On Linux, you get more options.

## Raspberry Pi Pico Extention
This is probably the easiest way to setup the developement enviroment for this project. Simply install the corresponding extention in your VSCode-based editor and import the project.

## Custom Editor
The following packages are debian based, if you were using another distribution, remember to adapt the corresponding package name.
### sdk Dependencies
```bash
cmake gcc-arm-none-eabi gcc g++
```
### openocd Dependencies
```bash
gdb-multiarch automake autoconf build-essential texinfo libtool libftdi-dev libusb-1.0-0-dev
```
### pico-sdk
