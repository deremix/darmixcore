/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_The_Best
Complete(%): 100
Comment:
Category: Blackrock Spire
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_FLAMEBREAK            16785
#define SPELL_IMMOLATE              20294
#define SPELL_TERRIFYINGROAR        14100

struct boss_thebeastAI : public ScriptedAI
{
    boss_thebeastAI(Creature* c) : ScriptedAI(c) {}

    uint32 Flamebreak_Timer;
    uint32 Immolate_Timer;
    uint32 TerrifyingRoar_Timer;

    void Reset()
    {
        Flamebreak_Timer = 12000;
        Immolate_Timer = 3000;
        TerrifyingRoar_Timer = 23000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //Flamebreak_Timer
        if (Flamebreak_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_FLAMEBREAK);
            Flamebreak_Timer = 10000;
        } else Flamebreak_Timer -= diff;

        //Immolate_Timer
        if (Immolate_Timer <= diff)
        {
            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                DoCast(pTarget, SPELL_IMMOLATE);
            Immolate_Timer = 8000;
        } else Immolate_Timer -= diff;

        //TerrifyingRoar_Timer
        if (TerrifyingRoar_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_TERRIFYINGROAR);
            TerrifyingRoar_Timer = 20000;
        } else TerrifyingRoar_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_thebeast(Creature* pCreature)
{
    return new boss_thebeastAI (pCreature);
}

void AddSC_boss_thebeast()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_the_beast";
    newscript->GetAI = &GetAI_boss_thebeast;
    newscript->RegisterSelf();
}

