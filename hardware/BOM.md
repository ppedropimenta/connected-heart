# BOM — carrier PCB rev A

Per device. The display, touch and LED modules are off the shelf and connect to the carrier by cable.
No battery and no extra regulator: power comes from the SuperMini's USB-C and its 3.3 V LDO (ADR-0005).

## Carrier PCB

| Ref | Qty | Value / part | Package | MPN | Notes |
|---|---|---|---|---|---|
| U1 | 1 | ESP32-C3 SuperMini | module | — | Soldered flat on the castellations, USB-C over the back edge |
| U2 | 1 | 74AHCT1G125 | SOT-23-5 | TI SN74AHCT1G125DBVR | 3.3 V → 5 V for the LED data line |
| R1 | 1 | 330 Ω 1% | 0603 | Yageo RC0603FR-07330RL | Series resistor on LED data, next to U2 |
| C1 | 1 | 220 µF 10 V polymer | SMD 6.3 mm | Würth 875105244013 | Bulk cap on 5 V |
| C2, C4 | 2 | 10 µF 25 V X5R | 0805 | KYOCERA AVX KGM21AR51E106KU | 3V3 at U1 and at J3 |
| C3, C5, C6 | 3 | 100 nF 50 V X7R | 0603 | Yageo CC0603KRX7R9BB104 | SW1, J1 and U2 decoupling |
| SW1 | 1 | Tactile switch, side-actuated, 2.2 N | 3.5 × 2.9 mm SMD | Panasonic EVQ-P7A01P | Factory reset (GPIO20), behind a pin-hole in the back wall |
| J1, J2 | 2 | JST-PH 3-pin, vertical | THT, 2.0 mm | JST B3B-PH-K-S | J1 touch, J2 LEDs |
| J3 | 1 | Molex PicoBlade 8-pin, vertical | THT, 1.25 mm | Molex 0530470810 | E-paper cable |
| JP1 | — | Solder jumper | — | — | Bypass for U2. Left open |

## Off-board

| Qty | Part | Notes |
|---|---|---|
| 1 | 2.13" e-paper module (SSD1680-class) | Driven with `GxEPD2_213_BN` |
| 1 | TTP223 touch module | Default pads (momentary, active high) |
| 1 | WS2812B module, 3 LEDs | 5 V |
| 1 | MX1.25 8-pin cable, same direction (pin 1 to pin 1) | J3 to display |
| 2 | JST-PH 3-pin cable | J1 to touch, J2 to LEDs |

## Notes

- **U2:** WS2812B needs V_IH ≥ 0.7 × VDD = 3.5 V at 5 V, and the ESP32-C3 outputs 3.3 V. It often works
  without a level shifter, but that is out of spec. JP1 is only there to bypass U2 if needed; never
  fit both.
- **C1:** there is no battery, so C1 covers the current peaks (Wi-Fi TX plus LEDs) on the 5 V rail.
- Order the SOT-23-5 version of the 74AHCT1G125 (`DBVR` / `GV`). The SOT-353 version (`GW`) does not
  fit the footprint.
- Not on the board because the modules already have them: USB-C CC resistors, 3.3 V regulator, e-paper
  boost circuit, pull-ups on the display lines, LED decoupling. GPIO20 uses the internal pull-up.
