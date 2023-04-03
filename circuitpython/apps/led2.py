# red LED on and off - 2022/05/23 @kreier
# LILYGO T-Display RP2040
# v0.2 2023/03/04

import time
import board
import digitalio

led = digitalio.DigitalInOut(board.GP25)
led.direction = digitalio.Direction.OUTPUT
for i in range(5):
    print("Turn LED on")
    led.value = True
    time.sleep(1)
    print("Turn LED off")
    led.value = False
    time.sleep(1)

print("Done!")
led.deinit()

