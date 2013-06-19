/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Drakkisath
Complete(%): 100
Comment:
Category: Blackrock Spire
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_FIRENOVA                  23462
#define SPELL_CLEAVE                    20691
#define SPELL_CONFLIGURATION            16805
#define SPELL_THUNDERCLAP               15548               //Not sure if right ID. 23931 would be a harder possibility.

struct boss_drakkisathAI : public ScriptedAI
{
    boss_drakkisathAI(Creature* c) : ScriptedAI(c) {}

    uint32 FireNova_Timer;
    uint32 Cleave_Timer;
    uint32 Confliguration_Timer;
    uint32 Thunderclap_Timer;

    void Reset()
    {
        FireNova_Timer = 6000;
        Cleave_Timer = 8000;
        Confliguration_Timer = 15000;
        Thunderclap_Timer = 17000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //FireNova_Timer
        if (FireNova_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_FIRENOVA);
            FireNova_Timer = 10000;
        } else FireNova_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = 8000;
        } else Cleave_Timer -= diff;

        //Confliguration_Timer
        if (Confliguration_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CONFLIGURATION);
            Confliguration_Timer = 18000;
        } else Confliguration_Timer -= diff;

        //Thunderclap_Timer
        if (Thunderclap_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_THUNDERCLAP);
            Thunderclap_Timer = 20000;
        } else Thunderclap_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_drakkisath(Creature* pCreature)
{
    return new boss_drakkisathAI (pCreature);
}

void AddSC_boss_drakkisath()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_drakkisath";
    newscript->GetAI = &GetAI_boss_drakkisath;
    newscript->RegisterSelf();
}

