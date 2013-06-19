/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef _GROUPS_H
#define _GROUPS_H

#include "Common.h"

class CreatureGroup;

struct GroupInfo
{
    uint32 leaderGUID;
    uint8  groupType;
};

class CreatureGroupManager
{
    public:
        void AddCreatureToGroup(uint32 group_id, Creature* creature);
        void RemoveCreatureFromGroup(CreatureGroup* group, Creature* creature);
        void LoadCreatureGroups();
};

#define group_mgr (*ACE_Singleton<CreatureGroupManager, ACE_Null_Mutex>::instance())

typedef UNORDERED_MAP<uint32/*groupId*/, GroupInfo*>   CreatureGroupInfoType;
typedef UNORDERED_MAP<uint32 /*memberGUID*/, uint32/*groupId*/>   CreatureGroupDataType;

extern CreatureGroupInfoType    CreatureGroupMap;
extern CreatureGroupDataType    CreatureGroupDataMap;

class CreatureGroup
{
    private:
        typedef std::map<Creature*, GroupInfo*>  CreatureGroupMemberType;
        CreatureGroupMemberType m_members;

        uint32 m_groupID;

    public:
        //Group cannot be created empty
        explicit CreatureGroup(uint32 id) : m_groupID(id) {}
        ~CreatureGroup() { sLog.outDebug("Destroying group"); }

        uint32 GetId() const { return m_groupID; }
        bool isEmpty() const { return m_members.empty(); }

        void AddMember(Creature* member);
        void RemoveMember(Creature* member);

        void MemberAttackStart(Creature* member, Unit* target);
        bool IsAllowedToRespawn(Creature* member);
};

#define formation_mgr BlizzLike::Singleton<CreatureGroupManager>::Instance()

#endif

