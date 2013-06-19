/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Huhuran
Complete(%): 100
Comment:
Category: Temple of Ahn'Qiraj
EndScriptData */

#include "ScriptPCH.h"

#define EMOTE_GENERIC_FRENZY_KILL   -1000001
#define EMOTE_GENERIC_BERSERK       -1000004

#define SPELL_FRENZY 26051
#define SPELL_BERSERK 26068
#define SPELL_POISONBOLT 26052
#define SPELL_NOXIOUSPOISON 26053
#define SPELL_WYVERNSTING 26180
#define SPELL_ACIDSPIT 26050

struct boss_huhuranAI : public ScriptedAI
{
    boss_huhuranAI(Creature* c) : ScriptedAI(c) {}

    uint32 Frenzy_Timer;
    uint32 Wyvern_Timer;
    uint32 Spit_Timer;
    uint32 PoisonBolt_Timer;
    uint32 NoxiousPoison_Timer;
    uint32 FrenzyBack_Timer;

    bool Frenzy;
    bool Berserk;

    void Reset()
    {
        Frenzy_Timer = 25000 + rand()%10000;
        Wyvern_Timer = 18000 + rand()%10000;
        Spit_Timer = 8000;
        PoisonBolt_Timer = 4000;
        NoxiousPoison_Timer = 10000 + rand()%10000;
        FrenzyBack_Timer = 15000;

        Frenzy = false;
        Berserk = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //Frenzy_Timer
        if (!Frenzy && Frenzy_Timer <= diff)
        {
            DoCast(me, SPELL_FRENZY);
            DoScriptText(EMOTE_GENERIC_FRENZY_KILL, me);
            Frenzy = true;
            PoisonBolt_Timer = 3000;
            Frenzy_Timer = 25000 + rand()%10000;
        } else Frenzy_Timer -= diff;

        // Wyvern Timer
        if (Wyvern_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget,SPELL_WYVERNSTING);
            Wyvern_Timer = 15000 + rand()%17000;
        } else Wyvern_Timer -= diff;

        //Spit Timer
        if (Spit_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_ACIDSPIT);
            Spit_Timer = 5000 + rand()%5000;
        } else Spit_Timer -= diff;

        //NoxiousPoison_Timer
        if (NoxiousPoison_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_NOXIOUSPOISON);
            NoxiousPoison_Timer = 12000 + rand()%12000;
        } else NoxiousPoison_Timer -= diff;

        //PoisonBolt only if frenzy or berserk
        if (Frenzy || Berserk)
        {
            if (PoisonBolt_Timer <= diff)
            {
                DoCast(me->getVictim(),SPELL_POISONBOLT);
                PoisonBolt_Timer = 3000;
            } else PoisonBolt_Timer -= diff;
        }

        //FrenzyBack_Timer
        if (Frenzy && FrenzyBack_Timer <= diff)
        {
            me->InterruptNonMeleeSpells(false);
            Frenzy = false;
            FrenzyBack_Timer = 15000;
        } else FrenzyBack_Timer -= diff;

        if (!Berserk && me->GetHealth()*100 / me->GetMaxHealth() < 31)
        {
            me->InterruptNonMeleeSpells(false);
            DoScriptText(EMOTE_GENERIC_BERSERK, me);
            DoCast(me, SPELL_BERSERK);
            Berserk = true;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_huhuran(Creature* pCreature)
{
    return new boss_huhuranAI (pCreature);
}

void AddSC_boss_huhuran()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_huhuran";
    newscript->GetAI = &GetAI_boss_huhuran;
    newscript->RegisterSelf();
}

