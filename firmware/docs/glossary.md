# Glossary

The terms below are used with these meanings in the code, the ADRs and the bot's replies.

A Connected Heart receives messages from one sender and shows the latest one on an e-paper display until
the next one arrives. It never sends anything. Two people can each have a heart and be each other's
sender, but the hearts do not know about each other. The device is always powered and connected, so a
message shows up as soon as it is sent.

## Device

**Connected Heart** — the device: ESP32-C3, e-paper display, notification LED and touch sensor.

**Setup** — the device has no working network or sender and opens its own access point so it can be
configured. Entered on first boot, and again after it has been unable to connect for a while.

**Waking** — the stage after power-on while the device makes its first connection.

**Recipient** — the person the heart belongs to. One per heart.

**Sender** — the only person allowed to send messages to a heart. Messages from anyone else are dropped.

**Pairing** — binding a heart to its sender, done once: the sender sends the code shown during setup to
the bot. Until then the heart accepts no messages.

## Messages

**Message** — what the sender sends. Either custom or predefined.

**Custom Message** — free text written by the sender.

**Predefined Message** — picked from a fixed list in the bot. It carries a Feeling, not text; the wording
is stored on the device.

**Feeling** — one of the five sentiments a predefined message can carry. The list is fixed per firmware
version.

**Pending Message** — arrived but not revealed yet. There is at most one; a new message replaces it
(ADR-0001).

**Current Message** — the message on the display. It stays until a new message arrives.

**Acknowledgement** — the bot's reply to the sender confirming the heart received the message. It confirms
delivery, not that it was read (ADR-0002).

## Interaction

**Notification** — the state of having a pending message and waiting for a touch. The LED pulses.

**Reveal** — what happens after the touch: the heart icon is shown while the LED beats, then the message
appears.
