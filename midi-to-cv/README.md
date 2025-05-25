## Setup

Install toolchain:
```
$ arduino-cli core install arduino:avr
```

## Install midi library

```
arduino-cli lib install "MIDI Library"
```

Add your unix user to the `dialout` group for non-root access to the serial
port.

## Common Commands

### List Connected Boards

```
$ arduino-cli board list
```

### Compile a Sketch

```
$ make compile
```

### Upload Sketch

```
$ make upload
```

### Connect a terminal to the serial port

Press Ctrl-a, Ctrl-x to exit.

```
$ make connect
```
