rpi_arduino_ir_remote
=====================



setup for ir_send.ino
---------------------

`ir_send.ino` reads serial port for IR signal data and transmit it via LED.

* connect IR LED to digital port 4.
* Upload ir_send.ino.
* open your serial terminal. baud rate 115200.
* type following samples.


### input data format

* L sends leader.
* T sends trailer.
* [0-9a-fA-F] sends 4bit signal. LSB will be sent first. So if you want to send a byte `e1` you type `1e` and it will be 10000111.


### samples

turn on cooler 26 degree
```
L 10 01 0 0 04 fb ff 00 cc 33 0a f5 31 ce 86 79 00 ff 00 ff 00 ff 00 ff 00 ff 35 ca 1f e0 00 ff 00 ff 00 ff 90 6f T
```


