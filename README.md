# styrofoam boat

toy boat project, made out of styrofoam sheets and 3D-printed parts.
some random electronics added on top, to control it from the computer remotely.

note - it's mostly a junk-bin build.
you'll most likely not want to reproduce it 1:1, but adjust to what you have, instead.


## LoRa modules

in the design `E32-433T20D` LoRa modules are used.
it's powered with 5V, but logic operates at 3.3V and is NOT 5V tolerant.

SW assumes modules are operating in 115200 bps 8N1 mode.
this is not the default mode, though.
in order to program these, `M0` and `M1` ports must be pulled high (3.3V).
then special, binary sequence must be sent, namely:

> C000003C2E00

it's represented in hex.
value must be sent in binary, using 9600bps 8N1 mode.
this will write the value and persist it.
from now on each module will send data wirelessly using 19200bps 8N1.
user-facing interface will accept data to be sent using 115200bps 8N1.
both modules shall be programmed the same way.

the easiest way to interface with these are prototyping boards and USB-to-serial interfaces.
note that the interface must operate at 3.3V.

if other operational parameters are needed, command for programming different work modes must be constructed.
to help with this see source code under: `common/LoRa/Ebyte`.
it allows to specify required settings and after compiling prints out given settings.


## final thoughts / lessons learned

### good

* LoRa modules worked like a charm (despite "guaranteed" latency)
* sealing worked flawlessly
* safety feature to stop engine and 0-out rudder in case of connectivity issues saved the boat countless times

### bad and ugly

* rudder blade was too shallow. making it go deeper would provide more authority over the boat
* screw was not deep enough - it should be fully submerged, couple cm under water
* rudder must be perfectly centered behind the screw - 3mm off is enough to make a bit difference wrt turning direction agility
* old, junk-bin analog cameras are really not worth the effort... :P
* screw should be as close to 0 deg angle wrt waterline as possible - otherwise it's just an energy loss
* make sure engine has enough power to drive the boat against the wind
* ship's side shall not be too high, as it then acts as a sale, even on a small wind

### misc notes

* have a small pontoon at hand - just in case (had couple of close calls)
* tall, rubber boots are helpful at the pond
* make sure to check transcievers range before learning it the hard way at the pond ;)
