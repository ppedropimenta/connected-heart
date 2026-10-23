# The heart is always powered and always connected

A Connected Heart runs from USB and keeps its Wi-Fi connection open, so a message appears as soon as it is sent. It never sleeps and has no battery.

The alternative was a battery with deep sleep. Rough numbers for a 3000 mAh cell, ignoring conversion losses:

- **Always connected:** the firmware polls Telegram over HTTPS every 3 s, so the radio hardly ever sleeps. At about 80–100 mA on average, the cell lasts around 30–40 hours.
- **Deep sleep with a poll every 5 minutes:** each wake-up (connect, poll, go back to sleep) takes about 5 s at around 120 mA, which averages about 2 mA. Adding the SuperMini's own sleep current, the cell would last roughly one to two months.

The battery life gain is large, but deep sleep was rejected for two reasons:

- **The LED can't pulse during deep sleep.** The pulse needs the CPU running. A sleeping heart could at most hold a static colour, and even a dim LED draws more than the polling saves. The pulsing LED is the main way the heart shows that a message is waiting.
- **Messages should arrive immediately.** With a poll every few minutes, a message can take minutes to appear, and the acknowledgement to the sender arrives just as late.

## Consequences

Battery life is not a constraint anywhere in the firmware: there is no power management, no sleep mode and no battery monitoring.

If deep sleep is ever revisited, the pin assignment has to be checked again for wake-up capable pins.
