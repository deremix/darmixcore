/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Instance_Mechanar
Complete(%): 100
Comment:
Category: Mechanar
EndScriptData */

#include "ScriptPCH.h"
#include "mechanar.h"

#define ENCOUNTERS      1

struct instance_mechanar : public ScriptedInstance
{
    instance_mechanar(Map *map) : ScriptedInstance(map) {Initialize();};


    uint32 Encounters[ENCOUNTERS];

    void OnCreatureCreate (Creature* /*creature*/, uint32 /*creature_entry*/)
    {
    }

    void Initialize()
    {
        for (uint8 i = 0; i < ENCOUNTERS; ++i)
            Encounters[i] = NOT_STARTED;
    }

    bool IsEncounterInProgress() const
    {
        for (uint8 i = 0; i < ENCOUNTERS; ++i)
            if (Encounters[i] == IN_PROGRESS)
                return true;

        return false;
    }

    uint32 GetData(uint32 type)
    {
        switch (type)
        {
        case DATA_NETHERMANCER_EVENT:   return Encounters[0];
        }

        return false;
    }

    uint64 GetData64 (uint32 /*identifier*/)
    {
        return 0;
    }

    void SetData(uint32 type, uint32 data)
    {
        switch (type)
        {
        case DATA_NETHERMANCER_EVENT:   Encounters[0] = data;   break;
        }
    }
};

InstanceData* GetInstanceData_instance_mechanar(Map* map)
{
    return new instance_mechanar(map);
}

void AddSC_instance_mechanar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_mechanar";
    newscript->GetInstanceData = &GetInstanceData_instance_mechanar;
    newscript->RegisterSelf();
}

