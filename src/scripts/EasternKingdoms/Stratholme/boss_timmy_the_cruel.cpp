/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: boss_timmy_the_cruel
Complete(%): 100
Comment:
Category: Stratholme
EndScriptData */

#include "ScriptPCH.h"

#define SAY_SPAWN   "TIMMY!"

#define SPELL_RAVENOUSCLAW    17470

struct boss_timmy_the_cruelAI : public ScriptedAI
{
    boss_timmy_the_cruelAI(Creature* c) : ScriptedAI(c) {}

    uint32 RavenousClaw_Timer;
    bool HasYelled;

    void Reset()
    {
        RavenousClaw_Timer = 10000;
        HasYelled = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
        if (!HasYelled)
        {
            me->MonsterYell(SAY_SPAWN,LANG_UNIVERSAL,0);
            HasYelled = true;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //RavenousClaw
        if (RavenousClaw_Timer <= diff)
        {
            //Cast
            DoCast(me->getVictim(), SPELL_RAVENOUSCLAW);
            //15 seconds until we should cast this again
            RavenousClaw_Timer = 15000;
        } else RavenousClaw_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_timmy_the_cruel(Creature* pCreature)
{
    return new boss_timmy_the_cruelAI (pCreature);
}

void AddSC_boss_timmy_the_cruel()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_timmy_the_cruel";
    newscript->GetAI = &GetAI_boss_timmy_the_cruel;
    newscript->RegisterSelf();
}

