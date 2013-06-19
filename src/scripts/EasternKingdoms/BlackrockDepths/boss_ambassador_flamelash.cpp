/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Ambassador_Flamelash
Complete(%): 100
Comment:
Category: Blackrock Depths
EndScriptData */

#include "ScriptPCH.h"

enum Spells
{
    SPELL_FIREBLAST                                        = 15573
};

struct boss_ambassador_flamelashAI : public ScriptedAI
{
    boss_ambassador_flamelashAI(Creature* c) : ScriptedAI(c) {}

    uint32 FireBlast_Timer;
    uint32 Spirit_Timer;

    void Reset()
    {
        FireBlast_Timer = 2000;
        Spirit_Timer = 24000;
    }

    void EnterCombat(Unit* /*who*/) {}

    void SummonSpirits(Unit* victim)
    {
        if (Creature* Spirit = DoSpawnCreature(9178, irand(-9,9), irand(-9,9), 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 60000))
            Spirit->AI()->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //FireBlast_Timer
        if (FireBlast_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_FIREBLAST);
            FireBlast_Timer = 7000;
        } else FireBlast_Timer -= diff;

        //Spirit_Timer
        if (Spirit_Timer <= diff)
        {
            SummonSpirits(me->getVictim());
            SummonSpirits(me->getVictim());
            SummonSpirits(me->getVictim());
            SummonSpirits(me->getVictim());

            Spirit_Timer = 30000;
        } else Spirit_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_ambassador_flamelash(Creature* pCreature)
{
    return new boss_ambassador_flamelashAI (pCreature);
}

void AddSC_boss_ambassador_flamelash()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ambassador_flamelash";
    newscript->GetAI = &GetAI_boss_ambassador_flamelash;
    newscript->RegisterSelf();
}
