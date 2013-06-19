/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Ayamiss
Complete(%): 50
Comment: VERIFY SCRIPT
Category: Ruins of Ahn'Qiraj
EndScriptData */

#include "ScriptPCH.h"

/*
To do:
make him fly from 70-100%
*/

#define SPELL_STINGERSPRAY 25749
#define SPELL_POISONSTINGER 25748                           //only used in phase1
#define SPELL_SUMMONSWARMER 25844                           //might be 25708
// #define SPELL_PARALYZE 23414 doesnt work correct (core)

struct boss_ayamissAI : public ScriptedAI
{
    boss_ayamissAI(Creature* c) : ScriptedAI(c) {}

    Unit* pTarget;
    uint32 STINGERSPRAY_Timer;
    uint32 POISONSTINGER_Timer;
    uint32 SUMMONSWARMER_Timer;
    uint32 phase;

    void Reset()
    {
        pTarget = NULL;
        STINGERSPRAY_Timer = 30000;
        POISONSTINGER_Timer = 30000;
        SUMMONSWARMER_Timer = 60000;
        phase=1;
    }

    void EnterCombat(Unit* who)
    {
        pTarget = who;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //If he is 70% start phase 2
        if (phase == 1 && me->GetHealth()*100 / me->GetMaxHealth() <= 70 && !me->IsNonMeleeSpellCasted(false))
        {
            phase=2;
        }

        //STINGERSPRAY_Timer (only in phase2)
        if (phase == 2 && STINGERSPRAY_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_STINGERSPRAY);
            STINGERSPRAY_Timer = 30000;
        } else STINGERSPRAY_Timer -= diff;

        //POISONSTINGER_Timer (only in phase1)
        if (phase == 1 && POISONSTINGER_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_POISONSTINGER);
            POISONSTINGER_Timer = 30000;
        } else POISONSTINGER_Timer -= diff;

        //SUMMONSWARMER_Timer (only in phase1)
        if (SUMMONSWARMER_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_SUMMONSWARMER);
            SUMMONSWARMER_Timer = 60000;
        } else SUMMONSWARMER_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_ayamiss(Creature* pCreature)
{
    return new boss_ayamissAI (pCreature);
}

void AddSC_boss_ayamiss()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ayamiss";
    newscript->GetAI = &GetAI_boss_ayamiss;
    newscript->RegisterSelf();
}

