/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Ebonroc
Complete(%): 50
Comment: Shadow of Ebonroc needs core support
Category: Blackwing Lair
EndScriptData */

#include "ScriptPCH.h"

#define SPELL_SHADOWFLAME           22539
#define SPELL_WINGBUFFET            18500
#define SPELL_SHADOWOFEBONROC       23340
#define SPELL_HEAL                  41386                   //Thea Heal spell of his Shadow

struct boss_ebonrocAI : public ScriptedAI
{
    boss_ebonrocAI(Creature* c) : ScriptedAI(c) {}

    uint32 ShadowFlame_Timer;
    uint32 WingBuffet_Timer;
    uint32 ShadowOfEbonroc_Timer;
    uint32 Heal_Timer;

    void Reset()
    {
        ShadowFlame_Timer = 15000;                          //These times are probably wrong
        WingBuffet_Timer = 30000;
        ShadowOfEbonroc_Timer = 45000;
        Heal_Timer = 1000;
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoZoneInCombat();
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        //Shadowflame Timer
        if (ShadowFlame_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SHADOWFLAME);
            ShadowFlame_Timer = urand(12000,15000);
        } else ShadowFlame_Timer -= diff;

        //Wing Buffet Timer
        if (WingBuffet_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_WINGBUFFET);
            WingBuffet_Timer = 25000;
        } else WingBuffet_Timer -= diff;

        //Shadow of Ebonroc Timer
        if (ShadowOfEbonroc_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SHADOWOFEBONROC);
            ShadowOfEbonroc_Timer = urand(25000,350000);
        } else ShadowOfEbonroc_Timer -= diff;

        if (me->getVictim()->HasAura(SPELL_SHADOWOFEBONROC, 0))
        {
            if (Heal_Timer <= diff)
            {
                DoCast(me, SPELL_HEAL);
                Heal_Timer = urand(1000,3000);
            } else Heal_Timer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_boss_ebonroc(Creature* pCreature)
{
    return new boss_ebonrocAI (pCreature);
}

void AddSC_boss_ebonroc()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_ebonroc";
    newscript->GetAI = &GetAI_boss_ebonroc;
    newscript->RegisterSelf();
}

