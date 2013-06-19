/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Gahz'ranka
Complete(%): 85
Comment: Massive Geyser with knockback not working. Spell buggy.
Category: Zul'Gurub
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_FROSTBREATH            16099
#define SPELL_MASSIVEGEYSER          22421                  //Not working. Cause its a summon...
#define SPELL_SLAM                   24326

struct boss_gahzrankaAI : public ScriptedAI
{
    boss_gahzrankaAI(Creature* c) : ScriptedAI(c) {}
    uint32 Frostbreath_Timer;
    uint32 MassiveGeyser_Timer;
    uint32 Slam_Timer;

    void Reset()
    {
        Frostbreath_Timer = 8000;
        MassiveGeyser_Timer = 25000;
        Slam_Timer = 17000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //Frostbreath_Timer
        if (Frostbreath_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_FROSTBREATH);
            Frostbreath_Timer = 7000 + rand()%4000;
        } else Frostbreath_Timer -= diff;

        //MassiveGeyser_Timer
        if (MassiveGeyser_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_MASSIVEGEYSER);
            DoResetThreat();

            MassiveGeyser_Timer = 22000 + rand()%10000;
        } else MassiveGeyser_Timer -= diff;

        //Slam_Timer
        if (Slam_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SLAM);
            Slam_Timer = 12000 + rand()%8000;
        } else Slam_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_gahzranka(Creature* pCreature)
{
    return new boss_gahzrankaAI (pCreature);
}

void AddSC_boss_gahzranka()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_gahzranka";
    newscript->GetAI = &GetAI_boss_gahzranka;
    newscript->RegisterSelf();
}

