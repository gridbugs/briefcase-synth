BOARD = arduino:avr:uno
SERIAL_PORT = /dev/ttyUSB0
BAUDRATE = 9600

compile:
	arduino-cli compile --fqbn $(BOARD) .

upload:
	arduino-cli upload --fqbn $(BOARD) --port $(SERIAL_PORT) .

connect:
	picocom --baud $(BAUDRATE) $(SERIAL_PORT)

.PHONY: compile upload connect
