/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Mother_Smolderweb
Complete(%): 100
Comment: Uncertain how often mother's milk is casted
Category: Blackrock Spire
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_CRYSTALIZE                16104
#define SPELL_MOTHERSMILK               16468
#define SPELL_SUMMON_SPIRE_SPIDERLING   16103

struct boss_mothersmolderwebAI : public ScriptedAI
{
    boss_mothersmolderwebAI(Creature* c) : ScriptedAI(c) {}

    uint32 Crystalize_Timer;
    uint32 MothersMilk_Timer;

    void Reset()
    {
        Crystalize_Timer = 20000;
        MothersMilk_Timer = 10000;
    }

    void EnterCombat(Unit* /*who*/) {}

    void DamageTaken(Unit* /*done_by*/, uint32 &damage)
    {
        if (me->GetHealth() <= damage)
            DoCast(me, SPELL_SUMMON_SPIRE_SPIDERLING, true);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //Crystalize_Timer
        if (Crystalize_Timer <= diff)
        {
            DoCast(me, SPELL_CRYSTALIZE);
            Crystalize_Timer = 15000;
        } else Crystalize_Timer -= diff;

        //MothersMilk_Timer
        if (MothersMilk_Timer <= diff)
        {
            DoCast(me, SPELL_MOTHERSMILK);
            MothersMilk_Timer = urand(5000,12500);
        } else MothersMilk_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_mothersmolderweb(Creature* pCreature)
{
    return new boss_mothersmolderwebAI (pCreature);
}

void AddSC_boss_mothersmolderweb()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_mother_smolderweb";
    newscript->GetAI = &GetAI_boss_mothersmolderweb;
    newscript->RegisterSelf();
}

