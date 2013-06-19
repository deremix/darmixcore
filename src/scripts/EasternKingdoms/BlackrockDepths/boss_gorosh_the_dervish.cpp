/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Gorosh_the_Dervish
Complete(%): 100
Comment:
Category: Blackrock Depths
EndScriptData */

#include "ScriptPCH.h"

enum Spells
{
    SPELL_WHIRLWIND                                        = 15589,
    SPELL_MORTALSTRIKE                                     = 24573
};

struct boss_gorosh_the_dervishAI : public ScriptedAI
{
    boss_gorosh_the_dervishAI(Creature* c) : ScriptedAI(c) {}

    uint32 WhirlWind_Timer;
    uint32 MortalStrike_Timer;

    void Reset()
    {
        WhirlWind_Timer = 12000;
        MortalStrike_Timer = 22000;
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
            DoCast(me, SPELL_WHIRLWIND);
            WhirlWind_Timer = 15000;
        } else WhirlWind_Timer -= diff;

        //MortalStrike_Timer
        if (MortalStrike_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_MORTALSTRIKE);
            MortalStrike_Timer = 15000;
        } else MortalStrike_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_gorosh_the_dervish(Creature* pCreature)
{
    return new boss_gorosh_the_dervishAI (pCreature);
}

void AddSC_boss_gorosh_the_dervish()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_gorosh_the_dervish";
    newscript->GetAI = &GetAI_boss_gorosh_the_dervish;
    newscript->RegisterSelf();
}
