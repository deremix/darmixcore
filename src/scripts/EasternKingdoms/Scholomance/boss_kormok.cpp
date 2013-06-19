/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Kormok
Complete(%): 100
Comment:
Category: Scholomance
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_SHADOWBOLTVOLLEY      20741
#define SPELL_BONESHIELD            27688

struct boss_kormokAI : public ScriptedAI
{
    boss_kormokAI(Creature* c) : ScriptedAI(c) {}

    uint32 ShadowVolley_Timer;
    uint32 BoneShield_Timer;
    uint32 Minion_Timer;
    uint32 Mage_Timer;
    bool Mages;

    void Reset()
    {
        ShadowVolley_Timer = 10000;
        BoneShield_Timer = 2000;
        Minion_Timer = 15000;
        Mage_Timer = 0;
        Mages = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void SummonMinions(Unit* victim)
    {
        if (Creature* SummonedMinion = DoSpawnCreature(16119, irand(-7,7), irand(-7,7), 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 120000))
            SummonedMinion->AI()->AttackStart(victim);
    }

    void SummonMages(Unit* victim)
    {
        if (Creature* SummonedMage = DoSpawnCreature(16120, irand(-9,9), irand(-9,9), 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 120000))
            SummonedMage->AI()->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //ShadowVolley_Timer
        if (ShadowVolley_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SHADOWBOLTVOLLEY);
            ShadowVolley_Timer = 15000;
        } else ShadowVolley_Timer -= diff;

        //BoneShield_Timer
        if (BoneShield_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_BONESHIELD);
            BoneShield_Timer = 45000;
        } else BoneShield_Timer -= diff;

        //Minion_Timer
        if (Minion_Timer <= diff)
        {
            //Cast
            SummonMinions(me->getVictim());
            SummonMinions(me->getVictim());
            SummonMinions(me->getVictim());
            SummonMinions(me->getVictim());

            Minion_Timer = 12000;
        } else Minion_Timer -= diff;

        //Summon 2 Bone Mages
        if (!Mages && me->GetHealth()*100 / me->GetMaxHealth() < 26)
        {
            //Cast
            SummonMages(me->getVictim());
            SummonMages(me->getVictim());
            Mages = true;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_kormok(Creature* pCreature)
{
    return new boss_kormokAI (pCreature);
}

void AddSC_boss_kormok()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_kormok";
    newscript->GetAI = &GetAI_boss_kormok;
    newscript->RegisterSelf();
}

