/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Princess_Theradras
Complete(%): 100
Comment:
Category: Maraudon
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_DUSTFIELD             21909
#define SPELL_BOULDER               21832
#define SPELL_THRASH                3391
#define SPELL_REPULSIVEGAZE         21869

struct boss_ptheradrasAI : public ScriptedAI
{
    boss_ptheradrasAI(Creature* c) : ScriptedAI(c) {}

    uint32 Dustfield_Timer;
    uint32 Boulder_Timer;
    uint32 Thrash_Timer;
    uint32 RepulsiveGaze_Timer;

    void Reset()
    {
        Dustfield_Timer = 8000;
        Boulder_Timer = 2000;
        Thrash_Timer = 5000;
        RepulsiveGaze_Timer = 23000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void JustDied(Unit* /*killer*/)
    {
        me->SummonCreature(12238,28.067f,61.875f,-123.405f,4.67f,TEMPSUMMON_TIMED_DESPAWN,600000);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Dustfield_Timer
        if (Dustfield_Timer <= diff)
        {
            DoCast(me, SPELL_DUSTFIELD);
            Dustfield_Timer = 14000;
        } else Dustfield_Timer -= diff;

        //Boulder_Timer
        if (Boulder_Timer <= diff)
        {
            Unit* pTarget = NULL;
            pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (pTarget)
                DoCast(pTarget, SPELL_BOULDER);
            Boulder_Timer = 10000;
        } else Boulder_Timer -= diff;

        //RepulsiveGaze_Timer
        if (RepulsiveGaze_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_REPULSIVEGAZE);
            RepulsiveGaze_Timer = 20000;
        } else RepulsiveGaze_Timer -= diff;

        //Thrash_Timer
        if (Thrash_Timer <= diff)
        {
            DoCast(me, SPELL_THRASH);
            Thrash_Timer = 18000;
        } else Thrash_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_ptheradras(Creature* pCreature)
{
    return new boss_ptheradrasAI (pCreature);
}

void AddSC_boss_ptheradras()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_princess_theradras";
    newscript->GetAI = &GetAI_boss_ptheradras;
    newscript->RegisterSelf();
}

