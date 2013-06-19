/*******************************************************
 * File:'npc_buffer'
 * ScriptName:'npc_buffer'
 * SDCreator: Deremix
 * SDComplete: 100%
 * SDComment: Buff Master
 * SDCategory: NPC
 *******************************************************/
#include "ScriptPCH.h"

bool GossipHello_npc_buffer(Player *player, Creature *creature)
{
	if(!player->isInCombat())
	{
		player->ADD_GOSSIP_ITEM( 5, "Mark of the Wild",      GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM( 5, "Thorns",                GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM( 5, "Amplify Magic",         GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM( 5, "Arcane Intellect",      GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM( 5, "Dampen Magic",          GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM( 5, "Blessing of Kings",     GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM( 5, "Blessing of Might",     GOSSIP_SENDER_MAIN, 7);
		player->ADD_GOSSIP_ITEM( 5, "Blessing of Wisdom",    GOSSIP_SENDER_MAIN, 8);
		player->ADD_GOSSIP_ITEM( 5, "Divine Spirit",         GOSSIP_SENDER_MAIN, 9);
		player->ADD_GOSSIP_ITEM( 5, "Power Word: Fortitude", GOSSIP_SENDER_MAIN, 10);
		player->ADD_GOSSIP_ITEM( 5, "Restore Power",         GOSSIP_SENDER_MAIN, 11);
		player->ADD_GOSSIP_ITEM( 5, "Reset Cooldown",        GOSSIP_SENDER_MAIN, 12);
		player->ADD_GOSSIP_ITEM( 5, "Repair Items",          GOSSIP_SENDER_MAIN, 13);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());
		return true;
	}
	player->GetSession()->SendNotification("You are in combat.");
    return false;
 }

bool GossipSelect_npc_buffer(Player *player, Creature *creature, uint32 sender, uint32 action )
{
    switch (action)
    {
		//Mark of the Wild
	case 1:
		player->CLOSE_GOSSIP_MENU();
        creature->CastSpell(player,9885,false);
		creature->MonsterWhisper("Nyni mas Mark of the Wild.", player->GetGUID());
	    break;
		//Thorns
	case 2:
        player->CLOSE_GOSSIP_MENU();
        creature->CastSpell(player,26992,false);
    	creature->MonsterWhisper("Nyni mas Thorns.", player->GetGUID());
	    break;
		//Amplify Magic
	case 3:
        player->CLOSE_GOSSIP_MENU();
        creature->CastSpell(player,33946,false);
		creature->MonsterWhisper("Nyni mas Amplify Magic.", player->GetGUID());
	    break;
		//Arcane Intellect
	case 4:
        player->CLOSE_GOSSIP_MENU();
        creature->CastSpell(player,27126,false);
		creature->MonsterWhisper("Nyni mas Arcane Intellect.", player->GetGUID());
		break;
		//Dampen Magic
	case 5:
        player->CLOSE_GOSSIP_MENU();
        creature->CastSpell(player,33944,false);
		creature->MonsterWhisper("Nyni mas Dampen Magic.", player->GetGUID());
		break;
		//Blessing of Kings
	case 6:
        player->CLOSE_GOSSIP_MENU();
        creature->CastSpell(player,20217,false);
		creature->MonsterWhisper("Nyni mas Blessing of Kings.", player->GetGUID());
		break;
		//Blessing of Might
	case 7:
        player->CLOSE_GOSSIP_MENU();
        creature->CastSpell(player,19838,false);
		creature->MonsterWhisper("Nyni mas Blessing of Might.", player->GetGUID());
		break;
		//Blessing of Wisdom
	case 8:
        player->CLOSE_GOSSIP_MENU();
        creature->CastSpell(player,25290,false);
		creature->MonsterWhisper("Nyni mas Blessing of Wisdom.", player->GetGUID());
		break;
		//Divine Spirit
	case 9:
        player->CLOSE_GOSSIP_MENU();
        creature->CastSpell(player,25312,false);
		creature->MonsterWhisper("Nyni mas Divine Spirit.", player->GetGUID());
		break;
		//Power Word: Fortitude
	case 10:
        player->CLOSE_GOSSIP_MENU();
        creature->CastSpell(player,25389,false);
		creature->MonsterWhisper("Nyni mas Power Word: Fortitude.", player->GetGUID());
		break;
		//Commandan Shout
	case 11:
	    player->CLOSE_GOSSIP_MENU();
		player->SetHealth(player->GetMaxHealth());
		player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));
		creature->MonsterWhisper("Obnovil jsi si zivoty a manu.", player->GetGUID());
		break;
		//Reset Cooldown
	case 12:
	    player->CLOSE_GOSSIP_MENU();
		player->RemoveAllSpellCooldown();
		creature->MonsterWhisper("Zrusil jsi si Cooldowny.", player->GetGUID());
		break;
		//Repair Item's
	case 13:
	    player->CLOSE_GOSSIP_MENU();
		player->DurabilityRepairAll(false, 0, false);
		creature->MonsterWhisper("Opravil jsi si itemy.", player->GetGUID());
		break;
	}
	return true;
}

void AddSC_npc_buffer()
{
    Script *newscript;
        newscript = new Script;
        newscript->Name="npc_buffer";
        newscript->pGossipHello = &GossipHello_npc_buffer;
        newscript->pGossipSelect = &GossipSelect_npc_buffer;
        newscript->RegisterSelf();
}
