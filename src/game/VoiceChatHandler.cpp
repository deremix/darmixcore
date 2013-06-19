/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "Opcodes.h"
#include "Log.h"

void WorldSession::HandleVoiceSettingsOpcode(WorldPacket& recv_data)
{
    sLog.outDebug("WORLD: CMSG_VOICE_SETTINGS");
    // uint8 isVoiceEnabled, uint8 isMicrophoneEnabled
    recv_data.read_skip<uint8>();
    recv_data.read_skip<uint8>();
    recv_data.hexlike();
}

void WorldSession::HandleChannelEnableVoiceOpcode(WorldPacket& recv_data)
{
    sLog.outDebug("WORLD: CMSG_CHANNEL_ENABLE_VOICE");
    // Enable Voice button in channel context menu
    recv_data.hexlike();
}

void WorldSession::HandleChannelVoiceChatQuery(WorldPacket& recv_data)
{
    sLog.outDebug("WORLD: CMSG_CHANNEL_VOICE_CHAT_QUERY");
    recv_data.read_skip<uint32>();
    recv_data.read_skip<char*>();
    recv_data.hexlike();
}

