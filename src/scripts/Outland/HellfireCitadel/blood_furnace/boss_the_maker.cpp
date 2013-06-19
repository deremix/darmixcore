/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_The_Maker
Complete(%): 99
Comment:
Category: Hellfire Citadel, Blood Furnace
EndScriptData */

#include "ScriptPCH.h"
#include "blood_furnace.h"

#define SAY_AGGRO_1                 -1542009
#define SAY_AGGRO_2                 -1542010
#define SAY_AGGRO_3                 -1542011
#define SAY_KILL_1                  -1542012
#define SAY_KILL_2                  -1542013
#define SAY_DIE                     -1542014

#define SPELL_EXPLODING_BREAKER     30925
#define SPELL_EXPLODING_BREAKER_H   40059
#define SPELL_DOMINATION            30923

struct boss_the_makerAI : public ScriptedAI
{
    boss_the_makerAI(Creature* c) : ScriptedAI(c) 
	{
            pInstance = c->GetInstanceData();
            HeroicMode = me->GetMap()->IsHeroic();
	}

    ScriptedInstance* pInstance;
    bool HeroicMode;

    uint32 ExplodingBreaker_Timer;
    uint32 Domination_Timer;

    void Reset()
    {
        ExplodingBreaker_Timer = 9000;
        Domination_Timer = 60000;
        if (pInstance)
            pInstance->SetData(DATA_MAKEREVENT, NOT_STARTED);
    }

    void EnterCombat(Unit* /*who*/)
    {
        switch (rand()%3)
        {
            case 0: DoScriptText(SAY_AGGRO_1, me); break;
            case 1: DoScriptText(SAY_AGGRO_2, me); break;
            case 2: DoScriptText(SAY_AGGRO_3, me); break;
        }

        if (pInstance)
            pInstance->SetData(DATA_MAKEREVENT, IN_PROGRESS);
    }

    void KilledUnit(Unit* /*victim*/)
    {
        switch (rand()%2)
        {
            case 0: DoScriptText(SAY_KILL_1, me); break;
            case 1: DoScriptText(SAY_KILL_2, me); break;
        }
    }

    void JustDied(Unit* /*Killer*/)
    {
        DoScriptText(SAY_DIE, me);

        if (pInstance)
            pInstance->SetData(DATA_MAKEREVENT, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (ExplodingBreaker_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget,HeroicMode ? SPELL_EXPLODING_BREAKER_H : SPELL_EXPLODING_BREAKER);
            ExplodingBreaker_Timer = 9000+rand()%2000;
        }
        else ExplodingBreaker_Timer -=diff;

        if (Domination_Timer <= diff)
        {
            Unit* pTarget;
            pTarget = SelectUnit(SELECT_TARGET_RANDOM,0);
            DoCast(pTarget,SPELL_DOMINATION);
            Domination_Timer = 60000;
        }
        else Domination_Timer -=diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_the_makerAI(Creature* pCreature)
{
    return new boss_the_makerAI (pCreature);
}

void AddSC_boss_the_maker()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_the_maker";
    newscript->GetAI = &GetAI_boss_the_makerAI;
    newscript->RegisterSelf();
}

