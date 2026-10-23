# BOM — carrier PCB rev A

Per device. Carrier parts were bought from TME. The display, touch and LED modules are off the shelf and connect to the carrier by cable.
No battery and no extra regulator: power comes from the SuperMini's USB-C and its 3.3 V LDO (ADR-0005).

## Carrier PCB

| Ref | Qty | Value / part | Package | MPN | Notes |
|---|---|---|---|---|---|
| U1 | 1 | ESP32-C3 SuperMini | module | — | Soldered flat on the castellations, USB-C over the back edge |
| U2 | 1 | 74AHCT1G125 | SOT-23-5 | TI SN74AHCT1G125DBVR | 3.3 V → 5 V for the LED data line |
| R1 | 1 | 330 Ω 5% | 0603 | Royalohm HP03W5J0331T5E | Series resistor on LED data, next to U2 |
| C1 | 1 | 220 µF 10 V electrolytic, low ESR | SMD 6.3 × 5.8 mm | Panasonic EEE-FT1A221AP | Bulk cap on 5 V |
| C2, C4 | 2 | 10 µF 25 V X5R | 0805 | Samsung CL21A106KAYNNNE | 3V3 at U1 and at J3 |
| C3, C5, C6 | 3 | 100 nF 50 V X7R | 0603 | Samsung CL10B104KB8WPNC | SW1, J1 and U2 decoupling |
| SW1 | 1 | Tactile switch, side-actuated, 1.6 N | 3.5 × 2.9 mm SMD | Panasonic EVQ-P7L01P | Factory reset (GPIO20), behind a pin-hole in the back wall |
| J1, J2 | 2 | JST-PH 3-pin, vertical | THT, 2.0 mm | JST B3B-PH-K-S | J1 touch, J2 LEDs |
| J3 | 1 | Molex PicoBlade 8-pin, vertical | THT, 1.25 mm | Molex 53047-0810 | E-paper cable |
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

- **U2:** The LEDs run on 5 V, and at 5 V the WS2812B needs at least 3.5 V (0.7 × VDD) to read a high.
  The ESP32-C3 only gives 3.3 V. It usually works anyway, but I didn't want to depend on something out of
  spec, so I added a level shifter. JP1 is a bypass in case I want to leave U2 out; only one of them is
  fitted.
- **C1:** there is no battery, so C1 covers the current peaks (Wi-Fi TX plus LEDs) on the 5 V rail.
- Not on the board because the modules already have them: USB-C CC resistors, 3.3 V regulator, e-paper
  boost circuit, pull-ups on the display lines, LED decoupling. GPIO20 uses the internal pull-up.
