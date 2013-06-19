/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_General_Angerforge
Complete(%): 100
Comment:
Category: Blackrock Depths
EndScriptData */

#include "ScriptPCH.h"

enum Spells
{
    SPELL_MIGHTYBLOW                                       = 14099,
    SPELL_HAMSTRING                                        = 9080,
    SPELL_CLEAVE                                           = 20691
};

struct boss_general_angerforgeAI : public ScriptedAI
{
    boss_general_angerforgeAI(Creature* c) : ScriptedAI(c) {}

    uint32 MightyBlow_Timer;
    uint32 HamString_Timer;
    uint32 Cleave_Timer;
    uint32 Adds_Timer;
    bool Medics;

    void Reset()
    {
        MightyBlow_Timer = 8000;
        HamString_Timer = 12000;
        Cleave_Timer = 16000;
        Adds_Timer = 0;
        Medics = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void SummonAdds(Unit* victim)
    {
        if (Creature* SummonedAdd = DoSpawnCreature(8901, irand(-14,14), irand(-14,14), 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 120000))
            SummonedAdd->AI()->AttackStart(victim);
    }

    void SummonMedics(Unit* victim)
    {
        if (Creature* SummonedMedic = DoSpawnCreature(8894, irand(-9,9), irand(-9,9), 0, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 120000))
            SummonedMedic->AI()->AttackStart(victim);
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //MightyBlow_Timer
        if (MightyBlow_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_MIGHTYBLOW);
            MightyBlow_Timer = 18000;
        } else MightyBlow_Timer -= diff;

        //HamString_Timer
        if (HamString_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_HAMSTRING);
            HamString_Timer = 15000;
        } else HamString_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = 9000;
        } else Cleave_Timer -= diff;

        //Adds_Timer
        if (me->GetHealth()*100 / me->GetMaxHealth() < 21)
        {
            if (Adds_Timer <= diff)
            {
                // summon 3 Adds every 25s
                SummonAdds(me->getVictim());
                SummonAdds(me->getVictim());
                SummonAdds(me->getVictim());

                Adds_Timer = 25000;
            } else Adds_Timer -= diff;
        }

        //Summon Medics
        if (!Medics && me->GetHealth()*100 / me->GetMaxHealth() < 21)
        {
            SummonMedics(me->getVictim());
            SummonMedics(me->getVictim());
            Medics = true;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_general_angerforge(Creature* pCreature)
{
    return new boss_general_angerforgeAI (pCreature);
}

void AddSC_boss_general_angerforge()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_general_angerforge";
    newscript->GetAI = &GetAI_boss_general_angerforge;
    newscript->RegisterSelf();
}
