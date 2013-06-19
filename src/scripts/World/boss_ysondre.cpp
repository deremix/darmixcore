/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Ysondre
Complete(%): 90
Comment: Mark of Nature & Teleport missing
Category: Bosses
EndScriptData */

#include "ScriptPCH.h"

enum eEnums
{
    SAY_AGGRO               = -1000360,
    SAY_SUMMONDRUIDS        = -1000361,

    SPELL_SLEEP             = 24777,
    SPELL_NOXIOUSBREATH     = 24818,
    SPELL_TAILSWEEP         = 15847,
    //SPELL_MARKOFNATURE   = 25040,                        // Not working
    SPELL_LIGHTNINGWAVE     = 24819,
    SPELL_SUMMONDRUIDS      = 24795,

    //druid spells
    SPELL_MOONFIRE          = 21669
};

// Ysondre script
struct boss_ysondreAI : public ScriptedAI
{
    boss_ysondreAI(Creature* pCreature) : ScriptedAI(pCreature) {}

    uint32 m_uiSleep_Timer;
    uint32 m_uiNoxiousBreath_Timer;
    uint32 m_uiTailSweep_Timer;
    //uint32 m_uiMarkOfNature_Timer;
    uint32 m_uiLightningWave_Timer;
    uint32 m_uiSummonDruidModifier;

    void Reset()
    {
        m_uiSleep_Timer = 15000 + rand()%5000;
        m_uiNoxiousBreath_Timer = 8000;
        m_uiTailSweep_Timer = 4000;
        //m_uiMarkOfNature_Timer = 45000;
        m_uiLightningWave_Timer = 12000;
        m_uiSummonDruidModifier = 0;
    }

    void EnterCombat(Unit* /*pWho*/)
    {
        DoScriptText(SAY_AGGRO, me);
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
            pSummoned->AI()->AttackStart(pTarget);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

        //Sleep_Timer
        if (m_uiSleep_Timer <= uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_SLEEP);

            m_uiSleep_Timer = 8000 + rand()%7000;
        }
        else
            m_uiSleep_Timer -= uiDiff;

        //NoxiousBreath_Timer
        if (m_uiNoxiousBreath_Timer <= uiDiff)
        {
            DoCast(me->getVictim(), SPELL_NOXIOUSBREATH);
            m_uiNoxiousBreath_Timer = 14000 + rand()%6000;
        }
        else
            m_uiNoxiousBreath_Timer -= uiDiff;

        //Tailsweep every 2 seconds
        if (m_uiTailSweep_Timer <= uiDiff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_TAILSWEEP);

            m_uiTailSweep_Timer = 2000;
        }
        else
            m_uiTailSweep_Timer -= uiDiff;

        //MarkOfNature_Timer
        //if (m_uiMarkOfNature_Timer <= uiDiff)
        //{
        //    DoCast(me->getVictim(), SPELL_MARKOFNATURE);
        //    m_uiMarkOfNature_Timer = 45000;
        //}
        //else
        //    m_uiMarkOfNature_Timer -= uiDiff;

        //LightningWave_Timer
        if (m_uiLightningWave_Timer <= uiDiff)
        {
            //Cast LIGHTNINGWAVE on a Random target
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_LIGHTNINGWAVE);

            m_uiLightningWave_Timer = 7000 + rand()%5000;
        }
        else
            m_uiLightningWave_Timer -= uiDiff;

        //Summon Druids
        if ((me->GetHealth()*100 / me->GetMaxHealth()) <= (100-(25*m_uiSummonDruidModifier)))
        {
            DoScriptText(SAY_SUMMONDRUIDS, me);

            for (int i = 0; i < 10; ++i)
                DoCast(me, SPELL_SUMMONDRUIDS, true);

            ++m_uiSummonDruidModifier;
        }

        DoMeleeAttackIfReady();
    }
};

// Summoned druid script
struct mob_dementeddruidsAI : public ScriptedAI
{
    mob_dementeddruidsAI(Creature* c) : ScriptedAI(c) {}

    uint32 m_uiMoonFire_Timer;

    void Reset()
    {
        m_uiMoonFire_Timer = 3000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!UpdateVictim())
            return;

        //MoonFire_Timer
        if (m_uiMoonFire_Timer <= uiDiff)
        {
            DoCast(me->getVictim(), SPELL_MOONFIRE);
            m_uiMoonFire_Timer = 5000;
        }
        else
            m_uiMoonFire_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_ysondre(Creature* pCreature)
{
    return new boss_ysondreAI (pCreature);
}

CreatureAI* GetAI_mob_dementeddruids(Creature* pCreature)
{
    return new mob_dementeddruidsAI (pCreature);
}

void AddSC_boss_ysondre()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_ysondre";
    newscript->GetAI = &GetAI_boss_ysondre;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dementeddruids";
    newscript->GetAI = &GetAI_mob_dementeddruids;
    newscript->RegisterSelf();
}

