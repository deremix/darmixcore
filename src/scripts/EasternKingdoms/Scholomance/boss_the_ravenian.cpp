/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_the_ravenian
Complete(%): 100
Comment:
Category: Scholomance
EndScriptData */

#include "ScriptPCH.h"
#include "scholomance.h"

#define SPELL_TRAMPLE           15550
#define SPELL_CLEAVE            20691
#define SPELL_SUNDERINCLEAVE    25174
#define SPELL_KNOCKAWAY         10101

struct boss_theravenianAI : public ScriptedAI
{
    boss_theravenianAI(Creature* c) : ScriptedAI(c) {}

    uint32 Trample_Timer;
    uint32 Cleave_Timer;
    uint32 SunderingCleave_Timer;
    uint32 KnockAway_Timer;
    bool HasYelled;

    void Reset()
    {
        Trample_Timer = 24000;
        Cleave_Timer = 15000;
        SunderingCleave_Timer = 40000;
        KnockAway_Timer = 32000;
        HasYelled = false;
    }

    void JustDied(Unit* /*killer*/)
    {
        ScriptedInstance *pInstance = me->GetInstanceData();
        if (pInstance)
        {
            pInstance->SetData(DATA_THERAVENIAN_DEATH, 0);

            if (pInstance->GetData(TYPE_GANDLING) == IN_PROGRESS)
                me->SummonCreature(1853, 180.73f, -9.43856f, 75.507f, 1.61399f, TEMPSUMMON_DEAD_DESPAWN, 0);
        }
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Trample_Timer
        if (Trample_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_TRAMPLE);
            Trample_Timer = 10000;
        } else Trample_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = 7000;
        } else Cleave_Timer -= diff;

        //SunderingCleave_Timer
        if (SunderingCleave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SUNDERINCLEAVE);
            SunderingCleave_Timer = 20000;
        } else SunderingCleave_Timer -= diff;

        //KnockAway_Timer
        if (KnockAway_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_KNOCKAWAY);
            KnockAway_Timer = 12000;
        } else KnockAway_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_theravenian(Creature* pCreature)
{
    return new boss_theravenianAI (pCreature);
}

void AddSC_boss_theravenian()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_the_ravenian";
    newscript->GetAI = &GetAI_boss_theravenian;
    newscript->RegisterSelf();
}

