# The bot acknowledges delivery, never reading

The sender has no way to see whether a message reached the heart, so the Telegram bot replies with an acknowledgement as soon as the device accepts a message. It is a delivery acknowledgement: it is sent on arrival, not when the recipient touches the heart.

A read receipt was rejected. The heart has no concept of a message being read, and adding one would let the sender check when the recipient looked at it. That is not something the device should report.

## Consequences

The heart itself never sends anything; only the bot replies. The acknowledgement confirms that the message was received, nothing more.

Sending the acknowledgement on touch, or reporting any other device state back to the sender, would go against this decision and needs this ADR to be revisited first.
