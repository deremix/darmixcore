/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_High_Inquisitor_Fairbanks
Complete(%): 100
Comment: TODO: if this guy not involved in some special event, remove (and let ACID script)
Category: Scarlet Monastery
EndScriptData */

#include "ScriptPCH.h"

enum eSpells
{
    SPELL_CURSEOFBLOOD              = 8282,
    SPELL_DISPELMAGIC               = 15090,
    SPELL_FEAR                      = 12096,
    SPELL_HEAL                      = 12039,
    SPELL_POWERWORDSHIELD           = 11647,
    SPELL_SLEEP                     = 8399
};

struct boss_high_inquisitor_fairbanksAI : public ScriptedAI
{
    boss_high_inquisitor_fairbanksAI(Creature* c) : ScriptedAI(c) {}

    uint32 CurseOfBlood_Timer;
    uint32 DispelMagic_Timer;
    uint32 Fear_Timer;
    uint32 Heal_Timer;
    uint32 Sleep_Timer;
    uint32 Dispel_Timer;
    bool PowerWordShield;

    void Reset()
    {
        CurseOfBlood_Timer = 10000;
        DispelMagic_Timer = 30000;
        Fear_Timer = 40000;
        Heal_Timer = 30000;
        Sleep_Timer = 30000;
        Dispel_Timer = 20000;
        PowerWordShield = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //If we are <25% hp cast Heal
        if (me->GetHealth()*100 / me->GetMaxHealth() <= 25 && !me->IsNonMeleeSpellCasted(false) && Heal_Timer <= diff)
        {
            DoCast(me, SPELL_HEAL);
            Heal_Timer = 30000;
        } else Heal_Timer -= diff;

        //Fear_Timer
        if (Fear_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,1))
                DoCast(pTarget, SPELL_FEAR);

            Fear_Timer = 40000;
        } else Fear_Timer -= diff;

        //Sleep_Timer
        if (Sleep_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_TOPAGGRO,0))
                DoCast(pTarget, SPELL_SLEEP);

            Sleep_Timer = 30000;
        } else Sleep_Timer -= diff;

        //PowerWordShield_Timer
        if (!PowerWordShield && me->GetHealth()*100 / me->GetMaxHealth() <= 25)
        {
            DoCast(me, SPELL_POWERWORDSHIELD);
            PowerWordShield = true;
        }

        //Dispel_Timer
        if (Dispel_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, SPELL_DISPELMAGIC);

            DispelMagic_Timer = 30000;
        } else DispelMagic_Timer -= diff;

        //CurseOfBlood_Timer
        if (CurseOfBlood_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CURSEOFBLOOD);
            CurseOfBlood_Timer = 25000;
        } else CurseOfBlood_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_high_inquisitor_fairbanks(Creature* pCreature)
{
    return new boss_high_inquisitor_fairbanksAI (pCreature);
}

void AddSC_boss_high_inquisitor_fairbanks()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_high_inquisitor_fairbanks";
    newscript->GetAI = &GetAI_boss_high_inquisitor_fairbanks;
    newscript->RegisterSelf();
}

