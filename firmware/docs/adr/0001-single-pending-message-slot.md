# A newly arrived message replaces the pending one

The Connected Heart holds at most one pending message. When a message arrives while another is still unrevealed, the older one is discarded and the recipient never sees it. A queue was not used: e-paper refreshes are slow, so revealing several old messages one touch at a time would be tedious.

## Consequences

Messages can be lost. If two messages are sent before the recipient touches the heart, only the second one is shown.

The sender is told about it: when a new message replaces an unrevealed one, the acknowledgement says that the previous message was replaced before it was opened (see ADR-0002). The recipient is not told, since for them the replaced message was never shown.

`MessageManager` has no queue on purpose. A queue could be added in a later version, but that is a product decision.
