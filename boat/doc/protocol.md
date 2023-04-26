# control protocol

communication is unidirectional (from computer to boat).
each package consists of 3 bytes:
1. `servo_pos` (0-127 that's mapped to +-60 deg)
2. `engine_speed` (0-64, that's mapped to 0-100% PWM)  + `engine_dir` (1 bit: 1==F, 0=B)
3. `checksum` (XOR of previous 2 bytes)


## stream (re)synchronization
bytes 1 and 2 always have MSb == 0.
byte 3 always has MSB == 1.

## servo position
servo position is encoded in 7, lower bits of a byte.

## engine speed
engine speed is encoded using 6, lower bits of a byte.
7th bit is a direction bit:
* 0 == backward
* 1 == forward

## encoding
data is 5V UART, sent at 1200 bps (AM transmitter and receiver have ~2.5 KHz bandwidth).
