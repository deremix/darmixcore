/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Lucifron
Complete(%): 100
Comment:
Category: Molten Core
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_IMPENDINGDOOM 19702
#define SPELL_LUCIFRONCURSE 19703
#define SPELL_SHADOWSHOCK   20603

struct boss_lucifronAI : public ScriptedAI
{
    boss_lucifronAI(Creature* c) : ScriptedAI(c) {}

    uint32 ImpendingDoom_Timer;
    uint32 LucifronCurse_Timer;
    uint32 ShadowShock_Timer;

    void Reset()
    {
        ImpendingDoom_Timer = 10000;                        //Initial cast after 10 seconds so the debuffs alternate
        LucifronCurse_Timer = 20000;                        //Initial cast after 20 seconds
        ShadowShock_Timer = 6000;                           //6 seconds
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Impending doom timer
        if (ImpendingDoom_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_IMPENDINGDOOM);
            ImpendingDoom_Timer = 20000;
        } else ImpendingDoom_Timer -= diff;

        //Lucifron's curse timer
        if (LucifronCurse_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_LUCIFRONCURSE);
            LucifronCurse_Timer = 15000;
        } else LucifronCurse_Timer -= diff;

        //Shadowshock
        if (ShadowShock_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SHADOWSHOCK);
            ShadowShock_Timer = 6000;
        } else ShadowShock_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_lucifron(Creature* pCreature)
{
    return new boss_lucifronAI (pCreature);
}

void AddSC_boss_lucifron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_lucifron";
    newscript->GetAI = &GetAI_boss_lucifron;
    newscript->RegisterSelf();
}

