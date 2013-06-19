/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Tirisfal_Glades
Complete(%): 100
Comment: Quest support: 590, 1819
Category: Tirisfal Glades
EndScriptData */

/* ContentData
npc_calvin_montague
go_mausoleum_door
go_mausoleum_trigger
EndContentData */

#include "ScriptPCH.h"

/*######
## npc_calvin_montague
######*/

enum eCalvin
{
    SAY_COMPLETE        = -1000356,
    SPELL_CALVIN_DRINK  = 2639,                             // possibly not correct spell (but iconId is correct)
    QUEST_590           = 590,
    FACTION_HOSTILE     = 168
};

struct npc_calvin_montagueAI : public ScriptedAI
{
    npc_calvin_montagueAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_uiNormFaction = pCreature->getFaction();
        Reset();
    }

    uint32 m_uiNormFaction;
    uint32 m_uiPhase;
    uint32 m_uiPhaseTimer;
    uint64 m_uiPlayerGUID;

    void Reset()
    {
        m_uiPhase = 0;
        m_uiPhaseTimer = 5000;
        m_uiPlayerGUID = 0;

        if (me->getFaction() != m_uiNormFaction)
            me->setFaction(m_uiNormFaction);
    }

    void AttackedBy(Unit* pAttacker)
    {
        if (me->getVictim() || me->IsFriendlyTo(pAttacker))
            return;

        AttackStart(pAttacker);
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (uiDamage > me->GetHealth() || ((me->GetHealth() - uiDamage)*100 / me->GetMaxHealth() < 15))
        {
            uiDamage = 0;

            me->setFaction(m_uiNormFaction);
            me->CombatStop(true);

            m_uiPhase = 1;

            if (pDoneBy->GetTypeId() == TYPEID_PLAYER)
                m_uiPlayerGUID = pDoneBy->GetGUID();
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiPhase)
        {
            if (m_uiPhaseTimer <= uiDiff)
                m_uiPhaseTimer = 7500;
            else
            {
                m_uiPhaseTimer -= uiDiff;
                return;
            }

            switch (m_uiPhase)
            {
                case 1:
                    DoScriptText(SAY_COMPLETE, me);
                    ++m_uiPhase;
                    break;
                case 2:
                    if (Player* pPlayer = Unit::GetPlayer(*me, m_uiPlayerGUID))
                        pPlayer->AreaExploredOrEventHappens(QUEST_590);

                    DoCast(me, SPELL_DRINK, true);
                    ++m_uiPhase;
                    break;
                case 3:
                    EnterEvadeMode();
                    break;
            }

            return;
        }

        if (!me->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_npc_calvin_montague(Creature* pCreature)
{
    return new npc_calvin_montagueAI(pCreature);
}

bool QuestAccept_npc_calvin_montague(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    if (pQuest->GetQuestId() == QUEST_590)
    {
        pCreature->setFaction(FACTION_HOSTILE);
        pCreature->AI()->AttackStart(pPlayer);
    }
    return true;
}

/*######
## go_mausoleum_door
## go_mausoleum_trigger
######*/

enum eMausoleum
{
    QUEST_ULAG      = 1819,
    NPC_ULAG        = 6390,
    GO_TRIGGER      = 104593,
    GO_DOOR         = 176594
};

bool GOHello_go_mausoleum_door(Player* pPlayer, GameObject* /*pGo*/)
{
    if (pPlayer->GetQuestStatus(QUEST_ULAG) != QUEST_STATUS_INCOMPLETE)
        return false;

    if (GameObject* pTrigger = pPlayer->FindNearestGameObject(GO_TRIGGER, 30.0f))
    {
        pTrigger->SetGoState(GO_STATE_READY);
        pPlayer->SummonCreature(NPC_ULAG, 2390.26f, 336.47f, 40.01f, 2.26f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
        return false;
    }

    return false;
}

bool GOHello_go_mausoleum_trigger(Player* pPlayer, GameObject* pGo)
{
    if (pPlayer->GetQuestStatus(QUEST_ULAG) != QUEST_STATUS_INCOMPLETE)
        return false;

    if (GameObject* pDoor = pPlayer->FindNearestGameObject(GO_DOOR, 30.0f))
    {
        pGo->SetGoState(GO_STATE_ACTIVE);
        pDoor->RemoveFlag(GAMEOBJECT_FLAGS,GO_FLAG_INTERACT_COND);
        return true;
    }

    return false;
}

void AddSC_tirisfal_glades()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_calvin_montague";
    newscript->GetAI = &GetAI_npc_calvin_montague;
    newscript->pQuestAccept = &QuestAccept_npc_calvin_montague;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_mausoleum_door";
    newscript->pGOHello = &GOHello_go_mausoleum_door;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_mausoleum_trigger";
    newscript->pGOHello = &GOHello_go_mausoleum_trigger;
    newscript->RegisterSelf();
}
