/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Garr
Complete(%): 50
Comment: Adds NYI
Category: Molten Core
EndScriptData */

#include "ScriptPCH.h"

// Garr spells
#define SPELL_ANTIMAGICPULSE        19492
#define SPELL_MAGMASHACKLES         19496
#define SPELL_ENRAGE                19516                   //Stacking enrage (stacks to 10 times)

//Add spells
#define SPELL_ERUPTION              19497
#define SPELL_IMMOLATE              20294

struct boss_garrAI : public ScriptedAI
{
    boss_garrAI(Creature* c) : ScriptedAI(c) {}

    uint32 AntiMagicPulse_Timer;
    uint32 MagmaShackles_Timer;
    uint32 CheckAdds_Timer;
    uint64 Add[8];
    bool Enraged[8];

    void Reset()
    {
        AntiMagicPulse_Timer = 25000;                       //These times are probably wrong
        MagmaShackles_Timer = 15000;
        CheckAdds_Timer = 2000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //AntiMagicPulse_Timer
        if (AntiMagicPulse_Timer <= diff)
        {
            DoCast(me, SPELL_ANTIMAGICPULSE);
            AntiMagicPulse_Timer = 10000 + rand()%5000;
        } else AntiMagicPulse_Timer -= diff;

        //MagmaShackles_Timer
        if (MagmaShackles_Timer <= diff)
        {
            DoCast(me, SPELL_MAGMASHACKLES);
            MagmaShackles_Timer = 8000 + rand()%4000;
        } else MagmaShackles_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct mob_fireswornAI : public ScriptedAI
{
    mob_fireswornAI(Creature* c) : ScriptedAI(c) {}

    uint32 Immolate_Timer;

    void Reset()
    {
        Immolate_Timer = 4000;                              //These times are probably wrong
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Immolate_Timer
        if (Immolate_Timer <= diff)
        {
             if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, SPELL_IMMOLATE);

            Immolate_Timer = urand(5000,10000);
        } else Immolate_Timer -= diff;

        //Cast Erruption and let them die
        if (me->GetHealth() <= me->GetMaxHealth() * 0.10f)
        {
            DoCast(me->getVictim(), SPELL_ERUPTION);
            me->setDeathState(JUST_DIED);
            me->RemoveCorpse();
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_garr(Creature* pCreature)
{
    return new boss_garrAI (pCreature);
}

CreatureAI* GetAI_mob_firesworn(Creature* pCreature)
{
    return new mob_fireswornAI (pCreature);
}

void AddSC_boss_garr()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_garr";
    newscript->GetAI = &GetAI_boss_garr;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_firesworn";
    newscript->GetAI = &GetAI_mob_firesworn;
    newscript->RegisterSelf();
}

