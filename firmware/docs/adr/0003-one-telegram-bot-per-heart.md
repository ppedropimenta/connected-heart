# Each heart has its own Telegram bot

Every Connected Heart talks to its own Telegram bot, created through BotFather. The bot token is the device's identity: there is no device ID, no pairing service and no backend. The heart polls Telegram directly.

The alternative, one shared bot for every heart, needs a server that maps senders to devices, and with it a real device identity and a backend to operate. For a single pair of devices that is a lot of infrastructure.

## Consequences

It costs nothing, but it is manual and limited. Telegram does not charge per bot or per message, but one Telegram account can create only about 20 bots, and each device needs its token set up individually.

This does not scale to a product. At roughly 20 units it has to move to a shared backend. The idea for that step is a dedicated app for sending messages, with its own backend replacing the Telegram bot.
