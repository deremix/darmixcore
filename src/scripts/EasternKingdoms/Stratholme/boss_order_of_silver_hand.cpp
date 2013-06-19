/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Silver_Hand_Bosses
Complete(%): 40
Comment: Basic script to have support for Horde paladin epic mount (quest 9737). All 5 members of Order of the Silver Hand running this script (least for now)
Category: Stratholme
EndScriptData */

#include "ScriptPCH.h"
#include "stratholme.h"

/*#####
# Additional:
# Although this is a working solution, the correct would be in addition to check if Aurius is dead.
# Once player extinguish the eternal flame (cast spell 31497->start event 11206) Aurius should become hostile.
# Once Aurius is defeated, he should be the one summoning the ghosts.
#####*/

#define SH_GREGOR           17910
#define SH_CATHELA          17911
#define SH_NEMAS            17912
#define SH_AELMAR           17913
#define SH_VICAR            17914
#define SH_QUEST_CREDIT     17915

#define SPELL_HOLY_LIGHT    25263
#define SPELL_DIVINE_SHIELD 13874

struct boss_silver_hand_bossesAI : public ScriptedAI
{
    boss_silver_hand_bossesAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
    }

    ScriptedInstance *pInstance;

    uint32 HolyLight_Timer;
    uint32 DivineShield_Timer;

    void Reset()
    {
        HolyLight_Timer = 20000;
        DivineShield_Timer = 20000;

        if (pInstance)
        {
            switch (me->GetEntry())
            {
                case SH_AELMAR:
                    pInstance->SetData(TYPE_SH_AELMAR, 0);
                    break;
                case SH_CATHELA:
                    pInstance->SetData(TYPE_SH_CATHELA, 0);
                    break;
                case SH_GREGOR:
                    pInstance->SetData(TYPE_SH_GREGOR, 0);
                    break;
                case SH_NEMAS:
                    pInstance->SetData(TYPE_SH_NEMAS, 0);
                    break;
                case SH_VICAR:
                    pInstance->SetData(TYPE_SH_VICAR, 0);
                    break;
            }
        }
    }

    void EnterCombat(Unit* /*who*/)
    {
    }

    void JustDied(Unit* Killer)
    {
        if (pInstance)
        {
            switch (me->GetEntry())
            {
                case SH_AELMAR:
                    pInstance->SetData(TYPE_SH_AELMAR, 2);
                    break;
                case SH_CATHELA:
                    pInstance->SetData(TYPE_SH_CATHELA, 2);
                    break;
                case SH_GREGOR:
                    pInstance->SetData(TYPE_SH_GREGOR, 2);
                    break;
                case SH_NEMAS:
                    pInstance->SetData(TYPE_SH_NEMAS, 2);
                    break;
                case SH_VICAR:
                    pInstance->SetData(TYPE_SH_VICAR, 2);
                    break;
            }
            if (pInstance->GetData(TYPE_SH_QUEST) && Killer->GetTypeId() == TYPEID_PLAYER)
                CAST_PLR(Killer)->KilledMonsterCredit(SH_QUEST_CREDIT,me->GetGUID());
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        if (HolyLight_Timer <= diff)
        {
            if (me->GetHealth()*5 < me->GetMaxHealth())
            {
                DoCast(me, SPELL_HOLY_LIGHT);
                HolyLight_Timer = 20000;
            }
        } else HolyLight_Timer -= diff;

        if (DivineShield_Timer <= diff)
        {
            if (me->GetHealth()*20 < me->GetMaxHealth())
            {
                DoCast(me, SPELL_DIVINE_SHIELD);
                DivineShield_Timer = 40000;
            }
        } else DivineShield_Timer -= diff;

        DoMeleeAttackIfReady();
    }

};
CreatureAI* GetAI_boss_silver_hand_bossesAI(Creature* pCreature)
{
    return new boss_silver_hand_bossesAI (pCreature);
}

void AddSC_boss_order_of_silver_hand()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_silver_hand_bosses";
    newscript->GetAI = &GetAI_boss_silver_hand_bossesAI;
    newscript->RegisterSelf();
}
