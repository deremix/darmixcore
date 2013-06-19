/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKE_WAYPOINTMANAGER_H
#define BLIZZLIKE_WAYPOINTMANAGER_H

#include <ace/Singleton.h>
#include <ace/Null_Mutex.h>
#include <vector>

struct WaypointData
{
    uint32 id;
    float x,y,z;
    bool run;
    uint32 delay;
    uint32 event_id;
    uint8 event_chance;
};

typedef std::vector<WaypointData*> WaypointPath;

class WaypointStore
{
    private :
        uint32  records;
        UNORDERED_MAP<uint32, WaypointPath*> waypoint_map;

    public:
        // Null Mutex is OK because WaypointMgr is initialized in the World thread before World is initialized
        static WaypointStore* instance() { return ACE_Singleton<WaypointStore, ACE_Null_Mutex>::instance(); }

        ~WaypointStore() { Free(); }
        void UpdatePath(uint32 id);
        void Load();
        void Free();

        WaypointPath* GetPath(uint32 id)
        {
            if (waypoint_map.find(id) != waypoint_map.end())
                return waypoint_map[id];
            else return 0;
        }

        inline uint32 GetRecordsCount() { return records; }
};

#define sWaypointMgr WaypointStore::instance()

#endif

