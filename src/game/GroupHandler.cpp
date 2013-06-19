/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Opcodes.h"
#include "Log.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Group.h"
#include "ObjectAccessor.h"
#include "MapManager.h"
#include "SocialMgr.h"
#include "Util.h"

/* differeces from off:
    -you can uninvite yourself - is is useful
    -you can accept invitation even if leader went offline
*/
/* todo:
    -group_destroyed msg is sent but not shown
    -reduce xp gaining when in raid group
    -quest sharing has to be corrected
    -FIX sending PartyMemberStats
*/

void WorldSession::SendPartyResult(PartyOperation operation, const std::string& member, PartyResult res)
{
    WorldPacket data(SMSG_PARTY_COMMAND_RESULT, (4+member.size()+1+4));
    data << (uint32)operation;
    data << member;
    data << (uint32)res;

    SendPacket(&data);
}

void WorldSession::HandleGroupInviteOpcode(WorldPacket& recv_data)
{
    std::string membername;
    recv_data >> membername;

    // attempt add selected player

    // cheating
    if (!normalizePlayerName(membername))
    {
        SendPartyResult(PARTY_OP_INVITE, membername, PARTY_RESULT_CANT_FIND_TARGET);
        return;
    }

    Player* player = ObjectAccessor::Instance().FindPlayerByName(membername.c_str());

    // no player
    if (!player)
    {
        SendPartyResult(PARTY_OP_INVITE, membername, PARTY_RESULT_CANT_FIND_TARGET);
        return;
    }

    // restrict invite to GMs
    if (!sWorld.getConfig(CONFIG_ALLOW_GM_GROUP) && !GetPlayer()->isGameMaster() && player->isGameMaster())
    {
        SendPartyResult(PARTY_OP_INVITE, membername, PARTY_RESULT_CANT_FIND_TARGET);
        return;
    }
    // can't group with
    if (!sWorld.getConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP) && GetPlayer()->GetTeam() != player->GetTeam())
    {
        SendPartyResult(PARTY_OP_INVITE, membername, PARTY_RESULT_TARGET_UNFRIENDLY);
        return;
    }
    if (GetPlayer()->GetInstanceId() != 0 && player->GetInstanceId() != 0 && GetPlayer()->GetInstanceId() != player->GetInstanceId() && GetPlayer()->GetMapId() == player->GetMapId())
    {
        SendPartyResult(PARTY_OP_INVITE, membername, PARTY_RESULT_NOT_IN_YOUR_INSTANCE);
        return;
    }
    // just ignore us
    if (player->GetInstanceId() != 0 && player->GetDifficulty() != GetPlayer()->GetDifficulty())
    {
        SendPartyResult(PARTY_OP_INVITE, membername, PARTY_RESULT_TARGET_IGNORE_YOU);
        return;
    }

    if (player->GetSocial()->HasIgnore(GetPlayer()->GetGUIDLow()))
    {
        SendPartyResult(PARTY_OP_INVITE, membername, PARTY_RESULT_TARGET_IGNORE_YOU);
        return;
    }

    Group* group = GetPlayer()->GetGroup();
    if (group && group->isBGGroup())
        group = GetPlayer()->GetOriginalGroup();

    Group* group2 = player->GetGroup();
    if (group2 && group2->isBGGroup())
        group2 = player->GetOriginalGroup();

    // player already in another group or invited
    if (group2 || player->GetGroupInvite())
    {
        SendPartyResult(PARTY_OP_INVITE, membername, PARTY_RESULT_ALREADY_IN_GROUP);
        return;
    }

    if (group)
    {
        // not have permissions for invite
        if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()))
        {
            SendPartyResult(PARTY_OP_INVITE, "", PARTY_RESULT_YOU_NOT_LEADER);
            return;
        }
        // not have place
        if (group->IsFull())
        {
            SendPartyResult(PARTY_OP_INVITE, "", PARTY_RESULT_PARTY_FULL);
            return;
        }
    }

    // ok, but group not exist, start a new group
    // but don't create and save the group to the DB until
    // at least one person joins
    if (!group)
    {
        group = new Group;
        // new group: if can't add then delete
        if (!group->AddLeaderInvite(GetPlayer()))
        {
            delete group;
            return;
        }
        if (!group->AddInvite(player))
        {
            delete group;
            return;
        }
    }
    else
    {
        // already existed group: if can't add then just leave
        if (!group->AddInvite(player))
        {
            return;
        }
    }

    // ok, we do it
    WorldPacket data(SMSG_GROUP_INVITE, 10);                // guess size
    data << GetPlayer()->GetName();
    player->GetSession()->SendPacket(&data);

    SendPartyResult(PARTY_OP_INVITE, membername, PARTY_RESULT_OK);
}

void WorldSession::HandleGroupAcceptOpcode(WorldPacket & /*recv_data*/)
{
    Group* group = GetPlayer()->GetGroupInvite();
    if (!group) return;

    if (group->GetLeaderGUID() == GetPlayer()->GetGUID())
    {
        sLog.outError("HandleGroupAcceptOpcode: player %s(%d) tried to accept an invite to his own group", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
        return;
    }

    // remove in from ivites in any case
    group->RemoveInvite(GetPlayer());

    /** error handling **/
    /********************/

    // not have place
    if (group->IsFull())
    {
        SendPartyResult(PARTY_OP_INVITE, "", PARTY_RESULT_PARTY_FULL);
        return;
    }

    Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID());

    // forming a new group, create it
    if (!group->IsCreated())
    {
        if (leader)
            group->RemoveInvite(leader);
        group->Create(group->GetLeaderGUID(), group->GetLeaderName());
        objmgr.AddGroup(group);
    }

    // everything is fine, do it, PLAYER'S GROUP IS SET IN ADDMEMBER!!!
    if (!group->AddMember(GetPlayer()->GetGUID(), GetPlayer()->GetName()))
        return;

    group->BroadcastGroupUpdate();
}

void WorldSession::HandleGroupDeclineOpcode(WorldPacket & /*recv_data*/)
{
    Group  *group  = GetPlayer()->GetGroupInvite();
    if (!group) return;

    // remember leader if online
    Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID());

    // uninvite, group can be deleted
    GetPlayer()->UninviteFromGroup();

    if (!leader || !leader->GetSession())
        return;

    // report
    WorldPacket data(SMSG_GROUP_DECLINE, 10);             // guess size
    data << GetPlayer()->GetName();
    leader->GetSession()->SendPacket(&data);
}

void WorldSession::HandleGroupUninviteGuidOpcode(WorldPacket& recv_data)
{
    uint64 guid;
    recv_data >> guid;

    //can't uninvite yourself
    if (guid == GetPlayer()->GetGUID())
    {
        sLog.outError("WorldSession::HandleGroupUninviteGuidOpcode: leader %s(%d) tried to uninvite himself from the group.", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
        return;
    }

    PartyResult res = GetPlayer()->CanUninviteFromGroup();
    if (res != PARTY_RESULT_OK)
    {
        SendPartyResult(PARTY_OP_LEAVE, "", res);
        return;
    }

    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
        return;

    if (grp->IsMember(guid))
    {
        Player::RemoveFromGroup(grp,guid);
        return;
    }

    if (Player* plr = grp->GetInvited(guid))
    {
        plr->UninviteFromGroup();
        return;
    }

    SendPartyResult(PARTY_OP_LEAVE, "", PARTY_RESULT_NOT_IN_YOUR_PARTY);
}

void WorldSession::HandleGroupUninviteNameOpcode(WorldPacket& recv_data)
{
    std::string membername;
    recv_data >> membername;

    // player not found
    if (!normalizePlayerName(membername))
        return;

    // can't uninvite yourself
    if (GetPlayer()->GetName() == membername)
    {
        sLog.outError("WorldSession::HandleGroupUninviteNameOpcode: leader %s(%d) tried to uninvite himself from the group.", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
        return;
    }

    PartyResult res = GetPlayer()->CanUninviteFromGroup();
    if (res != PARTY_RESULT_OK)
    {
        SendPartyResult(PARTY_OP_LEAVE, "", res);
        return;
    }

    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
        return;

    if (uint64 guid = grp->GetMemberGUID(membername))
    {
        Player::RemoveFromGroup(grp,guid);
        return;
    }

    if (Player* plr = grp->GetInvited(membername))
    {
        plr->UninviteFromGroup();
        return;
    }

    SendPartyResult(PARTY_OP_LEAVE, membername, PARTY_RESULT_NOT_IN_YOUR_PARTY);
}

void WorldSession::HandleGroupSetLeaderOpcode(WorldPacket& recv_data)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    uint64 guid;
    recv_data >> guid;

    Player* player = ObjectAccessor::FindPlayer(guid);

    /** error handling **/
    if (!player || !group->IsLeader(GetPlayer()->GetGUID()) || player->GetGroup() != group)
        return;
    /********************/

    // everything is fine, do it
    group->ChangeLeader(guid);
}

void WorldSession::HandleGroupLeaveOpcode(WorldPacket & /*recv_data*/)
{
    if (!GetPlayer()->GetGroup())
        return;

    if (_player->InBattleGround())
    {
        SendPartyResult(PARTY_OP_INVITE, "", PARTY_RESULT_INVITE_RESTRICTED);
        return;
    }

    /** error handling **/
    /********************/

    // everything is fine, do it
    SendPartyResult(PARTY_OP_LEAVE, GetPlayer()->GetName(), PARTY_RESULT_OK);

    GetPlayer()->RemoveFromGroup();
}

void WorldSession::HandleLootMethodOpcode(WorldPacket& recv_data)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    uint32 lootMethod;
    uint64 lootMaster;
    uint32 lootThreshold;
    recv_data >> lootMethod >> lootMaster >> lootThreshold;

    /** error handling **/
    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;
    /********************/

    // everything is fine, do it
    group->SetLootMethod((LootMethod)lootMethod);
    group->SetLooterGuid(lootMaster);
    group->SetLootThreshold((ItemQualities)lootThreshold);
    group->SendUpdate();
}

void WorldSession::HandleLootRoll(WorldPacket& recv_data)
{
    if (!GetPlayer()->GetGroup())
        return;

    uint64 Guid;
    uint32 NumberOfPlayers;
    uint8  Choise;
    recv_data >> Guid;                                      //guid of the item rolled
    recv_data >> NumberOfPlayers;
    recv_data >> Choise;                                    //0: pass, 1: need, 2: greed

    //sLog.outDebug("WORLD RECIEVE CMSG_LOOT_ROLL, From:%u, Numberofplayers:%u, Choise:%u", (uint32)Guid, NumberOfPlayers, Choise);

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    // everything is fine, do it
    group->CountRollVote(GetPlayer()->GetGUID(), Guid, NumberOfPlayers, Choise);
}

void WorldSession::HandleMinimapPingOpcode(WorldPacket& recv_data)
{
    if (!GetPlayer()->GetGroup())
        return;

    float x, y;
    recv_data >> x;
    recv_data >> y;

    //sLog.outDebug("Received opcode MSG_MINIMAP_PING X: %f, Y: %f", x, y);

    /** error handling **/
    /********************/

    // everything is fine, do it
    WorldPacket data(MSG_MINIMAP_PING, (8+4+4));
    data << GetPlayer()->GetGUID();
    data << x;
    data << y;
    GetPlayer()->GetGroup()->BroadcastPacket(&data, true, -1, GetPlayer()->GetGUID());
}

void WorldSession::HandleRandomRollOpcode(WorldPacket& recv_data)
{
    uint32 minimum, maximum, roll;
    recv_data >> minimum;
    recv_data >> maximum;

    /** error handling **/
    if (minimum > maximum || maximum > 10000)                // < 32768 for urand call
        return;
    /********************/

    // everything is fine, do it
    roll = urand(minimum, maximum);

    //sLog.outDebug("ROLL: MIN: %u, MAX: %u, ROLL: %u", minimum, maximum, roll);

    WorldPacket data(MSG_RANDOM_ROLL, 4+4+4+8);
    data << minimum;
    data << maximum;
    data << roll;
    data << GetPlayer()->GetGUID();
    if (GetPlayer()->GetGroup())
        GetPlayer()->GetGroup()->BroadcastPacket(&data, false);
    else
        SendPacket(&data);
}

void WorldSession::HandleRaidIconTargetOpcode(WorldPacket& recv_data)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    uint8  x;
    recv_data >> x;

    /** error handling **/
    /********************/

    // everything is fine, do it
    if (x == 0xFF)                                           // target icon request
    {
        group->SendTargetIconList(this);
    }
    else                                                    // target icon update
    {
        if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()))
            return;

        uint64 guid;
        recv_data >> guid;
        group->SetTargetIcon(x, guid);
    }
}

void WorldSession::HandleRaidConvertOpcode(WorldPacket & /*recv_data*/)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (_player->InBattleGround())
        return;

    /** error handling **/
    if (!group->IsLeader(GetPlayer()->GetGUID()) || group->GetMembersCount() < 2)
        return;
    /********************/

    // everything is fine, do it (is it 0 (PARTY_OP_INVITE) correct code)
    SendPartyResult(PARTY_OP_INVITE, "", PARTY_RESULT_OK);
    group->ConvertToRaid();
}

void WorldSession::HandleGroupChangeSubGroupOpcode(WorldPacket& recv_data)
{
    // we will get correct pointer for group here, so we don't have to check if group is BG raid
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    std::string name;
    uint8 groupNr;
    recv_data >> name;
    recv_data >> groupNr;

    /** error handling **/
    if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()))
        return;

    if (!group->HasFreeSlotSubGroup(groupNr))
        return;
    /********************/

    // everything is fine, do it
    if (Player* player = ObjectAccessor::Instance().FindPlayerByName(name.c_str()))
        group->ChangeMembersGroup(player, groupNr);
    else
        group->ChangeMembersGroup(objmgr.GetPlayerGUIDByName(name.c_str()), groupNr);
}

void WorldSession::HandleGroupAssistantOpcode(WorldPacket& recv_data)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    uint64 guid;
    uint8 flag;
    recv_data >> guid;
    recv_data >> flag;

    /** error handling **/
    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;
    /********************/

    // everything is fine, do it
    group->SetAssistant(guid, (flag == 0?false:true));
}

void WorldSession::HandlePartyAssignmentOpcode(WorldPacket& recv_data)
{
    uint8 flag1, flag2;
    uint64 guid;
    recv_data >> flag1 >> flag2;
    recv_data >> guid;

    sLog.outDebug("MSG_PARTY_ASSIGNMENT");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    // if (flag1) Main Assist
    //     0x4
    // if (flag2) Main Tank
    //     0x2

    /** error handling **/
    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;
    /********************/

    // everything is fine, do it
    if (flag1 == 1)
        group->SetMainAssistant(guid);
    if (flag2 == 1)
        group->SetMainTank(guid);
}

void WorldSession::HandleRaidReadyCheckOpcode(WorldPacket& recv_data)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (recv_data.empty())                                   // request
    {
        /** error handling **/
        if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()))
            return;
        /********************/

        // everything is fine, do it
        WorldPacket data(MSG_RAID_READY_CHECK, 8);
        data << GetPlayer()->GetGUID();
        group->BroadcastPacket(&data, false, -1);

        group->OfflineReadyCheck();
    }
    else                                                    // answer
    {
        uint8 state;
        recv_data >> state;

        // everything is fine, do it
        WorldPacket data(MSG_RAID_READY_CHECK_CONFIRM, 9);
        data << GetPlayer()->GetGUID();
        data << state;
        group->BroadcastReadyCheck(&data);
    }
}

void WorldSession::HandleRaidReadyCheckFinishOpcode(WorldPacket& /*recv_data*/)
{
    //Group* group = GetPlayer()->GetGroup();
    //if (!group)
    //    return;

    //if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()))
    //    return;

    // Is any reaction need?
}

void WorldSession::BuildPartyMemberStatsChangedPacket(Player* player, WorldPacket* data)
{
    uint32 mask = player->GetGroupUpdateFlag();

    if (mask == GROUP_UPDATE_FLAG_NONE)
        return;

    if (mask & GROUP_UPDATE_FLAG_POWER_TYPE)                // if update power type, update current/max power also
        mask |= (GROUP_UPDATE_FLAG_CUR_POWER | GROUP_UPDATE_FLAG_MAX_POWER);

    if (mask & GROUP_UPDATE_FLAG_PET_POWER_TYPE)            // same for pets
        mask |= (GROUP_UPDATE_FLAG_PET_CUR_POWER | GROUP_UPDATE_FLAG_PET_MAX_POWER);

    uint32 byteCount = 0;
    for (int i = 1; i < GROUP_UPDATE_FLAGS_COUNT; ++i)
        if (mask & (1 << i))
            byteCount += GroupUpdateLength[i];

    data->Initialize(SMSG_PARTY_MEMBER_STATS, 8 + 4 + byteCount);
    *data << player->GetPackGUID();
    *data << uint32(mask);

    if (mask & GROUP_UPDATE_FLAG_STATUS)
    {
        if (player)
        {
            if (player->IsPvP())
                *data << (uint16) (MEMBER_STATUS_ONLINE | MEMBER_STATUS_PVP);
            else
                *data << (uint16) MEMBER_STATUS_ONLINE;
        }
        else
            *data << (uint16) MEMBER_STATUS_OFFLINE;
    }

    if (mask & GROUP_UPDATE_FLAG_CUR_HP)
        *data << (uint16) player->GetHealth();

    if (mask & GROUP_UPDATE_FLAG_MAX_HP)
        *data << (uint16) player->GetMaxHealth();

    Powers powerType = player->getPowerType();
    if (mask & GROUP_UPDATE_FLAG_POWER_TYPE)
        *data << (uint8) powerType;

    if (mask & GROUP_UPDATE_FLAG_CUR_POWER)
        *data << (uint16) player->GetPower(powerType);

    if (mask & GROUP_UPDATE_FLAG_MAX_POWER)
        *data << (uint16) player->GetMaxPower(powerType);

    if (mask & GROUP_UPDATE_FLAG_LEVEL)
        *data << (uint16) player->getLevel();

    if (mask & GROUP_UPDATE_FLAG_ZONE)
        *data << (uint16) player->GetZoneId();

    if (mask & GROUP_UPDATE_FLAG_POSITION)
        *data << (uint16) player->GetPositionX() << (uint16) player->GetPositionY();

    if (mask & GROUP_UPDATE_FLAG_AURAS)
    {
        uint64 auramask = player->GetAuraUpdateMask();
        *data << uint64(auramask);
        for (uint32 i = 0; i < MAX_AURAS; ++i)
        {
            if (auramask & (uint64(1) << i))
            {
                uint32 updatedAura=player->GetUInt32Value(uint16(UNIT_FIELD_AURA + i));
                *data << uint16(updatedAura);
                *data << uint8(1);
            }
        }
    }

    Pet *pet = player->GetPet();
    if (mask & GROUP_UPDATE_FLAG_PET_GUID)
    {
        if (pet)
            *data << (uint64) pet->GetGUID();
        else
            *data << (uint64) 0;
    }

    if (mask & GROUP_UPDATE_FLAG_PET_NAME)
    {
        if (pet)
            *data << pet->GetName();
        else
            *data << (uint8)  0;
    }

    if (mask & GROUP_UPDATE_FLAG_PET_MODEL_ID)
    {
        if (pet)
            *data << (uint16) pet->GetDisplayId();
        else
            *data << (uint16) 0;
    }

    if (mask & GROUP_UPDATE_FLAG_PET_CUR_HP)
    {
        if (pet)
            *data << (uint16) pet->GetHealth();
        else
            *data << (uint16) 0;
    }

    if (mask & GROUP_UPDATE_FLAG_PET_MAX_HP)
    {
        if (pet)
            *data << (uint16) pet->GetMaxHealth();
        else
            *data << (uint16) 0;
    }

    if (mask & GROUP_UPDATE_FLAG_PET_POWER_TYPE)
    {
        if (pet)
            *data << (uint8)  pet->getPowerType();
        else
            *data << (uint8)  0;
    }

    if (mask & GROUP_UPDATE_FLAG_PET_CUR_POWER)
    {
        if (pet)
            *data << (uint16) pet->GetPower(pet->getPowerType());
        else
            *data << (uint16) 0;
    }

    if (mask & GROUP_UPDATE_FLAG_PET_MAX_POWER)
    {
        if (pet)
            *data << (uint16) pet->GetMaxPower(pet->getPowerType());
        else
            *data << (uint16) 0;
    }

    if (mask & GROUP_UPDATE_FLAG_PET_AURAS)
    {
        if (pet)
        {
            uint64 auramask = pet->GetAuraUpdateMask();
            *data << uint64(auramask);
            for (uint32 i = 0; i < MAX_AURAS; ++i)
            {
                if (auramask & (uint64(1) << i))
                {
                    uint32 updatedAura=pet->GetUInt32Value(uint16(UNIT_FIELD_AURA + i));
                    *data << uint16(updatedAura);
                    *data << uint8(1);
                }
            }
        }
        else
            *data << (uint64) 0;
    }
}

/*this procedure handles clients CMSG_REQUEST_PARTY_MEMBER_STATS request*/
void WorldSession::HandleRequestPartyMemberStatsOpcode(WorldPacket& recv_data)
{
    sLog.outDebug("WORLD: Received CMSG_REQUEST_PARTY_MEMBER_STATS");
    uint64 Guid;
    recv_data >> Guid;

    Player* player = HashMapHolder<Player>::Find(Guid);
    if (!player)
    {
        WorldPacket data(SMSG_PARTY_MEMBER_STATS_FULL, 3+4+2);
        data.appendPackGUID(Guid);
        data << (uint32) GROUP_UPDATE_FLAG_STATUS;
        data << (uint16) MEMBER_STATUS_OFFLINE;
        SendPacket(&data);
        return;
    }

    Pet *pet = player->GetPet();

    WorldPacket data(SMSG_PARTY_MEMBER_STATS_FULL, 4+2+2+2+1+2*6+8+1+8);
    data << player->GetPackGUID();

    uint32 mask1 = 0x00040BFF;                              // common mask, real flags used 0x000040BFF
    if (pet)
        mask1 = 0x7FFFFFFF;                                 // for hunters and other classes with pets

    Powers powerType = player->getPowerType();
    data << (uint32) mask1;                                 // group update mask
    data << (uint16) MEMBER_STATUS_ONLINE;                  // member's online status
    data << (uint16) player->GetHealth();                   // GROUP_UPDATE_FLAG_CUR_HP
    data << (uint16) player->GetMaxHealth();                // GROUP_UPDATE_FLAG_MAX_HP
    data << (uint8)  powerType;                             // GROUP_UPDATE_FLAG_POWER_TYPE
    data << (uint16) player->GetPower(powerType);           // GROUP_UPDATE_FLAG_CUR_POWER
    data << (uint16) player->GetMaxPower(powerType);        // GROUP_UPDATE_FLAG_MAX_POWER
    data << (uint16) player->getLevel();                    // GROUP_UPDATE_FLAG_LEVEL
    data << (uint16) player->GetZoneId();                   // GROUP_UPDATE_FLAG_ZONE
    data << (uint16) player->GetPositionX();                // GROUP_UPDATE_FLAG_POSITION
    data << (uint16) player->GetPositionY();                // GROUP_UPDATE_FLAG_POSITION

    uint64 auramask = 0;
    size_t maskPos = data.wpos();
    data << (uint64) auramask;                              // placeholder
    for (uint8 i = 0; i < MAX_AURAS; ++i)
    {
        if (uint32 aura = player->GetUInt32Value(UNIT_FIELD_AURA + i))
        {
            auramask |= (uint64(1) << i);
            data << uint16(aura);
            data << uint8(1);
        }
    }
    data.put<uint64>(maskPos,auramask);                     // GROUP_UPDATE_FLAG_AURAS

    if (pet)
    {
        Powers petpowertype = pet->getPowerType();
        data << (uint64) pet->GetGUID();                    // GROUP_UPDATE_FLAG_PET_GUID
        data << pet->GetName();                             // GROUP_UPDATE_FLAG_PET_NAME
        data << (uint16) pet->GetDisplayId();               // GROUP_UPDATE_FLAG_PET_MODEL_ID
        data << (uint16) pet->GetHealth();                  // GROUP_UPDATE_FLAG_PET_CUR_HP
        data << (uint16) pet->GetMaxHealth();               // GROUP_UPDATE_FLAG_PET_MAX_HP
        data << (uint8)  petpowertype;                      // GROUP_UPDATE_FLAG_PET_POWER_TYPE
        data << (uint16) pet->GetPower(petpowertype);       // GROUP_UPDATE_FLAG_PET_CUR_POWER
        data << (uint16) pet->GetMaxPower(petpowertype);    // GROUP_UPDATE_FLAG_PET_MAX_POWER

        uint64 petauramask = 0;
        size_t petMaskPos = data.wpos();
        data << (uint64) petauramask;                       // placeholder
        for (uint8 i = 0; i < MAX_AURAS; ++i)
        {
            if (uint32 petaura = pet->GetUInt32Value(UNIT_FIELD_AURA + i))
            {
                petauramask |= (uint64(1) << i);
                data << (uint16) petaura;
                data << (uint8)  1;
            }
        }
        data.put<uint64>(petMaskPos,petauramask);           // GROUP_UPDATE_FLAG_PET_AURAS
    }
    else
    {
        data << (uint8)  0;                                 // GROUP_UPDATE_FLAG_PET_NAME
        data << (uint64) 0;                                 // GROUP_UPDATE_FLAG_PET_AURAS
    }

    SendPacket(&data);
}

/*!*/void WorldSession::HandleRequestRaidInfoOpcode(WorldPacket & /*recv_data*/)
{
    // every time the player checks the character screen
    _player->SendRaidInfo();
}

/*void WorldSession::HandleGroupCancelOpcode(WorldPacket& recv_data)
{
    sLog.outDebug("WORLD: got CMSG_GROUP_CANCEL.");
}*/

void WorldSession::HandleGroupPassOnLootOpcode(WorldPacket& recv_data)
{
    sLog.outDebug("WORLD: Received CMSG_GROUP_PASS_ON_LOOT");

    uint32 unkn;
    recv_data >> unkn;

    // ignore if player not loaded
    if (!GetPlayer())                                        // needed because STATUS_AUTHED
    {
        if (unkn != 0)
            sLog.outError("CMSG_GROUP_PASS_ON_LOOT value<>0 for not-loaded character!");
        return;
    }

    if (unkn != 0)
        sLog.outError("CMSG_GROUP_PASS_ON_LOOT: activation not implemented!");
}
