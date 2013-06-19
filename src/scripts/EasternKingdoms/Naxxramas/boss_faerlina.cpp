/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Faerlina
Complete(%): 50
Comment: Without Mindcontrol boss cannot be defeated
Category: Naxxramas
EndScriptData */

#include "ScriptPCH.h"

#define SAY_GREET                   -1533009
#define SAY_AGGRO1                  -1533010
#define SAY_AGGRO2                  -1533011
#define SAY_AGGRO3                  -1533012
#define SAY_AGGRO4                  -1533013
#define SAY_SLAY1                   -1533014
#define SAY_SLAY2                   -1533015
#define SAY_DEATH                   -1533016

#define SPELL_POSIONBOLT_VOLLEY     28796
#define H_SPELL_POSIONBOLT_VOLLEY   54098
#define SPELL_ENRAGE                28798
#define H_SPELL_ENRAGE              54100
#define SPELL_RAINOFFIRE            28794                   //Not sure if targeted AoEs work if casted directly upon a player

struct boss_faerlinaAI : public ScriptedAI
{
    boss_faerlinaAI(Creature* c) : ScriptedAI(c) {}

    uint32 PoisonBoltVolley_Timer;
    uint32 RainOfFire_Timer;
    uint32 Enrage_Timer;
    bool HasTaunted;

    void Reset()
    {
        PoisonBoltVolley_Timer = 8000;
        RainOfFire_Timer = 16000;
        Enrage_Timer = 60000;
        HasTaunted = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
        switch (rand()%4)
        {
        case 0: DoScriptText(SAY_AGGRO1, me); break;
        case 1: DoScriptText(SAY_AGGRO2, me); break;
        case 2: DoScriptText(SAY_AGGRO3, me); break;
        case 3: DoScriptText(SAY_AGGRO4, me); break;
        }
    }

    void MoveInLineOfSight(Unit* who)
    {
         if (!HasTaunted && me->IsWithinDistInMap(who, 60.0f))
         {
                DoScriptText(SAY_GREET, me);
                HasTaunted = true;

        }
         ScriptedAI::MoveInLineOfSight(who);
    }

    void KilledUnit(Unit* /*victim*/)
    {
        switch (rand()%2)
        {
            case 0: DoScriptText(SAY_SLAY1, me); break;
            case 1: DoScriptText(SAY_SLAY2, me); break;
        }
    }

    void JustDied(Unit* /*Killer*/)
    {
        DoScriptText(SAY_DEATH, me);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //PoisonBoltVolley_Timer
        if (PoisonBoltVolley_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_POSIONBOLT_VOLLEY);
            PoisonBoltVolley_Timer = 11000;
        } else PoisonBoltVolley_Timer -= diff;

        //RainOfFire_Timer
        if (RainOfFire_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget,SPELL_RAINOFFIRE);
            RainOfFire_Timer = 16000;
        } else RainOfFire_Timer -= diff;

        //Enrage_Timer
        if (Enrage_Timer <= diff)
        {
            DoCast(me,SPELL_ENRAGE);
            Enrage_Timer = 61000;
        } else Enrage_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_faerlina(Creature* pCreature)
{
    return new boss_faerlinaAI (pCreature);
}

void AddSC_boss_faerlina()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_faerlina";
    newscript->GetAI = &GetAI_boss_faerlina;
    newscript->RegisterSelf();
}

