from enum import Enum
import serial
import time

class Boat:
    def __init__(self, port):
        self._sp = serial.Serial(port, 9600)
        self._next_frame_after = 0.0

    # eng_speed = -100..+100 [%] (0 == stop)
    # servo_pos = -60..+60 [deg] (0 == center)
    def send(self, eng_speed, servo_pos, output=False):
        # avoid buffering - always send latest instead
        if self._sp.out_waiting > 0:
            return False
        # give time for LoRa module to send
        if time.time() < self.self._next_frame_after:
            return False
        # actual sending
        frame = bytearray()
        frame.append( self._encode_servo_pos(servo_pos) )
        frame.append( self._encode_speed(eng_speed) )
        frame.append( self._encode_checksum(frame) )
        self._sp.write(frame)
        if output:
            print("{:02x} {:02x} {:02x}".format(frame[0], frame[1], frame[2]))
        # must wait >20ms after each transmission, to make sure LoRa module sends out the data
        self._sp.flush()
        self.self._next_frame_after = time.time() + 0.025
        return True

    def _encode_speed(self, s):
        assert( -100 <= s and s <= +100 )
        dir_bit = 0x00 if s < 0 else 0x40
        s = abs(s)
        s = int(s/100 * 63)
        assert( 0 <= s and s <= 63 )
        return dir_bit | s

    def _encode_servo_pos(self, p):
        assert( -60 <= p and p <= +60 )
        p = -p
        p = p + 60
        p = int(p/(2*60) * 127)
        assert( 0 <= p and p <= 127 )
        return p

    def _encode_checksum(self, frame):
        x = frame[0] ^ frame[1]
        return x | 0x80

