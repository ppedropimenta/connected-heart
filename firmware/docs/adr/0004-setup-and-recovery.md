# Setup and recovery assume the heart is unreachable

A Connected Heart may be given to someone far away, so every setup and recovery path has to work without physical access and without instructions.

The heart is set up through a captive portal: it boots as an access point, the recipient joins it with their phone, chooses their network and enters the password. The same portal page shows a short pairing code. The sender sends that code to the bot, and the first correct code stores the sender's chat ID in NVS, together with the Wi-Fi credentials and the current message.

The pairing code means neither person has to look up their numeric Telegram ID. Letting the first person who writes to the bot claim it would be simpler, but anyone could then take the device.

## Recovery

There are two mechanisms, for two different failures:

- **Automatic**: after five failed connection attempts in a row (about 2.5 minutes), the heart reopens the captive portal and says so on the e-paper. This covers a mistyped password or a new router, without anyone needing to know about a button.
- **Manual**: a long press on the reset button clears NVS completely, for re-pairing or a new owner.

## Consequences

The reset button has to stay reachable through the enclosure, and it must not be on GPIO2, GPIO8 or GPIO9. These are ESP32-C3 strapping pins, and pressing a button on one of them during power-on would put the device into download mode.
