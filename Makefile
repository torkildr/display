ARDUINO_DIR  = /usr/share/arduino

TARGET       = display
ARDUINO_LIBS = Ethernet Ethernet/utility SPI

BOARD_TAG    = uno
ARDUINO_PORT = /dev/cu.usb*

include Arduino.mk
