/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Arcanist_Doan
Complete(%): 100
Comment:
Category: Scarlet Monastery
EndScriptData */

#include "ScriptPCH.h"

enum eEnums
{
    SAY_AGGRO                   = -1189019,
    SAY_SPECIALAE               = -1189020,

    SPELL_POLYMORPH             = 13323,
    SPELL_AOESILENCE            = 8988,
    SPELL_ARCANEEXPLOSION       = 9433,
    SPELL_FIREAOE               = 9435,
    SPELL_ARCANEBUBBLE          = 9438,
};

struct boss_arcanist_doanAI : public ScriptedAI
{
    boss_arcanist_doanAI(Creature* c) : ScriptedAI(c) {}

    uint32 Polymorph_Timer;
    uint32 AoESilence_Timer;
    uint32 ArcaneExplosion_Timer;
    bool bCanDetonate;
    bool bShielded;

    void Reset()
    {
        Polymorph_Timer = 20000;
        AoESilence_Timer = 15000;
        ArcaneExplosion_Timer = 3000;
        bCanDetonate = false;
        bShielded = false;
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoScriptText(SAY_AGGRO, me);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        if (bShielded && bCanDetonate)
        {
            DoCast(me, SPELL_FIREAOE);
            bCanDetonate = false;
        }

        if (me->HasAura(SPELL_ARCANEBUBBLE, 0))
            return;

        //If we are <50% hp cast Arcane Bubble
        if (!bShielded && me->GetHealth()*100 / me->GetMaxHealth() <= 50)
        {
            //wait if we already casting
            if (me->IsNonMeleeSpellCasted(false))
                return;

            DoScriptText(SAY_SPECIALAE, me);
            DoCast(me, SPELL_ARCANEBUBBLE);

            bCanDetonate = true;
            bShielded = true;
        }

        if (Polymorph_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,1))
                DoCast(pTarget, SPELL_POLYMORPH);

            Polymorph_Timer = 20000;
        } else Polymorph_Timer -= diff;

        //AoESilence_Timer
        if (AoESilence_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_AOESILENCE);
            AoESilence_Timer = 15000 + rand()%5000;
        } else AoESilence_Timer -= diff;

        //ArcaneExplosion_Timer
        if (ArcaneExplosion_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_ARCANEEXPLOSION);
            ArcaneExplosion_Timer = 8000;
        } else ArcaneExplosion_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_arcanist_doan(Creature* pCreature)
{
    return new boss_arcanist_doanAI (pCreature);
}

void AddSC_boss_arcanist_doan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_arcanist_doan";
    newscript->GetAI = &GetAI_boss_arcanist_doan;
    newscript->RegisterSelf();
}

