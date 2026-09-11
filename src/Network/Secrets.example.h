/**
* @file Secrets.example.h
 * @brief Template for the compiled-in WiFi credentials (CompiledCredentialSource)
 * and Telegram identity (TelegramMessageSource) used for bench work, ahead
 * of the captive portal (ticket 11) and pairing by code (ticket 12).
 *
 * Copy this file to Secrets.h and fill in a real network, bot token, and
 * chat identifier. Secrets.h is excluded from version control by
 * .gitignore; this example file is the one that's committed.
 */

#pragma once

#define WIFI_SSID "your-network-name"
#define WIFI_PASSWORD "your-network-password"

#define TELEGRAM_BOT_TOKEN "your-bot-token"
#define TELEGRAM_PAIRED_CHAT_ID "your-chat-id"
