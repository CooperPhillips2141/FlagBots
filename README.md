# FlagBots
Mines CS Field Session CSM-Bahar Group

# Setup
On a Raspberry Pi, `git pull` everything then setup a python virtual environment in that directory.
Install all required dependencies then run `PoseEstimation/main.py` to run the whole program (make sure
the Arduino is connected by usb). If the arduino does not connect, run `$ ls /dev/tty*` on the Raspberry
Pi and pick the one that looks something like `ttyACM0` or something without a number. Then replace the 
`USB_PORT` variable with that (in `main.py`). If that still doesn't work, make sure to give the user
usb port permissions.

On the Arduino, load the `Arduino/Arduino.ino` file (The `OriginalBots` code is what was originally
coded onto the robots for the art installation.) and connect to the RPi via usb.

# Tests
To run tests, run any python file that's in the `tests` folder. They are all independent from each other.

# Complete Documentation
To view the full documentation, follow this link: https://docs.google.com/document/d/12jAbZjaoGc_1DOfswhz2kICc9eU9R_vDiXhDnZYvync/edit?usp=sharing 