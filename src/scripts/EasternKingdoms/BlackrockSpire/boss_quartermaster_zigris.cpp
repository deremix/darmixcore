/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Quartmaster_Zigris
Complete(%): 100
Comment: Needs revision
Category: Blackrock Spire
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_SHOOT             16496
#define SPELL_STUNBOMB          16497
#define SPELL_HEALING_POTION    15504
#define SPELL_HOOKEDNET         15609

struct boss_quatermasterzigrisAI : public ScriptedAI
{
    boss_quatermasterzigrisAI(Creature* c) : ScriptedAI(c) {}

    uint32 Shoot_Timer;
    uint32 StunBomb_Timer;
    //uint32 HelingPotion_Timer;

    void Reset()
    {
        Shoot_Timer = 1000;
        StunBomb_Timer = 16000;
        //HelingPotion_Timer = 25000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //Shoot_Timer
        if (Shoot_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SHOOT);
            Shoot_Timer = 500;
        } else Shoot_Timer -= diff;

        //StunBomb_Timer
        if (StunBomb_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_STUNBOMB);
            StunBomb_Timer = 14000;
        } else StunBomb_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_quatermasterzigris(Creature* pCreature)
{
    return new boss_quatermasterzigrisAI (pCreature);
}

void AddSC_boss_quatermasterzigris()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "quartermaster_zigris";
    newscript->GetAI = &GetAI_boss_quatermasterzigris;
    newscript->RegisterSelf();
}

