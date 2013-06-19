/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Magmadar
Complete(%): 75
Comment: Conflag on ground nyi, fear causes issues without VMAPs
Category: Molten Core
EndScriptData */

#include "ScriptPCH.h"

#define EMOTE_FRENZY                -1409001

#define SPELL_FRENZY                19451
#define SPELL_MAGMASPIT             19449                   //This is actually a buff he gives himself
#define SPELL_PANIC                 19408
#define SPELL_LAVABOMB              19411                   //This calls a dummy server side effect that isn't implemented yet
#define SPELL_LAVABOMB_ALT          19428                   //This is the spell that the lava bomb casts

struct boss_magmadarAI : public ScriptedAI
{
    boss_magmadarAI(Creature* c) : ScriptedAI(c) {}

    uint32 Frenzy_Timer;
    uint32 Panic_Timer;
    uint32 Lavabomb_Timer;

    void Reset()
    {
        Frenzy_Timer = 30000;
        Panic_Timer = 20000;
        Lavabomb_Timer = 12000;

        DoCast(me, SPELL_MAGMASPIT, true);
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Frenzy_Timer
        if (Frenzy_Timer <= diff)
        {
            DoScriptText(EMOTE_FRENZY, me);
            DoCast(me, SPELL_FRENZY);
            Frenzy_Timer = 15000;
        } else Frenzy_Timer -= diff;

        //Panic_Timer
        if (Panic_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_PANIC);
            Panic_Timer = 35000;
        } else Panic_Timer -= diff;

        //Lavabomb_Timer
        if (Lavabomb_Timer <= diff)
        {
            if (Unit* pTarget = SelectUnit(SELECT_TARGET_RANDOM,0))
                DoCast(pTarget, SPELL_LAVABOMB_ALT);

            Lavabomb_Timer = 12000;
        } else Lavabomb_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_magmadar(Creature* pCreature)
{
    return new boss_magmadarAI (pCreature);
}

void AddSC_boss_magmadar()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_magmadar";
    newscript->GetAI = &GetAI_boss_magmadar;
    newscript->RegisterSelf();
}

