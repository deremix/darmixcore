/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Razorgore
Complete(%): 50
Comment: Needs additional review. Phase 1 NYI (Grethok the Controller)
Category: Blackwing Lair
EndScriptData */

#include "ScriptPCH.h"

//Razorgore Phase 2 Script

#define SAY_EGGS_BROKEN1        -1469022
#define SAY_EGGS_BROKEN2        -1469023
#define SAY_EGGS_BROKEN3        -1469024
#define SAY_DEATH               -1469025

#define SPELL_CLEAVE            22540
#define SPELL_WARSTOMP          24375
#define SPELL_FIREBALLVOLLEY    22425
#define SPELL_CONFLAGRATION     23023

struct boss_razorgoreAI : public ScriptedAI
{
    boss_razorgoreAI(Creature* c) : ScriptedAI(c) {}

    uint32 Cleave_Timer;
    uint32 WarStomp_Timer;
    uint32 FireballVolley_Timer;
    uint32 Conflagration_Timer;

    void Reset()
    {
        Cleave_Timer = 15000;                               //These times are probably wrong
        WarStomp_Timer = 35000;
        FireballVolley_Timer = 7000;
        Conflagration_Timer = 12000;
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoZoneInCombat();
    }

    void JustDied(Unit* /*Killer*/)
    {
        DoScriptText(SAY_DEATH, me);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Cleave_Timer
        if (Cleave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = urand(7000,10000);
        } else Cleave_Timer -= diff;

        //WarStomp_Timer
        if (WarStomp_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_WARSTOMP);
            WarStomp_Timer = urand(15000,25000);
        } else WarStomp_Timer -= diff;

        //FireballVolley_Timer
        if (FireballVolley_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_FIREBALLVOLLEY);
            FireballVolley_Timer = urand(12000,15000);
        } else FireballVolley_Timer -= diff;

        //Conflagration_Timer
        if (Conflagration_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CONFLAGRATION);
            //We will remove this threat reduction and add an aura check.

            //if (DoGetThreat(me->getVictim()))
            //DoModifyThreatPercent(me->getVictim(),-50);

            Conflagration_Timer = 12000;
        } else Conflagration_Timer -= diff;

        // Aura Check. If the gamer is affected by confliguration we attack a random gamer.
        if (me->getVictim() && me->getVictim()->HasAura(SPELL_CONFLAGRATION, 0))
            if (Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM, 1, 100, true))
                me->TauntApply(pTarget);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_razorgore(Creature* pCreature)
{
    return new boss_razorgoreAI (pCreature);
}

void AddSC_boss_razorgore()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_razorgore";
    newscript->GetAI = &GetAI_boss_razorgore;
    newscript->RegisterSelf();
}

