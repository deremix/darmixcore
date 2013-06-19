/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Pyroguard_Emberseer
Complete(%): 100
Comment: Event to activate Emberseer NYI
Category: Blackrock Spire
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_FIRENOVA          23462
#define SPELL_FLAMEBUFFET       23341
#define SPELL_PYROBLAST         17274

struct boss_pyroguard_emberseerAI : public ScriptedAI
{
    boss_pyroguard_emberseerAI(Creature* c) : ScriptedAI(c) {}

    uint32 FireNova_Timer;
    uint32 FlameBuffet_Timer;
    uint32 PyroBlast_Timer;

    void Reset()
    {
        FireNova_Timer = 6000;
        FlameBuffet_Timer = 3000;
        PyroBlast_Timer = 14000;
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
            FireNova_Timer = 6000;
        } else FireNova_Timer -= diff;

        //FlameBuffet_Timer
        if (FlameBuffet_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_FLAMEBUFFET);
            FlameBuffet_Timer = 14000;
        } else FlameBuffet_Timer -= diff;

        //PyroBlast_Timer
        if (PyroBlast_Timer <= diff)
        {
            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                DoCast(pTarget, SPELL_PYROBLAST);
            PyroBlast_Timer = 15000;
        } else PyroBlast_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_pyroguard_emberseer(Creature* pCreature)
{
    return new boss_pyroguard_emberseerAI (pCreature);
}

void AddSC_boss_pyroguard_emberseer()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_pyroguard_emberseer";
    newscript->GetAI = &GetAI_boss_pyroguard_emberseer;
    newscript->RegisterSelf();
}

