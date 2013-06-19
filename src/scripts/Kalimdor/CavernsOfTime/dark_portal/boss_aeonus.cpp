/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Aeonus
Complete(%): 80
Comment: Some spells not implemented
Category: Caverns of Time, The Dark Portal
EndScriptData */

#include "ScriptPCH.h"
#include "dark_portal.h"

#define SAY_ENTER         -1269012
#define SAY_AGGRO         -1269013
#define SAY_BANISH        -1269014
#define SAY_SLAY1         -1269015
#define SAY_SLAY2         -1269016
#define SAY_DEATH         -1269017
#define EMOTE_FRENZY      -1269018

#define SPELL_CLEAVE        40504
#define SPELL_TIME_STOP     31422
#define SPELL_ENRAGE        37605
#define SPELL_SAND_BREATH   31473
#define H_SPELL_SAND_BREATH 39049

struct boss_aeonusAI : public ScriptedAI
{
    boss_aeonusAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
        HeroicMode = me->GetMap()->IsHeroic();
    }

    ScriptedInstance *pInstance;
    bool HeroicMode;

    uint32 SandBreath_Timer;
    uint32 TimeStop_Timer;
    uint32 Frenzy_Timer;

    void Reset()
    {
        SandBreath_Timer = 30000;
        TimeStop_Timer = 40000;
        Frenzy_Timer = 120000;
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoScriptText(SAY_AGGRO, me);
    }

    void MoveInLineOfSight(Unit* who)
    {
        //Despawn Time Keeper
        if (who->GetTypeId() == TYPEID_UNIT && who->GetEntry() == C_TIME_KEEPER)
        {
            if (me->IsWithinDistInMap(who,20.0f))
            {
                DoScriptText(SAY_BANISH, me);
                me->DealDamage(who, who->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            }
        }

        ScriptedAI::MoveInLineOfSight(who);
    }

    void JustDied(Unit* /*victim*/)
    {
        DoScriptText(SAY_DEATH, me);

         if (pInstance)
         {
             pInstance->SetData(TYPE_RIFT,DONE);
             pInstance->SetData(TYPE_MEDIVH,DONE);//FIXME: later should be removed
         }
    }

    void KilledUnit(Unit* /*victim*/)
    {
        switch (rand()%2)
        {
            case 0: DoScriptText(SAY_SLAY1, me); break;
            case 1: DoScriptText(SAY_SLAY2, me); break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!UpdateVictim())
            return;

        //Sand Breath
        if (SandBreath_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_SAND_BREATH);
            SandBreath_Timer = 30000;
        } else SandBreath_Timer -= diff;

        //Time Stop
        if (TimeStop_Timer <= diff)
        {
            DoCast(me->getVictim(), SPELL_TIME_STOP);
            TimeStop_Timer = 40000;
        } else TimeStop_Timer -= diff;

        //Frenzy
        if (Frenzy_Timer <= diff)
        {
            DoScriptText(EMOTE_FRENZY, me);
            DoCast(me, SPELL_ENRAGE);
            Frenzy_Timer = 120000;
        } else Frenzy_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_aeonus(Creature* pCreature)
{
    return new boss_aeonusAI (pCreature);
}

void AddSC_boss_aeonus()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_aeonus";
    newscript->GetAI = &GetAI_boss_aeonus;
    newscript->RegisterSelf();
}

