/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Halycon
Complete(%): 100
Comment:
Category: Blackrock Spire
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_CROWDPUMMEL       10887
#define SPELL_MIGHTYBLOW        14099

#define ADD_1X                  -169.839203f
#define ADD_1Y                  -324.961395f
#define ADD_1Z                  64.401443f
#define ADD_1O                  3.124724f

struct boss_halyconAI : public ScriptedAI
{
    boss_halyconAI(Creature* c) : ScriptedAI(c) {}

    uint32 CrowdPummel_Timer;
    uint32 MightyBlow_Timer;
    bool Summoned;

    void Reset()
    {
        CrowdPummel_Timer = 8000;
        MightyBlow_Timer = 14000;
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

        //CrowdPummel_Timer
        if (CrowdPummel_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CROWDPUMMEL);
            CrowdPummel_Timer = 14000;
        } else CrowdPummel_Timer -= diff;

        //MightyBlow_Timer
        if (MightyBlow_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_MIGHTYBLOW);
            MightyBlow_Timer = 10000;
        } else MightyBlow_Timer -= diff;

        //Summon Gizrul
        if (!Summoned && me->GetHealth()*100 / me->GetMaxHealth() < 25)
        {
            me->SummonCreature(10268,ADD_1X,ADD_1Y,ADD_1Z,ADD_1O,TEMPSUMMON_TIMED_DESPAWN,300000);
            Summoned = true;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_halycon(Creature* pCreature)
{
    return new boss_halyconAI (pCreature);
}

void AddSC_boss_halycon()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_halycon";
    newscript->GetAI = &GetAI_boss_halycon;
    newscript->RegisterSelf();
}

