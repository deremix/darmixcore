/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Azshir_the_Sleepless
Complete(%): 80
Comment:
Category: Scarlet Monastery
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_CALLOFTHEGRAVE            17831
#define SPELL_TERRIFY                   7399
#define SPELL_SOULSIPHON                7290

struct boss_azshir_the_sleeplessAI : public ScriptedAI
{
    boss_azshir_the_sleeplessAI(Creature* c) : ScriptedAI(c) {}

    uint32 SoulSiphon_Timer;
    uint32 CallOftheGrave_Timer;
    uint32 Terrify_Timer;

    void Reset()
    {
        SoulSiphon_Timer = 1;
        CallOftheGrave_Timer = 30000;
        Terrify_Timer = 20000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //If we are <50% hp cast Soul Siphon rank 1
        if (me->GetHealth()*100 / me->GetMaxHealth() <= 50 && !me->IsNonMeleeSpellCasted(false))
        {
            //SoulSiphon_Timer
            if (SoulSiphon_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SOULSIPHON);
                return;

                //SoulSiphon_Timer = 20000;
            } else SoulSiphon_Timer -= diff;
        }

        //CallOfTheGrave_Timer
        if (CallOftheGrave_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_CALLOFTHEGRAVE);
            CallOftheGrave_Timer = 30000;
        } else CallOftheGrave_Timer -= diff;

        //Terrify_Timer
        if (Terrify_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_TERRIFY);
            Terrify_Timer = 20000;
        } else Terrify_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_azshir_the_sleepless(Creature* pCreature)
{
    return new boss_azshir_the_sleeplessAI (pCreature);
}

void AddSC_boss_azshir_the_sleepless()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_azshir_the_sleepless";
    newscript->GetAI = &GetAI_boss_azshir_the_sleepless;
    newscript->RegisterSelf();
}

