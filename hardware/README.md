# Hardware

Carrier PCB and enclosure for the Connected Heart.

Rev A is built and running in the enclosure (v1.3). Schematic and PCB were done in KiCad 10 and pass ERC
and DRC. The schematic is in `kicad/`; the board layout, fabrication files and enclosure CAD are not
published.

- `BOM.md` — parts list with manufacturer part numbers
- `kicad/` — schematic (KiCad project and PDF), board renders, and notes on the layout

## Overview

**Parts:** ESP32-C3 SuperMini, 2.13" e-paper module (SSD1680-class), TTP223 touch module and a module with
3× WS2812B. All four are off-the-shelf modules with their own support circuitry, so the carrier only
connects them.

**Carrier PCB:** 70 × 40 mm, lying flat in the base of the case. The SuperMini sits at the back edge with
its USB-C sticking out through the back wall; that is the only port, for power and flashing. Next to it is
SW1, a side-actuated reset button behind a pin-hole. The three modules plug into J1 (touch), J2 (LEDs)
and J3 (display) at the front edge. U2 shifts the LED data line from 3.3 V to 5 V.

**Case:** about 74 × 46 × 50 mm. Display on the front, USB-C and reset hole on the back, touch and LED
window on top.

**Power:** 5 V from USB-C. The LEDs and U2 run on 5 V; the SuperMini's LDO supplies 3.3 V to the ESP32,
the display and the touch module. Around 130 mA steady, peaks around 560 mA (Wi-Fi TX plus LEDs).

## Pin map

Must match `firmware/src/Core/Pins.h`.

| GPIO | Function |
|---|---|
| 0 | Touch input |
| 1 | EPD DC |
| 3 | EPD BUSY |
| 4 | EPD SCK |
| 5 | LED data (through U2) |
| 6 | EPD MOSI |
| 7 | EPD CS |
| 10 | EPD RST |
| 20 | Reset button (internal pull-up) |

GPIO2, 8 and 9 are strapping pins and are left unconnected, as is GPIO21.

| J1 touch | J2 LEDs | J3 display |
|---|---|---|
| 1 3V3 | 1 5V | 1 BUSY, 2 RST, 3 DC, 4 CS |
| 2 GND | 2 GND | 5 CLK, 6 DIN, 7 GND, 8 VCC (3V3) |
| 3 OUT → GPIO0 | 3 DIN ← R1 | |

## Design decisions

- **No battery** (ADR-0005). The device is always powered. Without a battery, C1 (220 µF) handles the
  current peaks on 5 V.
- **No second USB-C port.** A power-only port would be simple, but the SuperMini doesn't break out D+/D−,
  so it couldn't be used for flashing. Turning the board so the SuperMini's own port faces the back wall
  solves it without extra parts.
- **Display on a cable instead of on the carrier.** This keeps the carrier flat, with the USB-C at the back
  and the display at the front.
- **Reset button on the board behind a pin-hole.** It's a factory reset, so it should be hard to press by
  accident.
- **Level shifter on the LED data line.** WS2812B needs at least 3.5 V for a high level at 5 V supply;
  the ESP32-C3 outputs 3.3 V. JP1 can bypass U2 but stays open.
- **LEDs on 5 V**, so their current (up to 180 mA) doesn't go through the SuperMini's LDO.
- **Antenna keep-out.** No copper under the SuperMini's antenna, and about 12 mm of space between the
  antenna and the display module.
