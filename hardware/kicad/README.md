# KiCad project — carrier rev A

KiCad 10. Only the schematic is published; `preview/` has the schematic as PDF and renders of the board.

| File | |
|---|---|
| `connected-heart-carrier.kicad_pro` | Project |
| `connected-heart-carrier.kicad_sch` | Schematic, one A4 sheet |
| `connected-heart.kicad_sym` | Symbol for the ESP32-C3 SuperMini (everything else is from the KiCad libraries) |
| `preview/` | Schematic PDF and board renders |

## Checks

- ERC: no errors or warnings
- DRC with schematic parity: no violations, nothing unconnected
- Netlist checked against the pin map in `firmware/src/Core/Pins.h`

## Board

2-layer, 1.6 mm, 70 × 40 mm, four mounting holes.

- The SuperMini is soldered by its castellated edges, with through-holes under each pin so pin headers
  would also fit. Its USB-C end sticks out about 1 mm over the back edge.
- No copper on either layer under the SuperMini's antenna.
- C1 and C2 sit next to the SuperMini's power pins; U2, C6, JP1 and R1 next to GPIO5, so R1 is at the
  driver end of the LED cable.
- J1, J2 and J3 are vertical connectors along the front edge; every pin is labelled on the silkscreen.
- GND pour on both sides.
