/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#ifndef BLIZZLIKE_MAP_INSTANCED_H
#define BLIZZLIKE_MAP_INSTANCED_H

#include "Map.h"
#include "InstanceSaveMgr.h"

class MapInstanced : public Map
{
    friend class MapManager;
    public:
        typedef UNORDERED_MAP< uint32, Map* > InstancedMaps;

        MapInstanced(uint32 id, time_t expiry);
        ~MapInstanced() {}

        // functions overwrite Map versions
        void Update(const uint32&);
        void DelayedUpdate(const uint32 diff);
        //void RelocationNotify();
        void UnloadAll();
        bool CanEnter(Player* player);

        Map* CreateInstance(const uint32 mapId, Player* player);
        Map* FindMap(uint32 InstanceId) const { return _FindMap(InstanceId); }
        bool DestroyInstance(InstancedMaps::iterator &itr);

        void AddGridMapReference(const GridPair &p)
        {
            ++GridMapReference[p.x_coord][p.y_coord];
            SetUnloadReferenceLock(GridPair(63-p.x_coord, 63-p.y_coord), true);
        }

        void RemoveGridMapReference(const GridPair &p)
        {
            --GridMapReference[p.x_coord][p.y_coord];
            if (!GridMapReference[p.x_coord][p.y_coord])
                SetUnloadReferenceLock(GridPair(63-p.x_coord, 63-p.y_coord), false);
        }

        InstancedMaps &GetInstancedMaps() { return m_InstancedMaps; }
        virtual void InitVisibilityDistance();

    private:

        InstanceMap* CreateInstance(uint32 InstanceId, InstanceSave *save, uint8 difficulty);
        BattleGroundMap* CreateBattleGround(uint32 InstanceId, BattleGround* bg);

        InstancedMaps m_InstancedMaps;

        Map* _FindMap(uint32 InstanceId) const
        {
            InstancedMaps::const_iterator i = m_InstancedMaps.find(InstanceId);
            return(i == m_InstancedMaps.end() ? NULL : i->second);
        }

        uint16 GridMapReference[MAX_NUMBER_OF_GRIDS][MAX_NUMBER_OF_GRIDS];
};
#endif

