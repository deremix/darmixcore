/* =Authors info============================================================= */
/*   Name:      Mark Changer - OC                                             */
/*   Filename:  mark_changer.cpp                                              */
/*   Author: 2011 Menur (d)                                                   */ 
/*                                                                            */
/*   Description: Scpript provides item change. Items for change are in       */
/*                world_db.creature_mark_changer                              */
/*                                                                            */
/*                                      --Please do not modify authors info-- */
/* ========================================================================== */
/* =Editors info============================================================= */
/*                                                                            */
/* ========================================================================== */

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "Creature.h"
#include <cstring>
#include <stdio.h>
#include <map>

#define SQL_SELECT_CHANGER "SELECT CMC.item_req, CMC.item_req_count, CMC.item_give, CMC.item_give_count, CMC.gossip_text FROM creature_mark_changer as CMC"
#define ACTION_MASK 100
#define NPC_GOODBYE_TEXT "Good bye."

struct TItemsArray
{
  uint32 items[4]; 
};

typedef TItemsArray ItemsArray;
std::map<uint32, ItemsArray *> changer_list;
uint32 count;


bool GossipHello_npc_mark_changer(Player *player, Creature *_Creature)
{
  QueryResult_AutoPtr result;
  result = WorldDatabase.PQuery(SQL_SELECT_CHANGER);
  if (result)
  {
    uint32 i = 0;
    uint32 items[4];
    ItemsArray *IA;
    do
    {
      Field *fields = result->Fetch();
      IA = new ItemsArray;
      
      player->ADD_GOSSIP_ITEM(4, fields[4].GetString(), GOSSIP_SENDER_MAIN, i);
      
      IA->items[0] = fields[0].GetInt32();
      IA->items[1] = fields[1].GetInt32();
      IA->items[2] = fields[2].GetInt32();
      IA->items[3] = fields[3].GetInt32();
      
      changer_list[i] = IA;
      i++;
    }while(result->NextRow());
        count = i + 1;
  }
  uint32 byebye = count + ACTION_MASK;
  player->ADD_GOSSIP_ITEM(4, NPC_GOODBYE_TEXT, GOSSIP_SENDER_MAIN, byebye);
  player->SEND_GOSSIP_MENU(1,_Creature->GetGUID()); //Shows Gossip menu
  return true; 
}


bool GossipSelect_npc_mark_changer(Player *player, Creature *_Creature, uint32 sender, uint32 action)
{
        if (action == (count + ACTION_MASK))
        {
                player->PlayerTalkClass->CloseGossip();
                //return true;
        }
        else
        {
                uint32 items[4];
                for (uint32 i = 0; i < 4; i++)
                { 
                items[i] = changer_list[action]->items[i];   
                }   
        ItemPosCountVec dest;
        if ((player->HasItemCount(items[0], items[1], false)) && (player->CanStoreNewItem( NULL_BAG, NULL_SLOT, dest, items[2], items[3]) == EQUIP_ERR_OK))
        {
           player->DestroyItemCount(items[0], items[1], true, false);
           Item* item = player->StoreNewItem(dest, items[2], true, Item::GenerateItemRandomPropertyId(items[2]));
           player->SendNewItem(item,items[3],true,false);
        }
        else
        {
           _Creature->MonsterWhisper("Nemáš požadované itemy!!", player->GetGUID());
        }
                changer_list.clear();
                GossipHello_npc_mark_changer(player, _Creature);
        }
        return true; 
}

void AddSC_npc_mark_changer()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="npc_mark_changer";
    newscript->pGossipHello = &GossipHello_npc_mark_changer;
    newscript->pGossipSelect = &GossipSelect_npc_mark_changer;
    newscript->RegisterSelf();
}