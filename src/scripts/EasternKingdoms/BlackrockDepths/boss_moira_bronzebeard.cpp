/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Moira_Bronzbeard
Complete(%): 90
Comment: Healing of Emperor NYI
Category: Blackrock Depths
EndScriptData */

#include "ScriptPCH.h"

enum Spells
{
    SPELL_HEAL                                             = 10917,
    SPELL_RENEW                                            = 10929,
    SPELL_SHIELD                                           = 10901,
    SPELL_MINDBLAST                                        = 10947,
    SPELL_SHADOWWORDPAIN                                   = 10894,
    SPELL_SMITE                                            = 10934
};

struct boss_moira_bronzebeardAI : public ScriptedAI
{
    boss_moira_bronzebeardAI(Creature* c) : ScriptedAI(c) {}

    uint32 Heal_Timer;
    uint32 MindBlast_Timer;
    uint32 ShadowWordPain_Timer;
    uint32 Smite_Timer;

    void Reset()
    {
        Heal_Timer = 12000;                                 //These times are probably wrong
        MindBlast_Timer = 16000;
        ShadowWordPain_Timer = 2000;
        Smite_Timer = 8000;
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //MindBlast_Timer
        if (MindBlast_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_MINDBLAST);
            MindBlast_Timer = 14000;
        } else MindBlast_Timer -= diff;

        //ShadowWordPain_Timer
        if (ShadowWordPain_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SHADOWWORDPAIN);
            ShadowWordPain_Timer = 18000;
        } else ShadowWordPain_Timer -= diff;

        //Smite_Timer
        if (Smite_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SMITE);
            Smite_Timer = 10000;
        } else Smite_Timer -= diff;

    }
};
CreatureAI* GetAI_boss_moira_bronzebeard(Creature* pCreature)
{
    return new boss_moira_bronzebeardAI (pCreature);
}

void AddSC_boss_moira_bronzebeard()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_moira_bronzebeard";
    newscript->GetAI = &GetAI_boss_moira_bronzebeard;
    newscript->RegisterSelf();
}
