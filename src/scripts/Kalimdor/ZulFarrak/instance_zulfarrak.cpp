/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "ScriptPCH.h"

#define NPC_GAHZRILLA 7273

struct instance_zulfarrak : public ScriptedInstance
{
    instance_zulfarrak(Map* pMap) : ScriptedInstance(pMap) {Initialize();}

    uint32 GahzRillaEncounter;

    void Initialize()
    {
        GahzRillaEncounter = NOT_STARTED;
    }

    void OnCreatureCreate(Creature* pCreature, bool /*add*/)
    {
        if (pCreature->GetEntry() == NPC_GAHZRILLA)
        {
            if (GahzRillaEncounter >= IN_PROGRESS)
                pCreature->DisappearAndDie();
            else
                GahzRillaEncounter = IN_PROGRESS;
        }
    }
};

InstanceData* GetInstanceData_instance_zulfarrak(Map* pMap)
{
    return new instance_zulfarrak(pMap);
}

void AddSC_instance_zulfarrak()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_zulfarrak";
    newscript->GetInstanceData = &GetInstanceData_instance_zulfarrak;
    newscript->RegisterSelf();
}
