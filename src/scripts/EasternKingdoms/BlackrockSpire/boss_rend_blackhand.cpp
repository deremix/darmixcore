/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Rend_Blackhand
Complete(%): 100
Comment: Intro event NYI
Category: Blackrock Spire
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_WHIRLWIND                 26038
#define SPELL_CLEAVE                    20691
#define SPELL_THUNDERCLAP               23931               //Not sure if he cast this spell

struct boss_rend_blackhandAI : public ScriptedAI
{
    boss_rend_blackhandAI(Creature* c) : ScriptedAI(c) {}

    uint32 WhirlWind_Timer;
    uint32 Cleave_Timer;
    uint32 Thunderclap_Timer;

    void Reset()
    {
        WhirlWind_Timer = 20000;
        Cleave_Timer = 5000;
        Thunderclap_Timer = 9000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //WhirlWind_Timer
        if (WhirlWind_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_WHIRLWIND);
            WhirlWind_Timer = 18000;
        } else WhirlWind_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = 10000;
        } else Cleave_Timer -= diff;

        //Thunderclap_Timer
        if (Thunderclap_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_THUNDERCLAP);
            Thunderclap_Timer = 16000;
        } else Thunderclap_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_rend_blackhand(Creature* pCreature)
{
    return new boss_rend_blackhandAI (pCreature);
}

void AddSC_boss_rend_blackhand()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_rend_blackhand";
    newscript->GetAI = &GetAI_boss_rend_blackhand;
    newscript->RegisterSelf();
}

