/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Grizzle
Complete(%): 100
Comment:
Category: Blackrock Depths
EndScriptData */

#include "ScriptPCH.h"

enum Yells
{
    EMOTE_GENERIC_FRENZY_KILL                              = -1000001
};

enum Spells
{
    SPELL_GROUNDTREMOR                                     = 6524,
    SPELL_FRENZY                                           = 28371
};

struct boss_grizzleAI : public ScriptedAI
{
    boss_grizzleAI(Creature* c) : ScriptedAI(c) {}

    uint32 GroundTremor_Timer;
    uint32 Frenzy_Timer;

    void Reset()
    {
        GroundTremor_Timer = 12000;
        Frenzy_Timer =0;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //GroundTremor_Timer
        if (GroundTremor_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_GROUNDTREMOR);
            GroundTremor_Timer = 8000;
        } else GroundTremor_Timer -= diff;

        //Frenzy_Timer
        if (me->GetHealth()*100 / me->GetMaxHealth() < 51)
        {
            if (Frenzy_Timer <= diff)
            {
                DoCast(me, SPELL_FRENZY);
                DoScriptText(EMOTE_GENERIC_FRENZY_KILL, me);

                Frenzy_Timer = 15000;
            } else Frenzy_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_grizzle(Creature* pCreature)
{
    return new boss_grizzleAI (pCreature);
}

void AddSC_boss_grizzle()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_grizzle";
    newscript->GetAI = &GetAI_boss_grizzle;
    newscript->RegisterSelf();
}
