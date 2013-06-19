/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Hazzarah
Complete(%): 100
Comment:
Category: Zul'Gurub
EndScriptData */

#include "ScriptPCH.h"
#include "zulgurub.h"

#define SPELL_MANABURN         26046
#define SPELL_SLEEP            24664

struct boss_hazzarahAI : public ScriptedAI
{
    boss_hazzarahAI(Creature* c) : ScriptedAI(c) {}

    uint32 ManaBurn_Timer;
    uint32 Sleep_Timer;
    uint32 Illusions_Timer;

    void Reset()
    {
        ManaBurn_Timer = 4000 + rand()%6000;
        Sleep_Timer = 10000 + rand()%8000;
        Illusions_Timer = 10000 + rand()%8000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //ManaBurn_Timer
        if (ManaBurn_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_MANABURN);
            ManaBurn_Timer = 8000 + rand()%8000;
        } else ManaBurn_Timer -= diff;

        //Sleep_Timer
        if (Sleep_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SLEEP);
            Sleep_Timer = 12000 + rand()%8000;
        } else Sleep_Timer -= diff;

        //Illusions_Timer
        if (Illusions_Timer <= diff)
        {
            //We will summon 3 illusions that will spawn on a random gamer and attack this gamer
            //We will just use one model for the beginning
            Unit* pTarget = NULL;
            for (uint8 i = 0; i < 3; ++i)
            {
                pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
                if (!pTarget)
                    return;

                Creature* Illusion = me->SummonCreature(15163,pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(),0,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,30000);
                if (Illusion)
                    Illusion->AI()->AttackStart(pTarget);
            }

            Illusions_Timer = 15000 + rand()%10000;
        } else Illusions_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_hazzarah(Creature* pCreature)
{
    return new boss_hazzarahAI (pCreature);
}

void AddSC_boss_hazzarah()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_hazzarah";
    newscript->GetAI = &GetAI_boss_hazzarah;
    newscript->RegisterSelf();
}

