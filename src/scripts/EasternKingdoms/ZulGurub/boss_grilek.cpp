/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Grilek
Complete(%): 100
Comment:
Category: Zul'Gurub
EndScriptData */

#include "ScriptPCH.h"
#include "zulgurub.h"

#define SPELL_AVARTAR                24646                  //The Enrage Spell
#define SPELL_GROUNDTREMOR            6524

struct boss_grilekAI : public ScriptedAI
{
    boss_grilekAI(Creature* c) : ScriptedAI(c) {}

    uint32 Avartar_Timer;
    uint32 GroundTremor_Timer;

    void Reset()
    {
        Avartar_Timer = 15000 + rand()%10000;
        GroundTremor_Timer = 8000 + rand()%8000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //Avartar_Timer
        if (Avartar_Timer <= diff)
        {

            DoCast(me, SPELL_AVARTAR);
            Unit* pTarget = NULL;

            pTarget = SelectUnit(SELECT_TARGET_RANDOM,1);

            if (DoGetThreat(me->getVictim()))
                DoModifyThreatPercent(me->getVictim(),-50);
            if (pTarget)
                AttackStart(pTarget);

            Avartar_Timer = 25000 + rand()%10000;
        } else Avartar_Timer -= diff;

        //GroundTremor_Timer
        if (GroundTremor_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_GROUNDTREMOR);
            GroundTremor_Timer = 12000 + rand()%4000;
        } else GroundTremor_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_grilek(Creature* pCreature)
{
    return new boss_grilekAI (pCreature);
}

void AddSC_boss_grilek()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_grilek";
    newscript->GetAI = &GetAI_boss_grilek;
    newscript->RegisterSelf();
}

