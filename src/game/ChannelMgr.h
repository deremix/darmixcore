/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKECORE_CHANNELMGR_H
#define BLIZZLIKECORE_CHANNELMGR_H

#include "Channel.h"
#include "Policies/Singleton.h"
#include "World.h"

#include <map>
#include <string>

class ChannelMgr
{
    public:
        typedef std::map<std::string,Channel *> ChannelMap;
        ChannelMgr() {}
        ~ChannelMgr()
        {
            for (ChannelMap::iterator itr = channels.begin();itr != channels.end(); ++itr)
                delete itr->second;
            channels.clear();
        }
        Channel *GetJoinChannel(const std::string& name, uint32 channel_id)
        {
            if (channels.count(name) == 0)
            {
                Channel *nchan = new Channel(name,channel_id);
                channels[name] = nchan;
            }
            return channels[name];
        }
        Channel *GetChannel(const std::string& name, Player* p)
        {
            ChannelMap::const_iterator i = channels.find(name);

            if (i == channels.end())
            {
                WorldPacket data;
                MakeNotOnPacket(&data,name);
                p->GetSession()->SendPacket(&data);
                return NULL;
            }
            else
                return i->second;
        }
        void LeftChannel(const std::string& name)
        {
            ChannelMap::const_iterator i = channels.find(name);

            if (i == channels.end())
                return;

            Channel* channel = i->second;

            if (channel->GetNumPlayers() == 0 && !channel->IsConstant())
            {
                channels.erase(name);
                delete channel;
            }
        }
    private:
        ChannelMap channels;
        void MakeNotOnPacket(WorldPacket* data, const std::string& name)
        {
            data->Initialize(SMSG_CHANNEL_NOTIFY, (1+10));  // we guess size
            (*data) << (uint8)0x05 << name;
        }
};

class AllianceChannelMgr : public ChannelMgr {};
class HordeChannelMgr    : public ChannelMgr {};

inline ChannelMgr* channelMgr(uint32 team)
{
    if (sWorld.getConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_CHANNEL))
                                                            //For Test,No Seprate Faction
        return &BlizzLike::Singleton<AllianceChannelMgr>::Instance();

    if (team == ALLIANCE)
        return &BlizzLike::Singleton<AllianceChannelMgr>::Instance();
    if (team == HORDE)
        return &BlizzLike::Singleton<HordeChannelMgr>::Instance();
    return NULL;
}
#endif

