/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Broodlord_Lashlayer
Complete(%): 100
Comment:
Category: Blackwing Lair
EndScriptData */

#include "ScriptPCH.h"

#define SAY_AGGRO               -1469000
#define SAY_LEASH               -1469001

#define SPELL_CLEAVE            26350
#define SPELL_BLASTWAVE         23331
#define SPELL_MORTALSTRIKE      24573
#define SPELL_KNOCKBACK         25778

struct boss_broodlordAI : public ScriptedAI
{
    boss_broodlordAI(Creature* c) : ScriptedAI(c) {}

    uint32 Cleave_Timer;
    uint32 BlastWave_Timer;
    uint32 MortalStrike_Timer;
    uint32 KnockBack_Timer;

    void Reset()
    {
        Cleave_Timer = 8000;                                //These times are probably wrong
        BlastWave_Timer = 12000;
        MortalStrike_Timer = 20000;
        KnockBack_Timer = 30000;
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoScriptText(SAY_AGGRO, me);
        DoZoneInCombat();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Cleave_Timer
        if (Cleave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CLEAVE);
            Cleave_Timer = 7000;
        } else Cleave_Timer -= diff;

        // BlastWave
        if (BlastWave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_BLASTWAVE);
            BlastWave_Timer = urand(8000,16000);
        } else BlastWave_Timer -= diff;

        //MortalStrike_Timer
        if (MortalStrike_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_MORTALSTRIKE);
            MortalStrike_Timer = urand(25000,35000);
        } else MortalStrike_Timer -= diff;

        if (KnockBack_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_KNOCKBACK);
            //Drop 50% aggro
            if (DoGetThreat(me->getVictim()))
                DoModifyThreatPercent(me->getVictim(),-50);

            KnockBack_Timer = urand(15000,30000);
        } else KnockBack_Timer -= diff;

        if (EnterEvadeIfOutOfCombatArea(diff))
            DoScriptText(SAY_LEASH, me);

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_broodlord(Creature* pCreature)
{
    return new boss_broodlordAI (pCreature);
}

void AddSC_boss_broodlord()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_broodlord";
    newscript->GetAI = &GetAI_boss_broodlord;
    newscript->RegisterSelf();
}

