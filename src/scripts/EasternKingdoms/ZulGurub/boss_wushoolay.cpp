/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Wushoolay
Complete(%): 100
Comment:
Category: Zul'Gurub
EndScriptData */

#include "ScriptPCH.h"
#include "zulgurub.h"

#define SPELL_LIGHTNINGCLOUD         25033
#define SPELL_LIGHTNINGWAVE          24819

struct boss_wushoolayAI : public ScriptedAI
{
    boss_wushoolayAI(Creature* c) : ScriptedAI(c) {}

    uint32 LightningCloud_Timer;
    uint32 LightningWave_Timer;

    void Reset()
    {
        LightningCloud_Timer = 5000 + rand()%5000;
        LightningWave_Timer = 8000 + rand()%8000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //LightningCloud_Timer
        if (LightningCloud_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_LIGHTNINGCLOUD);
            LightningCloud_Timer = 15000 + rand()%5000;
        } else LightningCloud_Timer -= diff;

        //LightningWave_Timer
        if (LightningWave_Timer <= diff)
        {
            Unit* pTarget = NULL;
            pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (pTarget) DoCast(pTarget, SPELL_LIGHTNINGWAVE);

            LightningWave_Timer = 12000 + rand()%4000;
        } else LightningWave_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_wushoolay(Creature* pCreature)
{
    return new boss_wushoolayAI (pCreature);
}

void AddSC_boss_wushoolay()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_wushoolay";
    newscript->GetAI = &GetAI_boss_wushoolay;
    newscript->RegisterSelf();
}

