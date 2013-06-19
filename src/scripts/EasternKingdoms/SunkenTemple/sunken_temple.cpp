/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Sunken_Temple
Complete(%): 100
Comment: Area Trigger + Puzzle event support
Category: Sunken Temple
EndScriptData */

/* ContentData
at_malfurion_Stormrage_trigger
EndContentData */

#include "ScriptPCH.h"
#include "sunken_temple.h"

/*#####
# at_malfurion_Stormrage_trigger
#####*/

bool AreaTrigger_at_malfurion_stormrage(Player* pPlayer, const AreaTriggerEntry* /*at*/)
{
    if (ScriptedInstance* pInstance = pPlayer->GetInstanceData())
    {
        if (!pPlayer->FindNearestCreature(15362,15))
            pPlayer->SummonCreature(15362, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), -1.52, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 100000);
        return false;
    }
return false;
}
/*#####
# go_atalai_statue
#####*/

bool GOHello_go_atalai_statue(Player* pPlayer, GameObject* pGo)
{
    ScriptedInstance* pInstance = pPlayer->GetInstanceData();
    if (!pInstance)
        return false;
    pInstance->SetData(EVENT_STATE,pGo->GetEntry());
    return false;
}

void AddSC_sunken_temple()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "at_malfurion_stormrage";
    newscript->pAreaTrigger = &AreaTrigger_at_malfurion_stormrage;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_atalai_statue";
    newscript->pGOHello = &GOHello_go_atalai_statue;
    newscript->RegisterSelf();
}
