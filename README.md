rpi_arduino_ir_remote
=====================



setup for ir_send.ino
---------------------

`ir_send.ino` reads serial port for IR signal data and transmit it via LED.

* connect IR LED to digital port 4.
* Upload ir_send.ino.
* open your serial terminal. baud rate 115200.
* type following samples.


### input data command format

* `Xtnnn` set format. `t = [NAS]` NEC, AEHA, SONY respectively. nnn=duration for each bit(known as `T`) in microseconds (`t=A` only).
* `L` queues a leader to send.
* `T` queues a  trailer to send.
* `[0-9a-fA-F]` queues a 4bit signal to send. LSB will be sent first. So if you want to send a byte `e1` you type `1e` and it will be 10000111.
* `Z` to send queued items.
* spaces and returns between commands are ignored.


### samples

* turn on cooler 26 degree

```
XA425 L 10 01 0 0 04 fb ff 00 cc 33 0a f5 31 ce 86 79 00 ff 00 ff 00 ff 00 ff 00 ff 35 ca 1f e0 00 ff 00 ff 00 ff 90 6f T Z
```

* turn off

```
XA425 L 10 01 0 0 04 fb ff 00 cc 33 0a f5 31 ce 86 79 00 ff 00 ff 00 ff 00 ff 00 ff 35 ca 1e e1 00 ff 00 ff 00 ff 90 6f T Z
```


