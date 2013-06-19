/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Baron_Geddon
Complete(%): 100
Comment:
Category: Molten Core
EndScriptData */

#include "ScriptPCH.h"

#define EMOTE_SERVICE               -1409000

#define SPELL_INFERNO               19695
#define SPELL_IGNITEMANA            19659
#define SPELL_LIVINGBOMB            20475
#define SPELL_ARMAGEDDOM            20479

struct boss_baron_geddonAI : public ScriptedAI
{
    boss_baron_geddonAI(Creature* c) : ScriptedAI(c) {}

    uint32 Inferno_Timer;
    uint32 IgniteMana_Timer;
    uint32 LivingBomb_Timer;

    void Reset()
    {
        Inferno_Timer = 45000;                              //These times are probably wrong
        IgniteMana_Timer = 30000;
        LivingBomb_Timer = 35000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //If we are <2% hp cast Armageddom
        if (me->GetHealth()*100 / me->GetMaxHealth() <= 2)
        {
            me->InterruptNonMeleeSpells(true);
            DoCast(me, SPELL_ARMAGEDDOM);
            DoScriptText(EMOTE_SERVICE, me);
            return;
        }

        //Inferno_Timer
        if (Inferno_Timer <= diff)
        {
            DoCast(me, SPELL_INFERNO);
            Inferno_Timer = 45000;
        } else Inferno_Timer -= diff;

        //IgniteMana_Timer
        if (IgniteMana_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, SPELL_IGNITEMANA);

            IgniteMana_Timer = 30000;
        } else IgniteMana_Timer -= diff;

        //LivingBomb_Timer
        if (LivingBomb_Timer <= diff)
        {
           if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
               DoCast(pTarget, SPELL_LIVINGBOMB);

            LivingBomb_Timer = 35000;
        } else LivingBomb_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_baron_geddon(Creature* pCreature)
{
    return new boss_baron_geddonAI (pCreature);
}

void AddSC_boss_baron_geddon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_baron_geddon";
    newscript->GetAI = &GetAI_boss_baron_geddon;
    newscript->RegisterSelf();
}

