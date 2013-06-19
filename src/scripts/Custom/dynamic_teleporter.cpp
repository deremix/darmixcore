/*******************************************************
 * File:'dynamic_teleporter.cpp'
 * (c)2011 - Wolf Officious (ladislav.alexa@gmail.com)
 *******************************************************/

/* ScriptData
ScriptName: dynamic_teleporter
LoaderName: void AddSC_dynamic_teleporter()
%Complete: 100
Comment: Dynamic Teleporter sctipt module (gameobject & npc)
Category: cusomscripts
EndScriptData */

/*
-- NPC SQL EXAMPLE
INSERT INTO `creature_template`   (`entry`, `modelid_A`, `modelid_H`,              `name`,`subname`,`minlevel`,`maxlevel`,`minhealth`,`maxhealth`,`faction_A`,`faction_H`,`npcflag`,`scale`,`mindmg`,`maxdmg`,`attackpower`,`baseattacktime`,`unit_flags`,`minrangedmg`,`maxrangedmg`,`rangedattackpower`,`RegenHealth`,        `ScriptName`)
VALUES                            ('500001',      '736',       '736','Dynamic Teleporter',   'TEST',      '80',      '80',     '5000',     '5000',       '35',       '35',      '1',    '2', '50000', '50000',      '35000',             '1',       '512',      '10000',      '10000',               '68',        '255','dynamic_teleporter');

-- GO SQL EXAMPLE
INSERT INTO `gameobject_template` (`entry`,`type`,`displayId`,              `name`,`size`,        `ScriptName`)
VALUES                            ('500001',  '1',       '17','Dynamic Teleporter',   '2','dynamic_teleporter');
*/

#include "ScriptPCH.h"
#include "DynamicTeleportMgr.h"

#ifndef GOSSIP_SENDER_MAIN
#define GOSSIP_SENDER_MAIN 1
#endif

// SHOW MENU BY ID
inline void DynamicTeleporter_ShowMenu(Player *player, Unit *unit, uint32 menu_id)
{
    if(!sDynamicTeleportMgr->isLoaded())
        sDynamicTeleportMgr->Init();

    uint32 count = 0;
    uint8  send_counter = 0;

    if(player->isGameMaster() && menu_id == 0)
    {
        ++count;
        player->ADD_GOSSIP_ITEM(5, "~RELOAD TELEPORTER DATA~\n", GOSSIP_SENDER_MAIN, 666);
    }

    for(uint32 itr = 0; itr < sDynamicTeleportMgr->GetCount(); itr++)
    {
        TeleportData const* td;
        td = sDynamicTeleportMgr->GetTeleportData(itr);

        if(td)
        {
            if(td->menu_parent == menu_id)
            {
                if(td->faction == 0 || player->GetTeam() == td->faction) // HORDE 67, ALLIANCE 469
                {
                    uint8 icon_id = td->icon;

                    if(icon_id > 9)
                    {
                        icon_id = 0;
                    }

                    player->ADD_GOSSIP_ITEM(icon_id, td->name.c_str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + td->entry);

                    ++count;
                    ++send_counter;

                    if(send_counter >= 10)
                    {
                        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, unit->GetGUID());
                        send_counter = 0;
                    }
                }
            }
        }
        else
        {
            sLog.outError("TD_ERROR: UNK1 (!td)");
            return;
        }
    }

    if(send_counter != 0 || count == 0)
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, unit->GetGUID());
}

// TELEPORT PROCESS
inline void DynamicTeleporter_TeleportToTarget(Player *player, uint32 target_id)
{
    if(!sDynamicTeleportMgr->isLoaded())
        sDynamicTeleportMgr->Init();

    TeleportLoc const* tl;
    tl = sDynamicTeleportMgr->GetTeleportLoc(target_id);

    if(tl)
    {
        player->TeleportTo(tl->map, tl->position_x, tl->position_y, tl->position_z, tl->position_o);
    }
    else
    {
        sLog.outError("TD_ERROR: UNK1 (!tl)");
    }

    return;
}

// ACTION TYPE HANDLER
inline bool DynamicTeleporter_HandleMenuAction(Player *player, Unit *unit, uint32 action)
{
    if(!sDynamicTeleportMgr->isLoaded())
        sDynamicTeleportMgr->Init();

    for(uint32 itr = 0; itr < sDynamicTeleportMgr->GetCount(); itr++)
    {
        TeleportData const* td;
        td = sDynamicTeleportMgr->GetTeleportData(itr);

        if(td)
        {
            if(td->entry == action - GOSSIP_ACTION_INFO_DEF)
            {
                if(td->menu_sub < 0)
                {
                    DynamicTeleporter_TeleportToTarget(player, action - GOSSIP_ACTION_INFO_DEF);
                    return true;
                }
                else
                {
                    DynamicTeleporter_ShowMenu(player, unit, td->menu_sub);
                    return true;
                }
            }
        }
        else
        {
            sLog.outError("TD_ERROR: UNK1 (!td)");
            return false;
        }
    }

    return false;
}

// UNIVERSAL GOSSIP HELLO
bool DynamicTeleporter_GossipHello(Player *player, Unit *unit)
{
    if(player->isInCombat())
    {
        player->GetSession()->SendNotification("You are in combat!");
        return true;
    }

    DynamicTeleporter_ShowMenu(player, unit, 0);
    return true;
}

// UNIVERSAL GOSSIP SELECT
bool DynamicTeleporter_GossipSelect(Player *player, Unit *unit, uint32 sender, uint32 action)
{
    if(sender != GOSSIP_SENDER_MAIN)
    {
        player->CLOSE_GOSSIP_MENU();
        return true;
    }

    if(player->isInCombat())
    {
        player->GetSession()->SendNotification("You are in combat!");
        player->CLOSE_GOSSIP_MENU();
        return true;
    }

    player->PlayerTalkClass->ClearMenus();

    if(action == 666 && player->isGameMaster())
    {
        player->CLOSE_GOSSIP_MENU();
        sDynamicTeleportMgr->Update();
        player->GetSession()->SendNotification("Reloaded..");
        return true;
    }

    if(!DynamicTeleporter_HandleMenuAction(player, unit, action))
        player->CLOSE_GOSSIP_MENU();

    return true;
}

// GAMEOBJECT GOSSIP HELLO
bool DynamicTeleporter_GO_GossipHello(Player *player, GameObject *gobject)
{ return DynamicTeleporter_GossipHello(player, (Unit*)gobject); }

// GAMEOBJECT GOSSIP SELECT
bool DynamicTeleporter_GO_GossipSelect(Player *player, GameObject *gobject, uint32 sender, uint32 action)
{ return DynamicTeleporter_GossipSelect(player, (Unit*)gobject, sender, action); }

// NPC GOSSIP HELLO
bool DynamicTeleporter_NPC_GossipHello(Player *player, Creature *creature)
{ return DynamicTeleporter_GossipHello(player, (Unit*)creature); }

// NPC GOSSIP SELECT
bool DynamicTeleporter_NPC_GossipSelect(Player *player, Creature *creature, uint32 sender, uint32 action)
{ return DynamicTeleporter_GossipSelect(player, (Unit*)creature, sender, action); }

void AddSC_dynamic_teleporter()
{
    Script *newscript;

    newscript                        = new Script;
    newscript->Name                  = "dynamic_teleporter";

    newscript->pGOHello              = &DynamicTeleporter_GO_GossipHello;
    newscript->pGOSelect             = &DynamicTeleporter_GO_GossipSelect;

    newscript->pGossipHello          = &DynamicTeleporter_NPC_GossipHello;
    newscript->pGossipSelect         = &DynamicTeleporter_NPC_GossipSelect;

    newscript->RegisterSelf();
}