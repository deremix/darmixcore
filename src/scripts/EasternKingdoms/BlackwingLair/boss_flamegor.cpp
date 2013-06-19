/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Flamegor
Complete(%): 100
Comment:
Category: Blackwing Lair
EndScriptData */

#include "ScriptPCH.h"

#define EMOTE_FRENZY            -1469031

#define SPELL_SHADOWFLAME        22539
#define SPELL_WINGBUFFET         23339
#define SPELL_FRENZY             23342                      //This spell periodically triggers fire nova

struct boss_flamegorAI : public ScriptedAI
{
    boss_flamegorAI(Creature* c) : ScriptedAI(c) {}

    uint32 ShadowFlame_Timer;
    uint32 WingBuffet_Timer;
    uint32 Frenzy_Timer;

    void Reset()
    {
        ShadowFlame_Timer = 21000;                          //These times are probably wrong
        WingBuffet_Timer = 35000;
        Frenzy_Timer = 10000;
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoZoneInCombat();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //ShadowFlame_Timer
        if (ShadowFlame_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SHADOWFLAME);
            ShadowFlame_Timer = 15000 + rand()%7000;
        } else ShadowFlame_Timer -= diff;

        //WingBuffet_Timer
        if (WingBuffet_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_WINGBUFFET);
            if (DoGetThreat(me->getVictim()))
                DoModifyThreatPercent(me->getVictim(),-75);

            WingBuffet_Timer = 25000;
        } else WingBuffet_Timer -= diff;

        //Frenzy_Timer
        if (Frenzy_Timer <= diff)
        {
            DoScriptText(EMOTE_FRENZY, me);
            DoCast(me, SPELL_FRENZY);
            Frenzy_Timer = urand(8000, 10000);
        } else Frenzy_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_flamegor(Creature* pCreature)
{
    return new boss_flamegorAI (pCreature);
}

void AddSC_boss_flamegor()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_flamegor";
    newscript->GetAI = &GetAI_boss_flamegor;
    newscript->RegisterSelf();
}

