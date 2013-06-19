/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Custom_Gossip_Codebox
Complete(%): 100
Comment: Show a codebox in gossip option
Category: Script Examples
EndScriptData */

#include "ScriptPCH.h"
#include <cstring>

//This function is called when the player opens the gossip menubool
bool GossipHello_custom_gossip_codebox(Player* player, Creature* pCreature)
{
    player->ADD_GOSSIP_ITEM_EXTENDED(0, "A quiz: what's your name?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1, "", 0, true);
    player->ADD_GOSSIP_ITEM(0, "I'm not interested", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+2);

    player->PlayerTalkClass->SendGossipMenu(907,pCreature->GetGUID());
    return true;
}

//This function is called when the player clicks an option on the gossip menubool
bool GossipSelect_custom_gossip_codebox(Player* player, Creature* pCreature, uint32 /*sender*/, uint32 action)
{
    if (action == GOSSIP_ACTION_INFO_DEF+2)
    {
        pCreature->Say("Normal select, guess you're not interested.", LANG_UNIVERSAL, 0);
        player->CLOSE_GOSSIP_MENU();
    }
    return true;
}

bool GossipSelectWithCode_custom_gossip_codebox(Player* player, Creature* pCreature, uint32 sender, uint32 action, const char* sCode)
{
    if (sender == GOSSIP_SENDER_MAIN)
    {
        if (action == GOSSIP_ACTION_INFO_DEF+1)
        {
            if (std::strcmp(sCode, player->GetName()) != 0)
            {
                pCreature->Say("Wrong!", LANG_UNIVERSAL, 0);
                pCreature->CastSpell(player, 12826, true);
            }
            else
            {
                pCreature->Say("You're right, you are allowed to see my inner secrets.", LANG_UNIVERSAL, 0);
                pCreature->CastSpell(player, 26990, true);
            }
            player->CLOSE_GOSSIP_MENU();
            return true;
        }
    }
    return false;
}

void AddSC_custom_gossip_codebox()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "custom_gossip_codebox";
    newscript->pGossipHello =           &GossipHello_custom_gossip_codebox;
    newscript->pGossipSelect =          &GossipSelect_custom_gossip_codebox;
    newscript->pGossipSelectWithCode =  &GossipSelectWithCode_custom_gossip_codebox;
    newscript->RegisterSelf();
}

