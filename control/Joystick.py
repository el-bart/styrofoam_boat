#!/usr/bin/env python3
import struct
import os

port = '/dev/ttyUSB0'
joystick_dev = '/dev/input/js0'
joystick_axis_engine = 0
joystick_axis_rudder = 1

class Device:
    def __init__(self, dev, axis_throttle, axis_rudder):
        self._js_dev = open(dev, 'rb')
        os.set_blocking(self._js_dev.fileno(), False)
        self._axis_throttle = axis_throttle
        self._axis_rudder = axis_rudder
        self._throttle = 0
        self._rudder = 0

    def throttle(self):
        return self._throttle

    def rudder(self):
        return self._rudder

    def update(self):
        ev = self._js_dev.read(8)
        if not ev:
            return
        js_time, js_value, js_type, js_number = struct.unpack('IhBB', ev)
        if js_type & 0x02:
            if js_number == self._axis_throttle:
                value = -int( ( js_value / 2**15 ) * 100 )
                self._throttle = self._map_throttle(value)
                return
            if js_number == self._axis_rudder:
                self._rudder = int( ( js_value / 2**15 ) * 60 )
                return

    def _map_throttle(self, v):
        abs_min_throttle = 75
        abs_max_throttle = 100
        delta = abs_max_throttle - abs_min_throttle
        if v == 0:
            return v
        sign = -1 if v < 0 else +1
        denom = abs_max_throttle / delta
        out_fp = ( sign * abs_min_throttle ) + ( v / denom )
        return int(out_fp)
