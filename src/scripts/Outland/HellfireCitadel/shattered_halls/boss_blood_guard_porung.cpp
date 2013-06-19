/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Blood_Guard_Porung
Complete(%): 99
Comment:
Category: Hellfire Citadel, Shattered Halls
EndScriptData */

/* ContentData
boss_blood_guard_porung
EndContentData */

#include "ScriptPCH.h"
#include "shattered_halls.h"

#define SPELL_CLEAVE    15496

struct boss_blood_guard_porungAI : public ScriptedAI
{
    boss_blood_guard_porungAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
        HeroicMode = me->GetMap()->IsHeroic();
    }

    ScriptedInstance* pInstance;
    bool Heroic;

    uint32 Cleave_Timer;

    void Reset()
    {
        Cleave_Timer = 15000;

        if (pInstance)
            pInstance->SetData(DATA_PORUNG, NOT_STARTED);
    }

    void EnterCombat(Unit* /*who*/)
    {
        if (pInstance)
            pInstance->SetData(DATA_PORUNG, IN_PROGRESS);
    }

    void JustDied(Unit* /*Killer*/)
    {
        if (pInstance)
            pInstance->SetData(DATA_PORUNG, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
           return;

        if (Cleave_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_CLEAVE);
            Cleave_Timer = 10000+rand()%5000;
        } else Cleave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_blood_guard_porungAI(Creature* pCreature)
{
    return new boss_blood_guard_porungAI (pCreature);
}

void AddSC_boss_blood_guard_porung()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_blood_guard_porung";
    newscript->GetAI = &GetAI_boss_blood_guard_porungAI;
    newscript->RegisterSelf();
}
