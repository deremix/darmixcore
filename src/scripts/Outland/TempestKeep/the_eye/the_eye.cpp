/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: The_Eye
Complete(%): 100
Comment:
Category: Tempest Keep, The Eye
EndScriptData */

/* ContentData
mob_crystalcore_devastator
EndContentData */

#include "ScriptPCH.h"
#include "the_eye.h"

#define SPELL_COUNTERCHARGE     35035
#define SPELL_KNOCKAWAY         22893

struct mob_crystalcore_devastatorAI : public ScriptedAI
{
    mob_crystalcore_devastatorAI(Creature* c) : ScriptedAI(c) {}

    uint32 Knockaway_Timer;
    uint32 Countercharge_Timer;

    void Reset()
    {
        Countercharge_Timer = 9000;
        Knockaway_Timer = 25000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Check if we have a current target
        //Knockaway_Timer
        if (Knockaway_Timer <= diff)
        {
            me->CastSpell(me->getVictim(),SPELL_KNOCKAWAY, true);

            // current aggro target is knocked away pick new target
            Unit* pTarget = SelectUnit(SELECT_TARGET_TOPAGGRO, 0);

            if (!pTarget || pTarget == me->getVictim())
                pTarget = SelectUnit(SELECT_TARGET_TOPAGGRO, 1);

            if (pTarget)
                me->TauntApply(pTarget);

            Knockaway_Timer = 23000;
        }
        else Knockaway_Timer -= diff;

        //Countercharge_Timer
        if (Countercharge_Timer <= diff)
        {
            DoCast(this->me,SPELL_COUNTERCHARGE);
            Countercharge_Timer = 45000;
        } else Countercharge_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_crystalcore_devastator(Creature* pCreature)
{
    return new mob_crystalcore_devastatorAI (pCreature);
}

void AddSC_the_eye()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "mob_crystalcore_devastator";
    newscript->GetAI = &GetAI_mob_crystalcore_devastator;
    newscript->RegisterSelf();
}

