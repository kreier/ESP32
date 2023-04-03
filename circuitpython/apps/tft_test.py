# ST7789 on T-Display RP2040 @kreier 2022-05-23
# TFT_SCLK  2
# TFT_CS    5
# TFT_DC    1
# TFT_RST   0
# TFT_BL    4
# TFT_MOSI  3
# TFT_MISO  N/A

import board
import displayio
import busio
from adafruit_st7789 import ST7789

spi = busio.SPI(board.GP2, MOSI=board.GP3, MISO=None)
while not spi.try_lock():
    pass
spi.configure(baudrate=24000000) # Configure SPI for 24MHz
spi.unlock()
tft_cs = board.GP5
tft_dc = board.GP1

displayio.release_displays()
display_bus = displayio.FourWire(spi, command=tft_dc, chip_select=tft_cs, reset=board.GP0)

display = ST7789(display_bus, width=240, height=135, rowstart=80)

# Make the display context
splash = displayio.Group(max_size=10)
display.show(splash)

color_bitmap = displayio.Bitmap(240, 240, 1)
color_palette = displayio.Palette(1)
color_palette[0] = 0xFF0000

bg_sprite = displayio.TileGrid(color_bitmap,
                               pixel_shader=color_palette,
                               x=0, y=0)
splash.append(bg_sprite)

while True:
    pass
