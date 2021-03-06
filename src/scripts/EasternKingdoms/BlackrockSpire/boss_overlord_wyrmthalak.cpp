/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Overlord_Wyrmthalak
Complete(%): 100
Comment:
Category: Blackrock Spire
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_BLASTWAVE         11130
#define SPELL_SHOUT             23511
#define SPELL_CLEAVE            20691
#define SPELL_KNOCKAWAY         20686

#define ADD_1X -39.355381f
#define ADD_1Y -513.456482f
#define ADD_1Z 88.472046f
#define ADD_1O 4.679872f

#define ADD_2X -49.875881f
#define ADD_2Y -511.896942f
#define ADD_2Z 88.195160f
#define ADD_2O 4.613114f

struct boss_overlordwyrmthalakAI : public ScriptedAI
{
    boss_overlordwyrmthalakAI(Creature* c) : ScriptedAI(c) {}

    uint32 BlastWave_Timer;
    uint32 Shout_Timer;
    uint32 Cleave_Timer;
    uint32 Knockaway_Timer;
    bool Summoned;

    void Reset()
    {
        BlastWave_Timer = 20000;
        Shout_Timer = 2000;
        Cleave_Timer = 6000;
        Knockaway_Timer = 12000;
        Summoned = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //BlastWave_Timer
        if (BlastWave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_BLASTWAVE);
            BlastWave_Timer = 20000;
        } else BlastWave_Timer -= diff;

        //Shout_Timer
        if (Shout_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SHOUT);
            Shout_Timer = 10000;
        } else Shout_Timer -= diff;

        //Cleave_Timer
        if (Cleave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = 7000;
        } else Cleave_Timer -= diff;

        //Knockaway_Timer
        if (Knockaway_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_KNOCKAWAY);
            Knockaway_Timer = 14000;
        } else Knockaway_Timer -= diff;

        //Summon two Beserks
        if (!Summoned && me->GetHealth()*100 / me->GetMaxHealth() < 51)
        {
            Unit* pTarget = SelectTarget(SELECT_TARGET_RANDOM,0, 100, true);

            if (Creature* SummonedCreature = me->SummonCreature(9216,ADD_1X,ADD_1Y,ADD_1Z,ADD_1O,TEMPSUMMON_TIMED_DESPAWN,300000))
                SummonedCreature->AI()->AttackStart(pTarget);
            if (Creature* SummonedCreature = me->SummonCreature(9268,ADD_2X,ADD_2Y,ADD_2Z,ADD_2O,TEMPSUMMON_TIMED_DESPAWN,300000))
                SummonedCreature->AI()->AttackStart(pTarget);
            Summoned = true;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_overlordwyrmthalak(Creature* pCreature)
{
    return new boss_overlordwyrmthalakAI (pCreature);
}

void AddSC_boss_overlordwyrmthalak()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_overlord_wyrmthalak";
    newscript->GetAI = &GetAI_boss_overlordwyrmthalak;
    newscript->RegisterSelf();
}

