/*******************************************************
 * File:'npc_customize'
 * ScriptName:'npc_customize'
 * SDCreator: Helper
 * SDComplete: 100%
 * SDComment: Character Customize
 * SDCategory: NPC
 *******************************************************/

//Includy
#include "ScriptPCH.h"

//Definice
#define EVENT_MARK      18154

bool GossipHello_npc_customize(Player *player, Creature *creature)
{
	if(!player->isInCombat())
	{
		player->ADD_GOSSIP_ITEM( 5, "Rename",                                GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM( 5, "Fire Shield",                           GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM( 5, "Blood Elf Male Illusion",               GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM( 5, "Blood Elf Female Illusion",             GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM( 5, "Tauren Male Illusion",                  GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM( 5, "Tauren Female Illusion",                GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM( 5, "Gnome Male Illusion",                   GOSSIP_SENDER_MAIN, 7);
		player->ADD_GOSSIP_ITEM( 5, "Gnome Female Illusion",                 GOSSIP_SENDER_MAIN, 8);
		player->ADD_GOSSIP_ITEM( 5, "Human Male Illusion",                   GOSSIP_SENDER_MAIN, 9);
		player->ADD_GOSSIP_ITEM( 5, "Human Female Illusion",                 GOSSIP_SENDER_MAIN, 10);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
		return true;
	}
	player->GetSession()->SendNotification("You are in combat.");
	return false;
};

bool GossipSelect_npc_customize(Player *player, Creature *creature, uint32 sender, uint32 action )
{
	switch(action)
	{
	//Rename
	case 1:
		if (player->HasItemCount(EVENT_MARK, 15, true))
		{
			player->CLOSE_GOSSIP_MENU();
			player->DestroyItemCount(EVENT_MARK, 15, true);
			player->SetAtLoginFlag(AT_LOGIN_RENAME);
			creature->MonsterWhisper("Relogni se pro Rename.", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Nemas dostatek Event Marek, dones mi jich 15!", player->GetGUID());
		}
		break;
	case 2:
		if (player->HasItemCount(EVENT_MARK, 75, true))
		{
			player->CLOSE_GOSSIP_MENU();
			player->learnSpell(46804);
			player->DestroyItemCount(EVENT_MARK, 75, true);
			creature->MonsterWhisper("Nyni mas Dark Fire Shield State", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Nemas dostatek Event Marek, dones mi jich 75!", player->GetGUID());
		}
		break;
	case 3:
		if (player->HasItemCount(EVENT_MARK, 40, true))
		{
			player->CLOSE_GOSSIP_MENU();
			player->learnSpell(37807);
			player->DestroyItemCount(EVENT_MARK, 40, true);
			creature->MonsterWhisper("Nyni mas Blood Elf Male Illusion", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Nemas dostatek Event Marek, dones mi jich 40!", player->GetGUID());
		}
		break;
	case 4:
		if (player->HasItemCount(EVENT_MARK, 40, true))
		{
			player->CLOSE_GOSSIP_MENU();
			player->learnSpell(37806);
			player->DestroyItemCount(EVENT_MARK, 40, true);
			creature->MonsterWhisper("Nyni mas Blood Elf Female Illusion", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Nemas dostatek Event Marek, Dones mi jich 40!", player->GetGUID());
		}
		break;
	case 5:
		if (player->HasItemCount(EVENT_MARK, 40, true))
		{
			player->CLOSE_GOSSIP_MENU();
			player->learnSpell(37810);
			player->DestroyItemCount(EVENT_MARK, 40, true);
			creature->MonsterWhisper("Nyni mas Tauren Male Illusion", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Nemas dostatek Event Marek, dones mi jich 40!", player->GetGUID());
		}
		break;
	case 6:
		if (player->HasItemCount(EVENT_MARK, 40, true))
		{
			player->CLOSE_GOSSIP_MENU();
			player->learnSpell(37811);
			player->DestroyItemCount(EVENT_MARK, 40, true);
			creature->MonsterWhisper("Nyni mas Tauren Female Illusion", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Nemas dostatek Event Marek, dones mi jich 40!", player->GetGUID());
		}
		break;
	case 7:
		if (player->HasItemCount(EVENT_MARK, 40, true))
		{
			player->CLOSE_GOSSIP_MENU();
			player->learnSpell(37808);
			player->DestroyItemCount(EVENT_MARK, 40, true);
			creature->MonsterWhisper("Nyni mas Gnome Male Illusion", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Nemas dostatek Event Marek, dones mi jich 40!", player->GetGUID());
		}
		break;
	case 8:
		if (player->HasItemCount(EVENT_MARK, 40, true))
		{
			player->CLOSE_GOSSIP_MENU();
			player->learnSpell(37809);
			player->DestroyItemCount(EVENT_MARK, 40, true);
			creature->MonsterWhisper("Nyni mas Gnome Female Illusion", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Nemas dostatek Event Marek, dones mi jich 40!", player->GetGUID());
		}
		break;
	case 9:
		if (player->HasItemCount(EVENT_MARK, 40, true))
		{
			player->CLOSE_GOSSIP_MENU();
			player->learnSpell(35466);
			player->DestroyItemCount(EVENT_MARK, 40, true);
			creature->MonsterWhisper("Nyni mas Human Male Illusion", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Nemas dostatek Event Marek, dones mi jich 40!", player->GetGUID());
		}
		break;
	case 10:
		if (player->HasItemCount(EVENT_MARK, 40, true))
		{
			player->CLOSE_GOSSIP_MENU();
			player->learnSpell(37805);
			player->DestroyItemCount(EVENT_MARK, 40, true);
			creature->MonsterWhisper("Nyni mas Human Female Illusion", player->GetGUID());
		}
		else
		{
			player->CLOSE_GOSSIP_MENU();
			creature->MonsterWhisper("Nemas dostatek Event Marek, dones mi jich 40!", player->GetGUID());
		}
		break;
	}
	return true;
};

void AddSC_npc_customize()
{
    Script *newscript;
        newscript = new Script;
        newscript->Name="npc_customize";
        newscript->pGossipHello = &GossipHello_npc_customize;
        newscript->pGossipSelect = &GossipSelect_npc_customize;
        newscript->RegisterSelf();
}