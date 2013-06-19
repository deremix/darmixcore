/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Boss_Void_Reaver
Complete(%): 90
Comment: Should reset if raid are out of room.
Category: Tempest Keep, The Eye
EndScriptData */

#include "ScriptPCH.h"
#include "the_eye.h"

#define SAY_AGGRO                   -1550000
#define SAY_SLAY1                   -1550001
#define SAY_SLAY2                   -1550002
#define SAY_SLAY3                   -1550003
#define SAY_DEATH                   -1550004
#define SAY_POUNDING1               -1550005
#define SAY_POUNDING2               -1550006

#define SPELL_POUNDING              34162
#define SPELL_ARCANE_ORB            34172
#define SPELL_KNOCK_AWAY            25778
#define SPELL_BERSERK               27680

struct boss_void_reaverAI : public ScriptedAI
{
    boss_void_reaverAI(Creature* c) : ScriptedAI(c)
    {
        pInstance = c->GetInstanceData();
    }

    ScriptedInstance* pInstance;

    uint32 Pounding_Timer;
    uint32 ArcaneOrb_Timer;
    uint32 KnockAway_Timer;
    uint32 Berserk_Timer;

    bool Enraged;

    void Reset()
    {
        Pounding_Timer = 15000;
        ArcaneOrb_Timer = 3000;
        KnockAway_Timer = 30000;
        Berserk_Timer = 600000;

        Enraged = false;

        if (pInstance && me->isAlive())
            pInstance->SetData(DATA_VOIDREAVEREVENT, NOT_STARTED);
    }

    void KilledUnit(Unit* /*victim*/)
    {
        switch (rand()%3)
        {
        case 0: DoScriptText(SAY_SLAY1, me); break;
        case 1: DoScriptText(SAY_SLAY2, me); break;
        case 2: DoScriptText(SAY_SLAY3, me); break;
        }
    }

    void JustDied(Unit* /*victim*/)
    {
        DoScriptText(SAY_DEATH, me);

        if (pInstance)
            pInstance->SetData(DATA_VOIDREAVEREVENT, DONE);
    }

    void EnterCombat(Unit* /*who*/)
    {
        DoScriptText(SAY_AGGRO, me);

        if (pInstance)
            pInstance->SetData(DATA_VOIDREAVEREVENT, IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!UpdateVictim())
            return;

        // Pounding
        if (Pounding_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_POUNDING);

            switch (rand()%2)
            {
            case 0: DoScriptText(SAY_POUNDING1, me); break;
            case 1: DoScriptText(SAY_POUNDING2, me); break;
            }
             Pounding_Timer = 15000;                         //cast time(3000) + cooldown time(12000)
        } else Pounding_Timer -= diff;

        // Arcane Orb
        if (ArcaneOrb_Timer <= diff)
        {
            if (me->IsNonMeleeSpellCasted(false))
            {
                ArcaneOrb_Timer = 3000;
                return;
            }
            Unit* pTarget = NULL;
            std::list<HostileReference *> t_list = me->getThreatManager().getThreatList();
            std::vector<Unit* > target_list;
            for (std::list<HostileReference *>::iterator itr = t_list.begin(); itr != t_list.end(); ++itr)
            {
                pTarget = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                                                            //18 yard radius minimum
                if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER && pTarget->isAlive() && pTarget->GetDistance2d(me) >= 18)
                    target_list.push_back(pTarget);
                pTarget = NULL;
            }
            if (target_list.size())
                pTarget = *(target_list.begin()+rand()%target_list.size());

            if (pTarget)
                me->CastSpell(pTarget->GetPositionX(),pTarget->GetPositionY(),pTarget->GetPositionZ(), SPELL_ARCANE_ORB, false);

            ArcaneOrb_Timer = 3000;
        } else ArcaneOrb_Timer -= diff;

        // Single Target knock back, reduces aggro
        if (KnockAway_Timer <= diff)
        {
            DoCast(me->getVictim(),SPELL_KNOCK_AWAY);

            //Drop 25% aggro
            if (DoGetThreat(me->getVictim()))
                DoModifyThreatPercent(me->getVictim(),-25);

            KnockAway_Timer = 30000;
        } else KnockAway_Timer -= diff;

        //Berserk
        if (Berserk_Timer <= diff && !Enraged)
        {
            DoCast(me,SPELL_BERSERK);
            Enraged = true;
        } else Berserk_Timer -= diff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_void_reaver(Creature* pCreature)
{
    return new boss_void_reaverAI (pCreature);
}

void AddSC_boss_void_reaver()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_void_reaver";
    newscript->GetAI = &GetAI_boss_void_reaver;
    newscript->RegisterSelf();
}

