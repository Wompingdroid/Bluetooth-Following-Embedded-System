# Hardware

This was done using an 'ESP32-D0WD-V3 (revision v3.1)' chip. This contains wifi
and bluetooth on the same board. With this we can create/track ibeacons. In
theroy *any* board with bluetooth should be supported. To identify the board
use this command to install the tool: `pip install esptool` and then you can
run this command replacing PORT with the port your device is connected to:
`esptool -p PORT chip-id`



# Setup

setup virtual env and install dependencies

## Linux (Debian)

`python3 -m venv venv`
`source venv/bin/activate`
`pip3 install -r ibeacon_requirements.txt`


# Dev notes

Setup project using esp32 board. First created ibeacon directory

`pio init --ide=emacs --board=esp32dev`


## Resources used

(https://medium.com/engineering-iot/ble-asset-tracking-with-esp32-a-comprehensive-guide-2a3f439b45c4) [tracking ibeacons]

Chatgpt for debugging errors
