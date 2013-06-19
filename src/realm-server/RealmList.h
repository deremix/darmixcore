/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef _REALMLIST_H
#define _REALMLIST_H

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include "Common.h"

struct RealmBuildInfo
{
    int build;
    int major_version;
    int minor_version;
    int bugfix_version;
    int hotfix_version;
};

RealmBuildInfo const* FindBuildInfo(uint16 _build);

typedef std::set<uint32> RealmBuilds;

// Storage object for a realm
struct Realm
{
    std::string address;
    uint8 icon;
    RealmFlags realmflags;                                  // realmflags
    uint8 timezone;
    uint32 m_ID;
    AccountTypes allowedSecurityLevel;                      // current allowed join security level (show as locked for not fit accounts)
    float populationLevel;
    RealmBuilds realmbuilds;                                // list of supported builds (updated in DB by worldserver)
    RealmBuildInfo realmBuildInfo;                          // build info for show version in list
};

// Storage object for the list of realms on the server
class RealmList
{
    public:
        // Null_Mutex is safe because the singleton initialized before the acceptor initialized(another place where the singleton called)
        static RealmList* instance() { return ACE_Singleton<RealmList, ACE_Null_Mutex>::instance(); }

        typedef std::map<std::string, Realm> RealmMap;

        RealmList();
        ~RealmList() {}

        void Initialize(uint32 updateInterval);

        void UpdateIfNeed();

        RealmMap::const_iterator begin() const { return m_realms.begin(); }
        RealmMap::const_iterator end() const { return m_realms.end(); }
        uint32 size() const { return m_realms.size(); }
    private:
        void UpdateRealms(bool init);
        void UpdateRealm( uint32 ID, const std::string& name, const std::string& address, uint32 port, uint8 icon, RealmFlags realmflags, uint8 timezone, AccountTypes allowedSecurityLevel, float popu, const char* builds);
    private:
        RealmMap m_realms;                                  ///< Internal map of realms
        uint32   m_UpdateInterval;
        time_t   m_NextUpdateTime;
};

#define sRealmList RealmList::instance()

#endif

