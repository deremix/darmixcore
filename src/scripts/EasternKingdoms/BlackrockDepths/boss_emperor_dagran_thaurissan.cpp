/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Emperor_Dagran_Thaurissan
Complete(%): 99
Comment:
Category: Blackrock Depths
EndScriptData */

#include "ScriptPCH.h"

enum Yells
{
    SAY_AGGRO                                              = -1230001,
    SAY_SLAY                                               = -1230002
};

enum Spells
{
    SPELL_HANDOFTHAURISSAN                                 = 17492,
    SPELL_AVATAROFFLAME                                    = 15636
};

struct boss_draganthaurissanAI : public ScriptedAI
{
    boss_draganthaurissanAI(Creature* c) : ScriptedAI(c) {}

    uint32 HandOfThaurissan_Timer;
    uint32 AvatarOfFlame_Timer;
    //uint32 Counter;

    void Reset()
    {
        HandOfThaurissan_Timer = 4000;
        AvatarOfFlame_Timer = 25000;
        //Counter= 0;
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoScriptText(SAY_AGGRO, me);
        me->CallForHelp(VISIBLE_RANGE);
    }

    void KilledUnit(Unit* /*victim*/)
    {
        DoScriptText(SAY_SLAY, me);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        if (HandOfThaurissan_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, SPELL_HANDOFTHAURISSAN);

            //3 Hands of Thaurissan will be casted
            //if (Counter < 3)
            //{
            //    HandOfThaurissan_Timer = 1000;
            //    ++Counter;
            //}
            //else
            //{
                HandOfThaurissan_Timer = 5000;
                //Counter = 0;
            //}
        } else HandOfThaurissan_Timer -= diff;

        //AvatarOfFlame_Timer
        if (AvatarOfFlame_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_AVATAROFFLAME);
            AvatarOfFlame_Timer = 18000;
        } else AvatarOfFlame_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_draganthaurissan(Creature* pCreature)
{
    return new boss_draganthaurissanAI (pCreature);
}

void AddSC_boss_draganthaurissan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_emperor_dagran_thaurissan";
    newscript->GetAI = &GetAI_boss_draganthaurissan;
    newscript->RegisterSelf();
}
