# Arduino Clock

## Useful Commands

### Flash an Arduino Nano
```
make && avrdude -P /dev/ttyUSB0 -c arduino -p m328p -U flash:w:main.elf
```

### Connect USB serial console
```
picocom -b9600 /dev/ttyUSB0
```
