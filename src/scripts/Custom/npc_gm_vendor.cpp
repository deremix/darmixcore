/*******************************************************
 * File:'npc_gm_vendor'
 * ScriptName:'npc_gm_vendor'
 * SDCreator: Deremix
 * SDComplete: 100%
 * SDComment: Gamemaster Vendor
 * SDCategory: NPC
 *******************************************************/

#include "ScriptPCH.h"

bool GossipHello_npc_gm_vendor(Player *player, Creature *creature)
{
	if (!player->isGameMaster())
	{
		player->CLOSE_GOSSIP_MENU();
		creature->MonsterWhisper( "Nejsi gm, nebo mas vypli gm mod.", player->GetGUID());
	}
	else
	{
		player->ADD_GOSSIP_ITEM( 2, "Gamemaster's Hood",                        GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM( 2, "Gamemaster's Robe",                      GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM( 2, "Gamemaster's Slippers",                  GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM( 2, "Gamemaster's Weapon",                    GOSSIP_SENDER_MAIN, 4);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
	}
	return true;
}

bool GossipSelect_npc_gm_vendor(Player *player, Creature *creature, uint32 sender, uint32 uiAction)
{
	ItemPosCountVec look;
	uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, look, 24573, 1, false);
	player->PlayerTalkClass->ClearMenus();
	switch(uiAction)
	{
	case 1:
		if (player->HasItemCount(12064, 1, true))
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Uz mas Gamemaster's Hood", player->GetGUID());
		}
		else
		{
			player->AddItem(12064, 1);
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Dostal jsi Gamemaster's Hood", player->GetGUID());
		}
		break;
	case 2:
		if (player->HasItemCount(2586, 1, true))
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Uz mas Gamemaster's Robe", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			player->AddItem(2586, 1);
			creature->MonsterWhisper("Dostal jsi Gamemaster's Robe", player->GetGUID());
		}
		break;
	case 3:
		if (player->HasItemCount(11508, 1, true))
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Uz mas Gamemaster's Slippers", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			player->AddItem(11508, 1);
			creature->MonsterWhisper("Dostal jsi Gamemaster's Slippers", player->GetGUID());
		}
		break;
	case 4:
		if (player->HasItemCount(18582, 1, true))
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Uz mas Gamemaster's Weapon", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			player->AddItem(18582, 1);
			creature->MonsterWhisper("Dostal ji Gamemaster's Weapon", player->GetGUID());
		}
		break;
	}
	return true;
}

void AddSC_npc_gm_vendor()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_gm_vendor";
    newscript->pGossipHello =  &GossipHello_npc_gm_vendor;
    newscript->pGossipSelect = &GossipSelect_npc_gm_vendor;
    newscript->RegisterSelf();
}