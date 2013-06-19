/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "AuctionHouseMgr.h"
#include "AccountMgr.h"
#include "PlayerDump.h"
#include "SpellMgr.h"
#include "Player.h"
#include "Opcodes.h"
#include "GameObject.h"
#include "Chat.h"
#include "Log.h"
#include "Guild.h"
#include "ObjectAccessor.h"
#include "MapManager.h"
#include "SpellAuras.h"
#include "Language.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "Weather.h"
#include "PointMovementGenerator.h"
#include "TargetedMovementGenerator.h"
#include "SkillDiscovery.h"
#include "SkillExtraItems.h"
#include "SystemConfig.h"
#include "Config/Config.h"
#include "Util.h"
#include "ItemEnchantmentMgr.h"
#include "BattleGroundMgr.h"
#include "InstanceSaveMgr.h"
#include "InstanceData.h"
#include "AuctionHouseBot.h"
#include "CreatureEventAIMgr.h"
#include "WaypointManager.h"

//Reload
bool ChatHandler::HandleReloadCommand(const char* arg)
{
    // this is error catcher for wrong table name in .reload commands
    PSendSysMessage("Db table with name starting from '%s' not found and can't be reloaded.",arg);
    SetSentErrorMessage(true);
    return false;
}

bool ChatHandler::HandleReloadAllCommand(const char*)
{
    HandleReloadAreaTriggerTeleportCommand("");
    HandleReloadSkillFishingBaseLevelCommand("");

    HandleReloadAllAreaCommand("");
    HandleReloadAllLootCommand("");
    HandleReloadAllNpcCommand("");
    HandleReloadAllQuestCommand("");
    HandleReloadAllSpellCommand("");
    HandleReloadAllItemCommand("");
    HandleReloadAllLocalesCommand("");

    HandleReloadCommandCommand("");
    HandleReloadReservedNameCommand("");
    HandleReloaddarmixStringCommand("");
    HandleReloadGameTeleCommand("");
    HandleReloadAutobroadcastCommand("");
    return true;
}

bool ChatHandler::HandleReloadAllAreaCommand(const char*)
{
    //HandleReloadQuestAreaTriggersCommand(""); -- reloaded in HandleReloadAllQuestCommand
    HandleReloadAreaTriggerTeleportCommand("");
    HandleReloadAreaTriggerTavernCommand("");
    HandleReloadGameGraveyardZoneCommand("");
    return true;
}

bool ChatHandler::HandleReloadAllLootCommand(const char*)
{
    sLog.outString("Re-Loading Loot Tables...");
    LoadLootTables();
    SendGlobalGMSysMessage("DB tables *_loot_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloadAllNpcCommand(const char* /*args*/)
{
    HandleReloadNpcGossipCommand("a");
    HandleReloadNpcTrainerCommand("a");
    HandleReloadNpcVendorCommand("a");
    return true;
}

bool ChatHandler::HandleReloadAllQuestCommand(const char* /*args*/)
{
    HandleReloadQuestAreaTriggersCommand("a");
    HandleReloadQuestTemplateCommand("a");

    sLog.outString("Re-Loading Quests Relations...");
    objmgr.LoadQuestRelations();
    SendGlobalGMSysMessage("DB tables *_questrelation and *_involvedrelation reloaded.");
    return true;
}

bool ChatHandler::HandleReloadAllScriptsCommand(const char*)
{
    if (sWorld.IsScriptScheduled())
    {
        PSendSysMessage("DB scripts used currently, please attempt reload later.");
        SetSentErrorMessage(true);
        return false;
    }

    sLog.outString("Re-Loading Scripts...");
    HandleReloadGameObjectScriptsCommand("a");
    HandleReloadEventScriptsCommand("a");
    HandleReloadQuestEndScriptsCommand("a");
    HandleReloadQuestStartScriptsCommand("a");
    HandleReloadSpellScriptsCommand("a");
    SendGlobalGMSysMessage("DB tables *_scripts reloaded.");
    HandleReloadDbScriptStringCommand("a");
    HandleReloadWpScriptsCommand("a");
    return true;
}

bool ChatHandler::HandleReloadAllSpellCommand(const char*)
{
    HandleReloadSkillDiscoveryTemplateCommand("a");
    HandleReloadSkillExtraItemTemplateCommand("a");
    HandleReloadSpellAffectCommand("a");
    HandleReloadSpellRequiredCommand("a");
    HandleReloadSpellElixirCommand("a");
    HandleReloadSpellLearnSpellCommand("a");
    HandleReloadSpellLinkedSpellCommand("a");
    HandleReloadSpellProcEventCommand("a");
    HandleReloadSpellRanksCommand("a");
    HandleReloadSpellScriptTargetCommand("a");
    HandleReloadSpellTargetPositionCommand("a");
    HandleReloadSpellThreatsCommand("a");
    HandleReloadSpellPetAurasCommand("a");
    HandleReloadSpellDisabledCommand("a");
    return true;
}

bool ChatHandler::HandleReloadAllItemCommand(const char*)
{
    HandleReloadPageTextsCommand("a");
    HandleReloadItemEnchantementsCommand("a");
    return true;
}

bool ChatHandler::HandleReloadAllLocalesCommand(const char* /*args*/)
{
    HandleReloadLocalesCreatureCommand("a");
    HandleReloadLocalesGameobjectCommand("a");
    HandleReloadLocalesItemCommand("a");
    HandleReloadLocalesNpcTextCommand("a");
    HandleReloadLocalesPageTextCommand("a");
    HandleReloadLocalesQuestCommand("a");
    return true;
}

bool ChatHandler::HandleReloadConfigCommand(const char* /*args*/)
{
    sLog.outString("Re-Loading config settings...");
    sWorld.LoadConfigSettings(true);
    MapManager::Instance().InitializeVisibilityDistanceInfo();
    SendGlobalGMSysMessage("World config settings reloaded.");
    return true;
}

bool ChatHandler::HandleReloadAreaTriggerTavernCommand(const char*)
{
    sLog.outString("Re-Loading Tavern Area Triggers...");
    objmgr.LoadTavernAreaTriggers();
    SendGlobalGMSysMessage("DB table areatrigger_tavern reloaded.");
    return true;
}

bool ChatHandler::HandleReloadAreaTriggerTeleportCommand(const char*)
{
    sLog.outString("Re-Loading AreaTrigger teleport definitions...");
    objmgr.LoadAreaTriggerTeleports();
    SendGlobalGMSysMessage("DB table areatrigger_teleport reloaded.");
    return true;
}

bool ChatHandler::HandleReloadAccessRequirementCommand(const char*)
{
    sLog.outString("Re-Loading Access Requirement definitions...");
    objmgr.LoadAccessRequirements();
    SendGlobalGMSysMessage("DB table access_requirement reloaded.");
     return true;
}

bool ChatHandler::HandleReloadAutobroadcastCommand(const char*)
{
    sLog.outString("Re-Loading Autobroadcast...");
    sWorld.LoadAutobroadcasts();
    SendGlobalGMSysMessage("DB table autobroadcast reloaded.");
    return true;
}

bool ChatHandler::HandleReloadCommandCommand(const char*)
{
    load_command_table = true;
    SendGlobalGMSysMessage("DB table command will be reloaded at next chat command use.");
    return true;
}

bool ChatHandler::HandleReloadCreatureQuestRelationsCommand(const char*)
{
    sLog.outString("Loading Quests Relations... (creature_questrelation)");
    objmgr.LoadCreatureQuestRelations();
    SendGlobalGMSysMessage("DB table creature_questrelation (creature quest givers) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadCreatureLinkedRespawnCommand(const char *args)
{
    sLog.outString("Loading Linked Respawns... (creature_linked_respawn)");
    objmgr.LoadCreatureLinkedRespawn();
    SendGlobalGMSysMessage("DB table creature_linked_respawn (creature linked respawns) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadGossipMenuCommand(const char*)
{
    sLog.outString( "Re-Loading `gossip_menu` Table!" );
    objmgr.LoadGossipMenu();
    SendGlobalSysMessage("DB table `gossip_menu` reloaded.");
    return true;
}

bool ChatHandler::HandleReloadGossipMenuOptionCommand(const char*)
{
    sLog.outString( "Re-Loading `gossip_menu_option` Table!" );
    objmgr.LoadGossipMenuItems();
    SendGlobalSysMessage("DB table `gossip_menu_option` reloaded.");
    return true;
}

bool ChatHandler::HandleReloadCreatureQuestInvRelationsCommand(const char*)
{
    sLog.outString("Loading Quests Relations... (creature_involvedrelation)");
    objmgr.LoadCreatureInvolvedRelations();
    SendGlobalGMSysMessage("DB table creature_involvedrelation (creature quest takers) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadGOQuestRelationsCommand(const char*)
{
    sLog.outString("Loading Quests Relations... (gameobject_questrelation)");
    objmgr.LoadGameobjectQuestRelations();
    SendGlobalGMSysMessage("DB table gameobject_questrelation (gameobject quest givers) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadGOQuestInvRelationsCommand(const char*)
{
    sLog.outString("Loading Quests Relations... (gameobject_involvedrelation)");
    objmgr.LoadGameobjectInvolvedRelations();
    SendGlobalGMSysMessage("DB table gameobject_involvedrelation (gameobject quest takers) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadQuestAreaTriggersCommand(const char*)
{
    sLog.outString("Re-Loading Quest Area Triggers...");
    objmgr.LoadQuestAreaTriggers();
    SendGlobalGMSysMessage("DB table areatrigger_involvedrelation (quest area triggers) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadQuestTemplateCommand(const char*)
{
    sLog.outString("Re-Loading Quest Templates...");
    objmgr.LoadQuests();
    SendGlobalGMSysMessage("DB table quest_template (quest definitions) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLootTemplatesCreatureCommand(const char*)
{
    sLog.outString("Re-Loading Loot Tables... (creature_loot_template)");
    LoadLootTemplates_Creature();
    LootTemplates_Creature.CheckLootRefs();
    SendGlobalGMSysMessage("DB table creature_loot_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLootTemplatesDisenchantCommand(const char*)
{
    sLog.outString("Re-Loading Loot Tables... (disenchant_loot_template)");
    LoadLootTemplates_Disenchant();
    LootTemplates_Disenchant.CheckLootRefs();
    SendGlobalGMSysMessage("DB table disenchant_loot_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLootTemplatesFishingCommand(const char*)
{
    sLog.outString("Re-Loading Loot Tables... (fishing_loot_template)");
    LoadLootTemplates_Fishing();
    LootTemplates_Fishing.CheckLootRefs();
    SendGlobalGMSysMessage("DB table fishing_loot_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLootTemplatesGameobjectCommand(const char*)
{
    sLog.outString("Re-Loading Loot Tables... (gameobject_loot_template)");
    LoadLootTemplates_Gameobject();
    LootTemplates_Gameobject.CheckLootRefs();
    SendGlobalGMSysMessage("DB table gameobject_loot_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLootTemplatesItemCommand(const char*)
{
    sLog.outString("Re-Loading Loot Tables... (item_loot_template)");
    LoadLootTemplates_Item();
    LootTemplates_Item.CheckLootRefs();
    SendGlobalGMSysMessage("DB table item_loot_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLootTemplatesPickpocketingCommand(const char*)
{
    sLog.outString("Re-Loading Loot Tables... (pickpocketing_loot_template)");
    LoadLootTemplates_Pickpocketing();
    LootTemplates_Pickpocketing.CheckLootRefs();
    SendGlobalGMSysMessage("DB table pickpocketing_loot_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLootTemplatesProspectingCommand(const char*)
{
    sLog.outString("Re-Loading Loot Tables... (prospecting_loot_template)");
    LoadLootTemplates_Prospecting();
    LootTemplates_Prospecting.CheckLootRefs();
    SendGlobalGMSysMessage("DB table prospecting_loot_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLootTemplatesMailCommand(const char*)
{
    sLog.outString( "Re-Loading Loot Tables... (`mail_loot_template`)" );
    LoadLootTemplates_Mail();
    LootTemplates_Mail.CheckLootRefs();
    SendGlobalSysMessage("DB table `mail_loot_template` reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLootTemplatesReferenceCommand(const char*)
{
    sLog.outString("Re-Loading Loot Tables... (reference_loot_template)");
    LoadLootTemplates_Reference();
    SendGlobalGMSysMessage("DB table reference_loot_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLootTemplatesSkinningCommand(const char*)
{
    sLog.outString("Re-Loading Loot Tables... (skinning_loot_template)");
    LoadLootTemplates_Skinning();
    LootTemplates_Skinning.CheckLootRefs();
    SendGlobalGMSysMessage("DB table skinning_loot_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloaddarmixStringCommand(const char*)
{
    sLog.outString("Re-Loading darmix_string Table!");
    objmgr.LoaddarmixStrings();
    SendGlobalGMSysMessage("DB table darmix_string reloaded.");
    return true;
}

bool ChatHandler::HandleReloadNpcGossipCommand(const char*)
{
    sLog.outString("Re-Loading npc_gossip Table!");
    objmgr.LoadNpcTextId();
    SendGlobalGMSysMessage("DB table npc_gossip reloaded.");
    return true;
}

bool ChatHandler::HandleReloadNpcTrainerCommand(const char*)
{
    sLog.outString("Re-Loading npc_trainer Table!");
    objmgr.LoadTrainerSpell();
    SendGlobalGMSysMessage("DB table npc_trainer reloaded.");
    return true;
}

bool ChatHandler::HandleReloadNpcVendorCommand(const char*)
{
    sLog.outString("Re-Loading npc_vendor Table!");
    objmgr.LoadVendors();
    SendGlobalGMSysMessage("DB table npc_vendor reloaded.");
    return true;
}

bool ChatHandler::HandleReloadReservedNameCommand(const char*)
{
    sLog.outString("Loading ReservedNames... (reserved_name)");
    objmgr.LoadReservedPlayersNames();
    SendGlobalGMSysMessage("DB table reserved_name (player reserved names) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSkillDiscoveryTemplateCommand(const char* /*args*/)
{
    sLog.outString("Re-Loading Skill Discovery Table...");
    LoadSkillDiscoveryTable();
    SendGlobalGMSysMessage("DB table skill_discovery_template (recipes discovered at crafting) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSkillExtraItemTemplateCommand(const char* /*args*/)
{
    sLog.outString("Re-Loading Skill Extra Item Table...");
    LoadSkillExtraItemTable();
    SendGlobalGMSysMessage("DB table skill_extra_item_template (extra item creation when crafting) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSkillFishingBaseLevelCommand(const char* /*args*/)
{
    sLog.outString("Re-Loading Skill Fishing base level requirements...");
    objmgr.LoadFishingBaseSkillLevel();
    SendGlobalGMSysMessage("DB table skill_fishing_base_level (fishing base level for zone/subzone) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellAffectCommand(const char*)
{
    sLog.outString("Re-Loading SpellAffect definitions...");
    spellmgr.LoadSpellAffects();
    SendGlobalGMSysMessage("DB table spell_affect (spell mods apply requirements) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellRequiredCommand(const char*)
{
    sLog.outString("Re-Loading Spell Required Data... ");
    spellmgr.LoadSpellRequired();
    SendGlobalGMSysMessage("DB table spell_required reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellElixirCommand(const char*)
{
    sLog.outString("Re-Loading Spell Elixir types...");
    spellmgr.LoadSpellElixirs();
    SendGlobalGMSysMessage("DB table spell_elixir (spell elixir types) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellLearnSpellCommand(const char*)
{
    sLog.outString("Re-Loading Spell Learn Spells...");
    spellmgr.LoadSpellLearnSpells();
    SendGlobalGMSysMessage("DB table spell_learn_spell reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellLinkedSpellCommand(const char*)
{
    sLog.outString("Re-Loading Spell Linked Spells...");
    spellmgr.LoadSpellLinked();
    SendGlobalGMSysMessage("DB table spell_linked_spell reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellProcEventCommand(const char*)
{
    sLog.outString("Re-Loading Spell Proc Event conditions...");
    spellmgr.LoadSpellProcEvents();
    SendGlobalGMSysMessage("DB table spell_proc_event (spell proc trigger requirements) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellRanksCommand(const char*)
{
    sLog.outString( "Re-Loading Spell Ranks..." );
    spellmgr.LoadSpellRanks();
    SendGlobalGMSysMessage("DB table `spell_ranks` reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellScriptTargetCommand(const char*)
{
    sLog.outString("Re-Loading SpellsScriptTarget...");
    spellmgr.LoadSpellScriptTarget();
    SendGlobalGMSysMessage("DB table spell_script_target (spell targets selection in case specific creature/GO requirements) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellTargetPositionCommand(const char*)
{
    sLog.outString("Re-Loading Spell target coordinates...");
    spellmgr.LoadSpellTargetPositions();
    SendGlobalGMSysMessage("DB table spell_target_position (destination coordinates for spell targets) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellThreatsCommand(const char*)
{
    sLog.outString("Re-Loading Aggro Spells Definitions...");
    spellmgr.LoadSpellThreats();
    SendGlobalGMSysMessage("DB table spell_threat (spell aggro definitions) reloaded.");
    return true;
}

bool ChatHandler::HandleReloadSpellPetAurasCommand(const char*)
{
    sLog.outString("Re-Loading Spell pet auras...");
    spellmgr.LoadSpellPetAuras();
    SendGlobalGMSysMessage("DB table spell_pet_auras reloaded.");
    return true;
}

bool ChatHandler::HandleReloadPageTextsCommand(const char*)
{
    sLog.outString("Re-Loading Page Texts...");
    objmgr.LoadPageTexts();
    SendGlobalGMSysMessage("DB table page_texts reloaded.");
    return true;
}

bool ChatHandler::HandleReloadItemEnchantementsCommand(const char*)
{
    sLog.outString("Re-Loading Item Random Enchantments Table...");
    LoadRandomEnchantmentsTable();
    SendGlobalGMSysMessage("DB table item_enchantment_template reloaded.");
    return true;
}

bool ChatHandler::HandleReloadGameObjectScriptsCommand(const char* arg)
{
    if (sWorld.IsScriptScheduled())
    {
        SendSysMessage("DB scripts used currently, please attempt reload later.");
        SetSentErrorMessage(true);
        return false;
    }

    if (*arg != 'a')
        sLog.outString("Re-Loading Scripts from gameobject_scripts...");

    objmgr.LoadGameObjectScripts();

    if (*arg != 'a')
        SendGlobalGMSysMessage("DB table gameobject_scripts reloaded.");

    return true;
}

bool ChatHandler::HandleReloadEventScriptsCommand(const char* arg)
{
    if (sWorld.IsScriptScheduled())
    {
        SendSysMessage("DB scripts used currently, please attempt reload later.");
        SetSentErrorMessage(true);
        return false;
    }

    if (*arg != 'a')
        sLog.outString("Re-Loading Scripts from event_scripts...");

    objmgr.LoadEventScripts();

    if (*arg != 'a')
        SendGlobalGMSysMessage("DB table event_scripts reloaded.");

    return true;
}

bool ChatHandler::HandleReloadEventAITextsCommand(const char* /*args*/)
{

    sLog.outString("Re-Loading Texts from `creature_ai_texts`...");
    CreatureEAI_Mgr.LoadCreatureEventAI_Texts(true);
    SendGlobalSysMessage("DB table `creature_ai_texts` reloaded.");
    return true;
}

bool ChatHandler::HandleReloadEventAISummonsCommand(const char* /*args*/)
{
    sLog.outString("Re-Loading Summons from `creature_ai_summons`...");
    CreatureEAI_Mgr.LoadCreatureEventAI_Summons(true);
    SendGlobalSysMessage("DB table `creature_ai_summons` reloaded.");
    return true;
}

bool ChatHandler::HandleReloadEventAIScriptsCommand(const char* /*args*/)
{
    sLog.outString("Re-Loading Scripts from `creature_ai_scripts`...");
    CreatureEAI_Mgr.LoadCreatureEventAI_Scripts();
    SendGlobalSysMessage("DB table `creature_ai_scripts` reloaded.");
    return true;
}

bool ChatHandler::HandleReloadWpScriptsCommand(const char* arg)
{
    if (sWorld.IsScriptScheduled())
    {
        SendSysMessage("DB scripts used currently, please attempt reload later.");
        SetSentErrorMessage(true);
        return false;
    }

    if (*arg != 'a')
        sLog.outString("Re-Loading Scripts from waypoint_scripts...");

    objmgr.LoadWaypointScripts();

    if (*arg != 'a')
        SendGlobalGMSysMessage("DB table waypoint_scripts reloaded.");

    return true;
}

bool ChatHandler::HandleReloadQuestEndScriptsCommand(const char* arg)
{
    if (sWorld.IsScriptScheduled())
    {
        SendSysMessage("DB scripts used currently, please attempt reload later.");
        SetSentErrorMessage(true);
        return false;
    }

    if (*arg != 'a')
        sLog.outString("Re-Loading Scripts from quest_end_scripts...");

    objmgr.LoadQuestEndScripts();

    if (*arg != 'a')
        SendGlobalGMSysMessage("DB table quest_end_scripts reloaded.");

    return true;
}

bool ChatHandler::HandleReloadQuestStartScriptsCommand(const char* arg)
{
    if (sWorld.IsScriptScheduled())
    {
        SendSysMessage("DB scripts used currently, please attempt reload later.");
        SetSentErrorMessage(true);
        return false;
    }

    if (*arg != 'a')
        sLog.outString("Re-Loading Scripts from quest_start_scripts...");

    objmgr.LoadQuestStartScripts();

    if (*arg != 'a')
        SendGlobalGMSysMessage("DB table quest_start_scripts reloaded.");

    return true;
}

bool ChatHandler::HandleReloadSpellScriptsCommand(const char* arg)
{
    if (sWorld.IsScriptScheduled())
    {
        SendSysMessage("DB scripts used currently, please attempt reload later.");
        SetSentErrorMessage(true);
        return false;
    }

    if (*arg != 'a')
        sLog.outString("Re-Loading Scripts from spell_scripts...");

    objmgr.LoadSpellScripts();

    if (*arg != 'a')
        SendGlobalGMSysMessage("DB table spell_scripts reloaded.");

    return true;
}

bool ChatHandler::HandleReloadDbScriptStringCommand(const char* arg)
{
    sLog.outString("Re-Loading Script strings from db_script_string...");
    objmgr.LoadDbScriptStrings();
    SendGlobalGMSysMessage("DB table db_script_string reloaded.");
    return true;
}

bool ChatHandler::HandleReloadGameGraveyardZoneCommand(const char* /*arg*/)
{
    sLog.outString("Re-Loading Graveyard-zone links...");

    objmgr.LoadGraveyardZones();

    SendGlobalGMSysMessage("DB table game_graveyard_zone reloaded.");

    return true;
}

bool ChatHandler::HandleReloadGameTeleCommand(const char* /*arg*/)
{
    sLog.outString("Re-Loading Game Tele coordinates...");

    objmgr.LoadGameTele();

    SendGlobalGMSysMessage("DB table game_tele reloaded.");

    return true;
}

bool ChatHandler::HandleReloadSpellDisabledCommand(const char* /*arg*/)
{
    sLog.outString("Re-Loading spell disabled table...");

    objmgr.LoadSpellDisabledEntrys();

    SendGlobalGMSysMessage("DB table spell_disabled reloaded.");

    return true;
}

bool ChatHandler::HandleReloadLocalesCreatureCommand(const char* /*arg*/)
{
    sLog.outString("Re-Loading Locales Creature ...");
    objmgr.LoadCreatureLocales();
    SendGlobalGMSysMessage("DB table locales_creature reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLocalesGameobjectCommand(const char* /*arg*/)
{
    sLog.outString("Re-Loading Locales Gameobject ... ");
    objmgr.LoadGameObjectLocales();
    SendGlobalGMSysMessage("DB table locales_gameobject reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLocalesItemCommand(const char* /*arg*/)
{
    sLog.outString("Re-Loading Locales Item ... ");
    objmgr.LoadItemLocales();
    SendGlobalGMSysMessage("DB table locales_item reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLocalesNpcTextCommand(const char* /*arg*/)
{
    sLog.outString("Re-Loading Locales NPC Text ... ");
    objmgr.LoadNpcTextLocales();
    SendGlobalGMSysMessage("DB table locales_npc_text reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLocalesPageTextCommand(const char* /*arg*/)
{
    sLog.outString("Re-Loading Locales Page Text ... ");
    objmgr.LoadPageTextLocales();
    SendGlobalGMSysMessage("DB table locales_page_text reloaded.");
    return true;
}

bool ChatHandler::HandleReloadLocalesQuestCommand(const char* /*arg*/)
{
    sLog.outString("Re-Loading Locales Quest ... ");
    objmgr.LoadQuestLocales();
    SendGlobalGMSysMessage("DB table locales_quest reloaded.");
    return true;
}

bool ChatHandler::HandleReloadAuctionsCommand(const char *args)
{
    // Reload dynamic data tables from the database
    sLog.outString("Re-Loading Auctions...");
    sAuctionMgr->LoadAuctionItems();
    sAuctionMgr->LoadAuctions();
    SendGlobalGMSysMessage("Auctions reloaded.");
    return true;
}

bool ChatHandler::HandleAccountSetGmLevelCommand(const char *args)
{
    if (!*args)
        return false;

    std::string targetAccountName;
    uint32 targetAccountId = 0;
    uint32 targetSecurity = 0;
    uint32 gm = 0;
    char* arg1 = strtok((char*)args, " ");
    char* arg2 = strtok(NULL, " ");
    char* arg3 = strtok(NULL, " ");

    if (getSelectedPlayer() && arg1 && !arg3)
    {
        targetAccountId = getSelectedPlayer()->GetSession()->GetAccountId();
        sAccountMgr->GetName(targetAccountId, targetAccountName);
        Player* targetPlayer = getSelectedPlayer();
        gm = atoi(arg1);
        uint32 gmRealmID = arg2 ? atoi(arg2) : realmID;

        // Check for invalid specified GM level.
        if ((gm < SEC_PLAYER || gm > SEC_ADMINISTRATOR))
        {
            SendSysMessage(LANG_BAD_VALUE);
            SetSentErrorMessage(true);
            return false;
        }

        // Check if targets GM level and specified GM level is not higher than current gm level
        targetSecurity = targetPlayer->GetSession()->GetSecurity();
        if (targetSecurity >= m_session->GetSecurity() ||
            gm >= m_session->GetSecurity()             ||
            (gmRealmID != realmID && m_session->GetSecurity() < SEC_CONSOLE))
        {
            SendSysMessage(LANG_YOURS_SECURITY_IS_LOW);
            SetSentErrorMessage(true);
            return false;
        }

        // Decide which string to show
        if (m_session->GetPlayer() != targetPlayer)
        {
            PSendSysMessage(LANG_YOU_CHANGE_SECURITY, targetAccountName.c_str(), gm);
        }
        else
        {
            PSendSysMessage(LANG_YOURS_SECURITY_CHANGED, m_session->GetPlayer()->GetName(), gm);
        }

        // If gmRealmID is -1, delete all values for the account id, else, insert values for the specific realmID
        if (gmRealmID == -1)
        {
            LoginDatabase.PExecute("DELETE FROM account_access WHERE id = '%u'", targetAccountId);
            LoginDatabase.PExecute("INSERT INTO account_access VALUES ('%u', '%d', -1)", targetAccountId, gm);
        }
        else
        {
            LoginDatabase.PExecute("DELETE FROM account_access WHERE id = '%u' AND RealmID = '%d'", targetAccountId, realmID);
            LoginDatabase.PExecute("INSERT INTO account_access VALUES ('%u','%d','%d')", targetAccountId, gm, realmID);
        }        return true;
    }
    else
    {
        // Check for second parameter
        if (!arg2)
            return false;

        // Check for account
        targetAccountName = arg1;
        if (!AccountMgr::normalizeString(targetAccountName))
        {
            PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,targetAccountName.c_str());
            SetSentErrorMessage(true);
            return false;
        }

        // Check for username not exist
        targetAccountId = sAccountMgr->GetId(targetAccountName);
        if (!targetAccountId)
        {
            PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,targetAccountName.c_str());
            SetSentErrorMessage(true);
            return false;
        }

        // Check for invalid specified GM level.
        gm = atoi(arg2);
        if ((gm < SEC_PLAYER || gm > SEC_ADMINISTRATOR))
        {
            SendSysMessage(LANG_BAD_VALUE);
            SetSentErrorMessage(true);
            return false;
        }

        uint32 gmRealmID = arg3 ? atoi(arg3) : realmID;
        // Check if provided realmID is not current realmID, or isn't -1
        if (gmRealmID != realmID && gmRealmID != -1)
        {
            SendSysMessage(LANG_INVALID_REALMID);
            SetSentErrorMessage(true);
            return false;
        }

        targetAccountId = sAccountMgr->GetId(arg1);
        // m_session == NULL only for console
        uint32 plSecurity = m_session ? m_session->GetSecurity() : SEC_CONSOLE;

        // can set security level only for target with less security and to less security that we have
        // This is also reject self apply in fact
        targetSecurity = sAccountMgr->GetSecurity(targetAccountId);
        if (targetSecurity >= plSecurity || gm >= plSecurity)
        {
            SendSysMessage(LANG_YOURS_SECURITY_IS_LOW);
            SetSentErrorMessage(true);
            return false;
        }

        PSendSysMessage(LANG_YOU_CHANGE_SECURITY, targetAccountName.c_str(), gm);
        // If gmRealmID is -1, delete all values for the account id, else, insert values for the specific realmID
        if (gmRealmID == -1)
        {
            LoginDatabase.PExecute("DELETE FROM account_access WHERE id = '%u'", targetAccountId);
            LoginDatabase.PExecute("INSERT INTO account_access VALUES ('%u', '%d', -1)", targetAccountId, gm);
        }
        else
        {
            LoginDatabase.PExecute("DELETE FROM account_access WHERE id = '%u' AND RealmID = '%d'", targetAccountId, realmID);
            LoginDatabase.PExecute("INSERT INTO account_access VALUES ('%u','%d','%d')", targetAccountId, gm, realmID);
        }        return true;
    }
}

// Set password for account
bool ChatHandler::HandleAccountSetPasswordCommand(const char *args)
{
    if (!*args)
        return false;

    // Get the command line arguments
    char *szAccount = strtok ((char*)args," ");
    char *szPassword1 =  strtok (NULL," ");
    char *szPassword2 =  strtok (NULL," ");

    if (!szAccount||!szPassword1 || !szPassword2)
        return false;

    std::string account_name = szAccount;
    if (!AccountMgr::normalizeString(account_name))
    {
        PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,account_name.c_str());
        SetSentErrorMessage(true);
        return false;
    }

    uint32 targetAccountId = sAccountMgr->GetId(account_name);
    if (!targetAccountId)
    {
        PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,account_name.c_str());
        SetSentErrorMessage(true);
        return false;
    }

    uint32 targetSecurity = sAccountMgr->GetSecurity(targetAccountId);

    // m_session == NULL only for console
    uint32 plSecurity = m_session ? m_session->GetSecurity() : SEC_CONSOLE;

    // can set password only for target with less security
    // This is also reject self apply in fact
    if (targetSecurity >= plSecurity)
    {
        SendSysMessage (LANG_YOURS_SECURITY_IS_LOW);
        SetSentErrorMessage (true);
        return false;
    }

    if (strcmp(szPassword1,szPassword2))
    {
        SendSysMessage (LANG_NEW_PASSWORDS_NOT_MATCH);
        SetSentErrorMessage (true);
        return false;
    }

    AccountOpResult result = sAccountMgr->ChangePassword(targetAccountId, szPassword1);

    switch (result)
    {
        case AOR_OK:
            SendSysMessage(LANG_COMMAND_PASSWORD);
            break;
        case AOR_NAME_NOT_EXIST:
            PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,account_name.c_str());
            SetSentErrorMessage(true);
            return false;
        case AOR_PASS_TOO_LONG:
            SendSysMessage(LANG_PASSWORD_TOO_LONG);
            SetSentErrorMessage(true);
            return false;
        default:
            SendSysMessage(LANG_COMMAND_NOTCHANGEPASSWORD);
            SetSentErrorMessage(true);
            return false;
    }

    return true;
}

bool ChatHandler::HandleGUIDCommand(const char* /*args*/)
{
    uint64 guid = m_session->GetPlayer()->GetSelection();

    if (guid == 0)
    {
        SendSysMessage(LANG_NO_SELECTION);
        SetSentErrorMessage(true);
        return false;
    }

    PSendSysMessage(LANG_OBJECT_GUID, GUID_LOPART(guid), GUID_HIPART(guid));
    return true;
}

bool ChatHandler::HandleItemMoveCommand(const char* args)
{
    if (!*args)
        return false;
    uint8 srcslot, dstslot;

    char* pParam1 = strtok((char*)args, " ");
    if (!pParam1)
        return false;

    char* pParam2 = strtok(NULL, " ");
    if (!pParam2)
        return false;

    srcslot = (uint8)atoi(pParam1);
    dstslot = (uint8)atoi(pParam2);

    if (srcslot == dstslot)
        return true;

    if (!m_session->GetPlayer()->IsValidPos(INVENTORY_SLOT_BAG_0,srcslot))
        return false;

    if (!m_session->GetPlayer()->IsValidPos(INVENTORY_SLOT_BAG_0,dstslot))
        return false;

    uint16 src = ((INVENTORY_SLOT_BAG_0 << 8) | srcslot);
    uint16 dst = ((INVENTORY_SLOT_BAG_0 << 8) | dstslot);

    m_session->GetPlayer()->SwapItem(src, dst);

    return true;
}

bool ChatHandler::HandleMod32Value(const char *args)
{
    if (!*args)
        return false;

    char* px = strtok((char*)args, " ");
    char* py = strtok(NULL, " ");

    if (!px || !py)
        return false;

    uint32 Opcode = (uint32)atoi(px);
    int Value = atoi(py);

    if (Opcode >= m_session->GetPlayer()->GetValuesCount())
    {
        PSendSysMessage(LANG_TOO_BIG_INDEX, Opcode, m_session->GetPlayer()->GetGUIDLow(), m_session->GetPlayer()->GetValuesCount());
        return false;
    }

 // sLog.outDebug(GetBlizzLikeString(LANG_CHANGE_32BIT), Opcode, Value);

    int CurrentValue = (int)m_session->GetPlayer()->GetUInt32Value(Opcode);

    CurrentValue += Value;
    m_session->GetPlayer()->SetUInt32Value(Opcode , (uint32)CurrentValue);

    PSendSysMessage(LANG_CHANGE_32BIT_FIELD, Opcode,CurrentValue);

    return true;
}

bool ChatHandler::HandleModifyBitCommand(const char* args)
{
    if (!*args)
        return false;

    Player* target = getSelectedPlayer();
    if (!target)
    {
        SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
    }

    char* pField = strtok((char*)args, " ");
    if (!pField)
        return false;

    char* pBit = strtok(NULL, " ");
    if (!pBit)
        return false;

    uint16 field = atoi(pField);
    uint32 bit   = atoi(pBit);

    if (field < 1 || field >= PLAYER_END)
    {
        SendSysMessage(LANG_BAD_VALUE);
        SetSentErrorMessage(true);
        return false;
    }
    if (bit < 1 || bit > 32)
    {
        SendSysMessage(LANG_BAD_VALUE);
        SetSentErrorMessage(true);
        return false;
    }

    if (target->HasFlag(field, (1<<(bit-1))))
    {
        target->RemoveFlag(field, (1<<(bit-1)));
        PSendSysMessage(LANG_REMOVE_BIT, bit, field);
    }
    else
    {
        target->SetFlag(field, (1<<(bit-1)));
        PSendSysMessage(LANG_SET_BIT, bit, field);
    }
    return true;
}

bool ChatHandler::HandleModifySpellCommand(const char* args)
{
    if (!*args) return false;
    char* pspellflatid = strtok((char*)args, " ");
    if (!pspellflatid)
        return false;

    char* pop = strtok(NULL, " ");
    if (!pop)
        return false;

    char* pval = strtok(NULL, " ");
    if (!pval)
        return false;

    uint16 mark;

    char* pmark = strtok(NULL, " ");

    uint8 spellflatid = atoi(pspellflatid);
    uint8 op   = atoi(pop);
    uint16 val = atoi(pval);
    if (!pmark)
        mark = 65535;
    else
        mark = atoi(pmark);

    Player* target = getSelectedPlayer();
    if (target == NULL)
    {
        SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
    }

    PSendSysMessage(LANG_YOU_CHANGE_SPELLFLATID, spellflatid, val, mark, target->GetName());
    if (needReportToTarget(target))
        ChatHandler(target).PSendSysMessage(LANG_YOURS_SPELLFLATID_CHANGED, GetName(), spellflatid, val, mark);

    WorldPacket data(SMSG_SET_FLAT_SPELL_MODIFIER, (1+1+2+2));
    data << uint8(spellflatid);
    data << uint8(op);
    data << uint16(val);
    data << uint16(mark);
    target->GetSession()->SendPacket(&data);

    return true;
}

bool ChatHandler::HandleAddVendorItemCommand(const char* args)
{
    if (!*args)
        return false;

    char* pitem  = extractKeyFromLink((char*)args,"Hitem");
    if (!pitem)
    {
        SendSysMessage(LANG_COMMAND_NEEDITEMSEND);
        SetSentErrorMessage(true);
        return false;
    }

    uint32 itemId = atol(pitem);

    char* fmaxcount = strtok(NULL, " ");                    //add maxcount, default: 0
    uint32 maxcount = 0;
    if (fmaxcount)
        maxcount = atol(fmaxcount);

    char* fincrtime = strtok(NULL, " ");                    //add incrtime, default: 0
    uint32 incrtime = 0;
    if (fincrtime)
        incrtime = atol(fincrtime);

    char* fextendedcost = strtok(NULL, " ");                //add ExtendedCost, default: 0
    uint32 extendedcost = fextendedcost ? atol(fextendedcost) : 0;

    Creature* vendor = getSelectedCreature();

    uint32 vendor_entry = vendor ? vendor->GetEntry() : 0;

    if (!objmgr.IsVendorItemValid(vendor_entry,itemId,maxcount,incrtime,extendedcost,m_session->GetPlayer()))
    {
        SetSentErrorMessage(true);
        return false;
    }

    objmgr.AddVendorItem(vendor_entry,itemId,maxcount,incrtime,extendedcost);

    ItemPrototype const* pProto = objmgr.GetItemPrototype(itemId);

    PSendSysMessage(LANG_ITEM_ADDED_TO_LIST,itemId,pProto->Name1,maxcount,incrtime,extendedcost);
    return true;
}

bool ChatHandler::HandleDelVendorItemCommand(const char* args)
{
    if (!*args)
        return false;

    Creature* vendor = getSelectedCreature();
    if (!vendor || !vendor->isVendor())
    {
        SendSysMessage(LANG_COMMAND_VENDORSELECTION);
        SetSentErrorMessage(true);
        return false;
    }

    char* pitem  = extractKeyFromLink((char*)args,"Hitem");
    if (!pitem)
    {
        SendSysMessage(LANG_COMMAND_NEEDITEMSEND);
        SetSentErrorMessage(true);
        return false;
    }
    uint32 itemId = atol(pitem);

    if (!objmgr.RemoveVendorItem(vendor->GetEntry(),itemId))
    {
        PSendSysMessage(LANG_ITEM_NOT_IN_LIST,itemId);
        SetSentErrorMessage(true);
        return false;
    }

    ItemPrototype const* pProto = objmgr.GetItemPrototype(itemId);

    PSendSysMessage(LANG_ITEM_DELETED_FROM_LIST,itemId,pProto->Name1);
    return true;
}

bool ChatHandler::HandleWpShowCommand(const char* args)
{
    if (!*args)
        return false;

    // first arg: on, off, first, last
    char* show_str = strtok((char*)args, " ");
    if (!show_str)
        return false;

    // second arg: GUID (optional, if a creature is selected)
    char* guid_str = strtok((char*)NULL, " ");
    sLog.outDebug("DEBUG: HandleWpShowCommand: show_str: %s guid_str: %s", show_str, guid_str);

    uint32 pathid = 0;
    Creature* target = getSelectedCreature();

    // Did player provide a PathID?

    if (!guid_str)
    {
        sLog.outDebug("DEBUG: HandleWpShowCommand: !guid_str");
        // No PathID provided
        // -> Player must have selected a creature

        if (!target)
        {
            SendSysMessage(LANG_SELECT_CREATURE);
            SetSentErrorMessage(true);
            return false;
        }

        pathid = target->GetWaypointPath();
    }
    else
    {
        sLog.outDebug("|cff00ff00DEBUG: HandleWpShowCommand: PathID provided|r");
        // PathID provided
        // Warn if player also selected a creature
        // -> Creature selection is ignored <-
        if (target)
            SendSysMessage(LANG_WAYPOINT_CREATSELECTED);

        pathid = atoi((char*)guid_str);
    }

    sLog.outDebug("DEBUG: HandleWpShowCommand: danach");

    std::string show = show_str;
    uint32 Maxpoint;

    sLog.outDebug("DEBUG: HandleWpShowCommand: PathID: %u", pathid);

    //PSendSysMessage("wpshow - show: %s", show);

    // Show info for the selected waypoint
    if (show == "info")
    {
        // Check if the user did specify a visual waypoint
        if (target->GetEntry() != VISUAL_WAYPOINT)
        {
            PSendSysMessage(LANG_WAYPOINT_VP_SELECT);
            SetSentErrorMessage(true);
            return false;
        }

        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT id, point, delay, move_flag, action, action_chance FROM waypoint_data WHERE wpguid = %u", target->GetGUIDLow());

        if (!result)
        {
            SendSysMessage(LANG_WAYPOINT_NOTFOUNDDBPROBLEM);
            return true;
        }

        SendSysMessage("|cff00ffffDEBUG: wp show info:|r");
        do
        {
            Field *fields = result->Fetch();
            pathid                  = fields[0].GetUInt32();
            uint32 point            = fields[1].GetUInt32();
            uint32 delay            = fields[2].GetUInt32();
            uint32 flag             = fields[3].GetUInt32();
            uint32 ev_id            = fields[4].GetUInt32();
            uint32 ev_chance        = fields[5].GetUInt32();

            PSendSysMessage("|cff00ff00Show info: for current point: |r|cff00ffff%u|r|cff00ff00, Path ID: |r|cff00ffff%u|r", point, pathid);
            PSendSysMessage("|cff00ff00Show info: delay: |r|cff00ffff%u|r", delay);
            PSendSysMessage("|cff00ff00Show info: Move flag: |r|cff00ffff%u|r", flag);
            PSendSysMessage("|cff00ff00Show info: Waypoint event: |r|cff00ffff%u|r", ev_id);
            PSendSysMessage("|cff00ff00Show info: Event chance: |r|cff00ffff%u|r", ev_chance);
            }
            while (result->NextRow());

        return true;
    }

    if (show == "on")
    {
        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT point, position_x,position_y,position_z FROM waypoint_data WHERE id = '%u'", pathid);

        if (!result)
        {
            SendSysMessage("|cffff33ffPath no found.|r");
            SetSentErrorMessage(true);
            return false;
        }

        PSendSysMessage("|cff00ff00DEBUG: wp on, PathID: |cff00ffff%u|r", pathid);

        // Delete all visuals for this NPC
        QueryResult_AutoPtr result2 = WorldDatabase.PQuery("SELECT wpguid FROM waypoint_data WHERE id = '%u' and wpguid <> 0", pathid);

        if (result2)
        {
            bool hasError = false;
            do
            {
                Field *fields = result2->Fetch();
                uint32 wpguid = fields[0].GetUInt32();
                Creature* pCreature = m_session->GetPlayer()->GetMap()->GetCreature(MAKE_NEW_GUID(wpguid,VISUAL_WAYPOINT,HIGHGUID_UNIT));

                if (!pCreature)
                {
                    PSendSysMessage(LANG_WAYPOINT_NOTREMOVED, wpguid);
                    hasError = true;
                    WorldDatabase.PExecuteLog("DELETE FROM creature WHERE guid = '%u'", wpguid);
                }
                else
                {
                    pCreature->CombatStop();
                    pCreature->DeleteFromDB();
                    pCreature->AddObjectToRemoveList();
                }

            }
            while (result2->NextRow());

            if (hasError)
            {
                PSendSysMessage(LANG_WAYPOINT_TOOFAR1);
                PSendSysMessage(LANG_WAYPOINT_TOOFAR2);
                PSendSysMessage(LANG_WAYPOINT_TOOFAR3);
            }
        }

        do
        {
            Field *fields = result->Fetch();
            uint32 point    = fields[0].GetUInt32();
            float x         = fields[1].GetFloat();
            float y         = fields[2].GetFloat();
            float z         = fields[3].GetFloat();

            uint32 id = VISUAL_WAYPOINT;

            Player* chr = m_session->GetPlayer();
            Map *map = chr->GetMap();
            float o = chr->GetOrientation();

            Creature* wpCreature = new Creature;
            if (!wpCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_UNIT), map, id, 0, x, y, z, o))
            {
                PSendSysMessage(LANG_WAYPOINT_VP_NOTCREATED, id);
                delete wpCreature;
                return false;
            }

            sLog.outDebug("DEBUG: UPDATE waypoint_data SET wpguid = '%u'", wpCreature->GetGUIDLow());
            // set "wpguid" column to the visual waypoint
            WorldDatabase.PExecuteLog("UPDATE waypoint_data SET wpguid = '%u' WHERE id = '%u' and point = '%u'", wpCreature->GetGUIDLow(), pathid, point);

            wpCreature->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()));
            wpCreature->LoadFromDB(wpCreature->GetDBTableGUIDLow(),map);
            map->Add(wpCreature);

            if (target)
            {
                wpCreature->SetDisplayId(target->GetDisplayId());
                wpCreature->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5);
            }
        }
        while (result->NextRow());

        SendSysMessage("|cff00ff00Showing the current creature's path.|r");
        return true;
    }

    if (show == "first")
    {
        PSendSysMessage("|cff00ff00DEBUG: wp first, GUID: %u|r", pathid);

        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT position_x,position_y,position_z FROM waypoint_data WHERE point='1' AND id = '%u'",pathid);
        if (!result)
        {
            PSendSysMessage(LANG_WAYPOINT_NOTFOUND, pathid);
            SetSentErrorMessage(true);
            return false;
        }

        Field *fields = result->Fetch();
        float x         = fields[0].GetFloat();
        float y         = fields[1].GetFloat();
        float z         = fields[2].GetFloat();
        uint32 id = VISUAL_WAYPOINT;

        Player* chr = m_session->GetPlayer();
        float o = chr->GetOrientation();
        Map *map = chr->GetMap();

        Creature* pCreature = new Creature;
        if (!pCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_UNIT),map, id, 0, x, y, z, o))
        {
            PSendSysMessage(LANG_WAYPOINT_VP_NOTCREATED, id);
            delete pCreature;
            return false;
        }

        pCreature->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()));
        pCreature->LoadFromDB(pCreature->GetDBTableGUIDLow(), map);
        map->Add(pCreature);

        if (target)
        {
            pCreature->SetDisplayId(target->GetDisplayId());
            pCreature->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5);
        }

        return true;
    }

    if (show == "last")
    {
        PSendSysMessage("|cff00ff00DEBUG: wp last, PathID: |r|cff00ffff%u|r", pathid);

        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT MAX(point) FROM waypoint_data WHERE id = '%u'",pathid);
        if (result)
            Maxpoint = (*result)[0].GetUInt32();
        else
            Maxpoint = 0;

        result = WorldDatabase.PQuery("SELECT position_x,position_y,position_z FROM waypoint_data WHERE point ='%u' AND id = '%u'",Maxpoint, pathid);
        if (!result)
        {
            PSendSysMessage(LANG_WAYPOINT_NOTFOUNDLAST, pathid);
            SetSentErrorMessage(true);
            return false;
        }
        Field *fields = result->Fetch();
        float x         = fields[0].GetFloat();
        float y         = fields[1].GetFloat();
        float z         = fields[2].GetFloat();
        uint32 id = VISUAL_WAYPOINT;

        Player* chr = m_session->GetPlayer();
        float o = chr->GetOrientation();
        Map *map = chr->GetMap();

        Creature* pCreature = new Creature;
        if (!pCreature->Create(objmgr.GenerateLowGuid(HIGHGUID_UNIT), map, id, 0, x, y, z, o))
        {
            PSendSysMessage(LANG_WAYPOINT_NOTCREATED, id);
            delete pCreature;
            return false;
        }

        pCreature->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()));
        pCreature->LoadFromDB(pCreature->GetDBTableGUIDLow(), map);
        map->Add(pCreature);

        if (target)
        {
            pCreature->SetDisplayId(target->GetDisplayId());
            pCreature->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.5);
        }

        return true;
    }

    if (show == "off")
    {
        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT guid FROM creature WHERE id = '%u'", 1);
        if (!result)
        {
            SendSysMessage(LANG_WAYPOINT_VP_NOTFOUND);
            SetSentErrorMessage(true);
            return false;
        }
        bool hasError = false;
        do
        {
            Field *fields = result->Fetch();
            uint32 guid = fields[0].GetUInt32();
            Creature* pCreature = m_session->GetPlayer()->GetMap()->GetCreature(MAKE_NEW_GUID(guid,VISUAL_WAYPOINT,HIGHGUID_UNIT));
            if (!pCreature)
            {
                PSendSysMessage(LANG_WAYPOINT_NOTREMOVED, guid);
                hasError = true;
                WorldDatabase.PExecuteLog("DELETE FROM creature WHERE guid = '%u'", guid);
            }
            else
            {
                pCreature->CombatStop();
                pCreature->DeleteFromDB();
                pCreature->AddObjectToRemoveList();
            }
        }
        while (result->NextRow());
        // set "wpguid" column to "empty" - no visual waypoint spawned
        WorldDatabase.PExecuteLog("UPDATE waypoint_data SET wpguid = '0'");
        //WorldDatabase.PExecuteLog("UPDATE creature_movement SET wpguid = '0' WHERE wpguid <> '0'");

        if (hasError)
        {
            PSendSysMessage(LANG_WAYPOINT_TOOFAR1);
            PSendSysMessage(LANG_WAYPOINT_TOOFAR2);
            PSendSysMessage(LANG_WAYPOINT_TOOFAR3);
        }

        SendSysMessage(LANG_WAYPOINT_VP_ALLREMOVED);
        return true;
    }

    PSendSysMessage("|cffff33ffDEBUG: wpshow - no valid command found|r");
    return true;
}

bool ChatHandler::HandleWpAddCommand(const char* args)
{
    uint32 pathid = 0;
    uint32 point = 0;
    uint32 wpdelay = 0;

    if (*args)
    {
        char* path_number = strtok((char*)args, " ");
        pathid = atoi(path_number);
        if (!pathid)
        {
            PSendSysMessage("%s%s|r", "|cffff33ff", "Invalid creature GUID.");
            return true;
        }
        char* wp_delay = strtok((char*)NULL, " ");
        if (wp_delay)
            wpdelay = atoi(wp_delay);
        PSendSysMessage("%s%s|r", "|cff00ff00", "Path added to the creature:");
    }
    else
    {
    Creature* target = getSelectedCreature();
        if (target)
        {
            pathid = target->GetGUIDLow();
            PSendSysMessage("%s%s|r", "|cff00ff00", "Path added to the selected creature:");
        }
        else
        {
            QueryResult_AutoPtr result = WorldDatabase.Query("SELECT MAX(id) FROM waypoint_data");
            uint32 maxpathid = result->Fetch()->GetInt32();
            pathid = maxpathid+1;
            PSendSysMessage("%s%s|r", "|cff00ff00", "New path started.");
        }
    }

    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT MAX(point) FROM waypoint_data WHERE id = '%u'",pathid);

    if (result)
        point = (*result)[0].GetUInt32();

    Player* player = m_session->GetPlayer();

    WorldDatabase.PExecuteLog("INSERT INTO waypoint_data (id, point, position_x, position_y, position_z, delay) VALUES ('%u', '%u', '%f', '%f', '%f', '%u')",
        pathid, point+1, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), wpdelay);

    PSendSysMessage("%s%s%u%s%u%s%u%s|r", "|cff00ff00", "PathID: |r|cff00ffff", pathid, "|r|cff00ff00 Waypoint: |r|cff00ffff", point, "|r|cff00ff00 Delay: |r|cff00ffff", wpdelay, "|r|cff00ff00 created. ");
    return true;
}

bool ChatHandler::HandleWpLoadPathCommand(const char *args)
{
    if (!*args)
        return false;

    // optional
    char* path_number = NULL;

    if (*args)
        path_number = strtok((char*)args, " ");

    uint32 pathid = 0;
    uint32 guidlow = 0;
    Creature* target = getSelectedCreature();

    // Did player provide a path_id?
    if (!path_number)
        sLog.outDebug("DEBUG: HandleWpLoadPathCommand - No path number provided");

    if (!target)
    {
        SendSysMessage(LANG_SELECT_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    if (target->GetEntry() == 1)
    {
        PSendSysMessage("%s%s|r", "|cffff33ff", "You want to load path to a waypoint? Aren't you?");
        SetSentErrorMessage(true);
        return false;
    }

    pathid = atoi(path_number);

    if (!pathid)
    {
        PSendSysMessage("%s%s|r", "|cffff33ff", "No vallid path number provided.");
        return true;
    }

    guidlow = target->GetDBTableGUIDLow();
    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT guid FROM creature_addon WHERE guid = '%u'",guidlow);

    if (result)
        WorldDatabase.PExecuteLog("UPDATE creature_addon SET path_id = '%u' WHERE guid = '%u'", pathid, guidlow);
    else
        WorldDatabase.PExecuteLog("INSERT INTO creature_addon(guid,path_id) VALUES ('%u','%u')", guidlow, pathid);

    WorldDatabase.PExecuteLog("UPDATE creature SET MovementType = '%u' WHERE guid = '%u'", WAYPOINT_MOTION_TYPE, guidlow);

    target->LoadPath(pathid);
    target->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
    target->GetMotionMaster()->Initialize();
    target->Say("Path loaded.",0,0);

    return true;
}

bool ChatHandler::HandleWpModifyCommand(const char* args)
{
    if (!*args)
        return false;

    // first arg: add del text emote spell waittime move
    char* show_str = strtok((char*)args, " ");
    if (!show_str)
    {
        return false;
    }

    std::string show = show_str;
    // Check
    // Remember: "show" must also be the name of a column!
    if ((show != "delay") && (show != "action") && (show != "action_chance")
        && (show != "move_flag") && (show != "del") && (show != "move") && (show != "wpadd")
)
    {
        return false;
    }

    // Next arg is: <PATHID> <WPNUM> <ARGUMENT>
    char* arg_str = NULL;

    // Did user provide a GUID
    // or did the user select a creature?
    // -> variable lowguid is filled with the GUID of the NPC
    uint32 pathid = 0;
    uint32 point = 0;
    uint32 wpGuid = 0;
    Creature* target = getSelectedCreature();

    if (!target || target->GetEntry() != VISUAL_WAYPOINT)
    {
        SendSysMessage("|cffff33ffERROR: You must select a waypoint.|r");
        return false;
    }

    // The visual waypoint
    Creature* wpCreature = NULL;
    wpGuid = target->GetGUIDLow();

    // Did the user select a visual spawnpoint?
    if (wpGuid)
        wpCreature = m_session->GetPlayer()->GetMap()->GetCreature(MAKE_NEW_GUID(wpGuid, VISUAL_WAYPOINT, HIGHGUID_UNIT));
    // attempt check creature existence by DB data
    else
    {
        PSendSysMessage(LANG_WAYPOINT_CREATNOTFOUND, wpGuid);
        return false;
    }
    // User did select a visual waypoint?
    // Check the creature
    if (wpCreature->GetEntry() == VISUAL_WAYPOINT)
    {
        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT id, point FROM waypoint_data WHERE wpguid = %u", wpGuid);

        if (!result)
        {
            // No waypoint found - used 'wpguid'

            PSendSysMessage(LANG_WAYPOINT_NOTFOUNDSEARCH, target->GetGUIDLow());
            // Select waypoint number from database
            // Since we compare float values, we have to deal with
            // some difficulties.
            // Here we search for all waypoints that only differ in one from 1 thousand
            // (0.001) - There is no other way to compare C++ floats with mySQL floats
            // See also: http://dev.mysql.com/doc/refman/5.0/en/problems-with-float.html
            const char* maxDIFF = "0.01";
            result = WorldDatabase.PQuery("SELECT id, point FROM waypoint_data WHERE (abs(position_x - %f) <= %s) and (abs(position_y - %f) <= %s) and (abs(position_z - %f) <= %s)",
            wpCreature->GetPositionX(), maxDIFF, wpCreature->GetPositionY(), maxDIFF, wpCreature->GetPositionZ(), maxDIFF);
            if (!result)
            {
                    PSendSysMessage(LANG_WAYPOINT_NOTFOUNDDBPROBLEM, wpGuid);
                    return true;
            }
        }
        // After getting wpGuid

        do
        {
            Field *fields = result->Fetch();
            pathid = fields[0].GetUInt32();
            point  = fields[1].GetUInt32();
        }
        while (result->NextRow());

        // We have the waypoint number and the GUID of the "master npc"
        // Text is enclosed in "<>", all other arguments not
        arg_str = strtok((char*)NULL, " ");
    }

    // Parameters parsed - now execute the command

    // Check for argument
    if (show != "del" && show != "move" && arg_str == NULL)
    {
        PSendSysMessage(LANG_WAYPOINT_ARGUMENTREQ, show_str);
        return false;
    }

    if (show == "del" && target)
    {
        PSendSysMessage("|cff00ff00DEBUG: wp modify del, PathID: |r|cff00ffff%u|r", pathid);

        // wpCreature
        Creature* wpCreature = NULL;

        if (wpGuid != 0)
        {
            wpCreature = m_session->GetPlayer()->GetMap()->GetCreature(MAKE_NEW_GUID(wpGuid, VISUAL_WAYPOINT, HIGHGUID_UNIT));
            wpCreature->CombatStop();
            wpCreature->DeleteFromDB();
            wpCreature->AddObjectToRemoveList();
        }

        WorldDatabase.PExecuteLog("DELETE FROM waypoint_data WHERE id='%u' AND point='%u'",
            pathid, point);
        WorldDatabase.PExecuteLog("UPDATE waypoint_data SET point=point-1 WHERE id='%u' AND point>'%u'",
            pathid, point);

        PSendSysMessage(LANG_WAYPOINT_REMOVED);
        return true;
    }                                                       // del

    if (show == "move" && target)
    {
        PSendSysMessage("|cff00ff00DEBUG: wp move, PathID: |r|cff00ffff%u|r", pathid);

        Player* chr = m_session->GetPlayer();
        Map *map = chr->GetMap();
        {
            // wpCreature
            Creature* wpCreature = NULL;
            // What to do:
            // Move the visual spawnpoint
            // Respawn the owner of the waypoints
            if (wpGuid != 0)
            {
                wpCreature = m_session->GetPlayer()->GetMap()->GetCreature(MAKE_NEW_GUID(wpGuid, VISUAL_WAYPOINT, HIGHGUID_UNIT));
                wpCreature->CombatStop();
                wpCreature->DeleteFromDB();
                wpCreature->AddObjectToRemoveList();
                // re-create
                Creature* wpCreature2 = new Creature;
                if (!wpCreature2->Create(objmgr.GenerateLowGuid(HIGHGUID_UNIT), map, VISUAL_WAYPOINT, 0, chr->GetPositionX(), chr->GetPositionY(), chr->GetPositionZ(), chr->GetOrientation()))
                {
                    PSendSysMessage(LANG_WAYPOINT_VP_NOTCREATED, VISUAL_WAYPOINT);
                    delete wpCreature2;
                    return false;
                }

                wpCreature2->SaveToDB(map->GetId(), (1 << map->GetSpawnMode()));
                // To call _LoadGoods(); _LoadQuests(); CreateTrainerSpells();
                wpCreature2->LoadFromDB(wpCreature2->GetDBTableGUIDLow(), map);
                map->Add(wpCreature2);
            }

            WorldDatabase.PExecuteLog("UPDATE waypoint_data SET position_x = '%f',position_y = '%f',position_z = '%f' where id = '%u' AND point='%u'",
                chr->GetPositionX(), chr->GetPositionY(), chr->GetPositionZ(), pathid, point);

            PSendSysMessage(LANG_WAYPOINT_CHANGED);
        }
        return true;
    }                                                       // move

    const char *text = arg_str;

    if (text == 0)
    {
        // show_str check for present in list of correct values, no sql injection possible
        WorldDatabase.PExecuteLog("UPDATE waypoint_data SET %s=NULL WHERE id='%u' AND point='%u'",
            show_str, pathid, point);
    }
    else
    {
        // show_str check for present in list of correct values, no sql injection possible
        std::string text2 = text;
        WorldDatabase.escape_string(text2);
        WorldDatabase.PExecuteLog("UPDATE waypoint_data SET %s='%s' WHERE id='%u' AND point='%u'",
            show_str, text2.c_str(), pathid, point);
    }

    PSendSysMessage(LANG_WAYPOINT_CHANGED_NO, show_str);
    return true;
}

bool ChatHandler::HandleWpEventCommand(const char* args)
{
    if (!*args)
        return false;

    char* show_str = strtok((char*)args, " ");
    std::string show = show_str;

    // Check
    if ((show != "add") && (show != "mod") && (show != "del") && (show != "listid")) return false;

    char* arg_id = strtok(NULL, " ");
    uint32 id = 0;

    if (show == "add")
    {
        if (arg_id)
            id = atoi(arg_id);

        if (id)
        {
            QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT id FROM waypoint_scripts WHERE guid = %u", id);

            if (!result)
            {
                WorldDatabase.PExecuteLog("INSERT INTO waypoint_scripts(guid)VALUES(%u)", id);
                PSendSysMessage("%s%s%u|r", "|cff00ff00", "Wp Event: New waypoint event added: ", id);
            }
            else
                PSendSysMessage("|cff00ff00Wp Event: You have chosen an existing waypoint script guid: %u|r", id);
        }
        else
        {
            QueryResult_AutoPtr result = WorldDatabase.Query("SELECT MAX(guid) FROM waypoint_scripts");
            id = result->Fetch()->GetUInt32();
            WorldDatabase.PExecuteLog("INSERT INTO waypoint_scripts(guid)VALUES(%u)", id+1);
            PSendSysMessage("%s%s%u|r", "|cff00ff00","Wp Event: New waypoint event added: |r|cff00ffff", id+1);
        }

        return true;
    }

    if (show == "listid")
    {
        if (!arg_id)
        {
            PSendSysMessage("%s%s|r", "|cff33ffff","Wp Event: You must provide waypoint script id.");
            return true;
        }

        id = atoi(arg_id);

        uint32 a2, a3, a4, a5, a6;
        float a8, a9, a10, a11;
        char const* a7;

        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT guid, delay, command, datalong, datalong2, dataint, x, y, z, o FROM waypoint_scripts WHERE id = %u", id);

        if (!result)
        {
            PSendSysMessage("%s%s%u|r", "|cff33ffff", "Wp Event: No waypoint scripts found on id: ", id);
            return true;
        }

        Field *fields;

        do
        {
            fields = result->Fetch();
            a2 = fields[0].GetUInt32();
            a3 = fields[1].GetUInt32();
            a4 = fields[2].GetUInt32();
            a5 = fields[3].GetUInt32();
            a6 = fields[4].GetUInt32();
            a7 = fields[5].GetString();
            a8 = fields[6].GetFloat();
            a9 = fields[7].GetFloat();
            a10 = fields[8].GetFloat();
            a11 = fields[9].GetFloat();

            PSendSysMessage("|cffff33ffid:|r|cff00ffff %u|r|cff00ff00, guid: |r|cff00ffff%u|r|cff00ff00, delay: |r|cff00ffff%u|r|cff00ff00, command: |r|cff00ffff%u|r|cff00ff00, datalong: |r|cff00ffff%u|r|cff00ff00, datalong2: |r|cff00ffff%u|r|cff00ff00, datatext: |r|cff00ffff%s|r|cff00ff00, posx: |r|cff00ffff%f|r|cff00ff00, posy: |r|cff00ffff%f|r|cff00ff00, posz: |r|cff00ffff%f|r|cff00ff00, orientation: |r|cff00ffff%f|r", id, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
        }
        while (result->NextRow());
    }

    if (show == "del")
    {
        id = atoi(arg_id);

        QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT guid FROM waypoint_scripts WHERE guid = %u", id);

        if (result)
        {
           WorldDatabase.PExecuteLog("DELETE FROM waypoint_scripts WHERE guid = %u", id);
           PSendSysMessage("%s%s%u|r","|cff00ff00","Wp Event: Waypoint script removed: ", id);
        }
        else
            PSendSysMessage("|cffff33ffWp Event: ERROR: you have selected an invalid script: %u|r", id);

        return true;
    }

    if (show == "mod")
    {
        if (!arg_id)
        {
            SendSysMessage("|cffff33ffERROR: Waypoint script guid not present.|r");
            return true;
        }

        id = atoi(arg_id);

        if (!id)
        {
            SendSysMessage("|cffff33ffERROR: No vallid waypoint script id not present.|r");
            return true;
        }

        char* arg_2 = strtok(NULL," ");

        if (!arg_2)
        {
            SendSysMessage("|cffff33ffERROR: No argument present.|r");
            return true;
        }

        std::string arg_string  = arg_2;

        if ((arg_string != "setid") && (arg_string != "delay") && (arg_string != "command")
        && (arg_string != "datalong") && (arg_string != "datalong2") && (arg_string != "dataint") && (arg_string != "posx")
        && (arg_string != "posy") && (arg_string != "posz") && (arg_string != "orientation"))
        {
            SendSysMessage("|cffff33ffERROR: No valid argument present.|r");
            return true;
        }

        char* arg_3;
        std::string arg_str_2 = arg_2;
        arg_3 = strtok(NULL," ");

        if (!arg_3)
        {
            SendSysMessage("|cffff33ffERROR: No additional argument present.|r");
            return true;
        }

        float coord;

        if (arg_str_2 == "setid")
        {
            uint32 newid = atoi(arg_3);
            PSendSysMessage("%s%s|r|cff00ffff%u|r|cff00ff00%s|r|cff00ffff%u|r","|cff00ff00","Wp Event: Wypoint scipt guid: ", newid," id changed: ", id);
            WorldDatabase.PExecuteLog("UPDATE waypoint_scripts SET id='%u' WHERE guid='%u'",
            newid, id); return true;
        }
        else
        {
            QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT id FROM waypoint_scripts WHERE guid='%u'",id);

            if (!result)
            {
                SendSysMessage("|cffff33ffERROR: You have selected an invalid waypoint script guid.|r");
                return true;
            }

            if (arg_str_2 == "posx")
            {
                coord = atof(arg_3);
                WorldDatabase.PExecuteLog("UPDATE waypoint_scripts SET x='%f' WHERE guid='%u'",
                    coord, id);
                PSendSysMessage("|cff00ff00Waypoint script:|r|cff00ffff %u|r|cff00ff00 position_x updated.|r", id);
                return true;
            }
            else if (arg_str_2 == "posy")
            {
                coord = atof(arg_3);
                WorldDatabase.PExecuteLog("UPDATE waypoint_scripts SET y='%f' WHERE guid='%u'",
                    coord, id);
                PSendSysMessage("|cff00ff00Waypoint script: %u position_y updated.|r", id);
                return true;
            }
            else if (arg_str_2 == "posz")
            {
                coord = atof(arg_3);
                WorldDatabase.PExecuteLog("UPDATE waypoint_scripts SET z='%f' WHERE guid='%u'",
                    coord, id);
                PSendSysMessage("|cff00ff00Waypoint script: |r|cff00ffff%u|r|cff00ff00 position_z updated.|r", id);
                return true;
            }
            else if (arg_str_2 == "orientation")
            {
                coord = atof(arg_3);
                WorldDatabase.PExecuteLog("UPDATE waypoint_scripts SET o='%f' WHERE guid='%u'",
                    coord, id);
                PSendSysMessage("|cff00ff00Waypoint script: |r|cff00ffff%u|r|cff00ff00 orientation updated.|r", id);
                return true;
            }
            else if (arg_str_2 == "dataint")
            {
                    WorldDatabase.PExecuteLog("UPDATE waypoint_scripts SET %s='%u' WHERE guid='%u'",
                    arg_2, atoi(arg_3), id);
                    PSendSysMessage("|cff00ff00Waypoint script: |r|cff00ffff%u|r|cff00ff00 dataint updated.|r", id);
                    return true;
            }
            else
            {
                    std::string arg_str_3 = arg_3;
                    WorldDatabase.escape_string(arg_str_3);
                    WorldDatabase.PExecuteLog("UPDATE waypoint_scripts SET %s='%s' WHERE guid='%u'",
                    arg_2, arg_str_3.c_str(), id);
            }
        }
        PSendSysMessage("%s%s|r|cff00ffff%u:|r|cff00ff00 %s %s|r","|cff00ff00","Waypoint script:", id, arg_2,"updated.");
    }
    return true;
}

bool ChatHandler::HandleWpUnLoadPathCommand(const char * /*args*/)
{
    uint32 guidlow = 0;
    Creature* target = getSelectedCreature();

    if (!target)
    {
        PSendSysMessage("%s%s|r", "|cff33ffff", "You must select target.");
        return true;
    }

    if (target->GetCreatureAddon())
    {
        if (target->GetCreatureAddon()->path_id != 0)
        {
            WorldDatabase.PExecuteLog("DELETE FROM creature_addon WHERE guid = %u", target->GetGUIDLow());
            target->UpdateWaypointID(0);
            WorldDatabase.PExecuteLog("UPDATE creature SET MovementType = '%u' WHERE guid = '%u'", IDLE_MOTION_TYPE, guidlow);
            target->LoadPath(0);
            target->SetDefaultMovementType(IDLE_MOTION_TYPE);
            target->GetMotionMaster()->MoveTargetedHome();
            target->GetMotionMaster()->Initialize();
            target->Say("Path unloaded.",0,0);
            return true;
        }
        PSendSysMessage("%s%s|r", "|cffff33ff", "Target has no loaded path.");
    }
    return true;
}

bool ChatHandler::HandleReloadAllPaths(const char* args)
{
    if (!*args)
        return false;

    uint32 id = atoi(args);

    if (!id)
        return false;

    PSendSysMessage("%s%s|r|cff00ffff%u|r", "|cff00ff00", "Loading Path: ", id);
    sWaypointMgr->UpdatePath(id);
    return true;
}

bool ChatHandler::HandleLoadPDumpCommand(const char *args)
{
    if (!*args)
        return false;

    char * file = strtok((char*)args, " ");
    if (!file)
        return false;

    char * account = strtok(NULL, " ");
    if (!account)
        return false;

    std::string account_name = account;
    if (!AccountMgr::normalizeString(account_name))
    {
        PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,account_name.c_str());
        SetSentErrorMessage(true);
        return false;
    }

    uint32 account_id = sAccountMgr->GetId(account_name);
    if (!account_id)
    {
        account_id = atoi(account);                             // use original string
        if (!account_id)
        {
            PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,account_name.c_str());
            SetSentErrorMessage(true);
            return false;
        }
    }

    if (!sAccountMgr->GetName(account_id,account_name))
    {
        PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,account_name.c_str());
        SetSentErrorMessage(true);
        return false;
    }

    char* guid_str = NULL;
    char* name_str = strtok(NULL, " ");

    std::string name;
    if (name_str)
    {
        name = name_str;
        // normalize the name if specified and check if it exists
        if (!normalizePlayerName(name))
        {
            PSendSysMessage(LANG_INVALID_CHARACTER_NAME);
            SetSentErrorMessage(true);
            return false;
        }

        if (!ObjectMgr::IsValidName(name,true))
        {
            PSendSysMessage(LANG_INVALID_CHARACTER_NAME);
            SetSentErrorMessage(true);
            return false;
        }

        guid_str = strtok(NULL, " ");
    }

    uint32 guid = 0;

    if (guid_str)
    {
        guid = atoi(guid_str);
        if (!guid)
        {
            PSendSysMessage(LANG_INVALID_CHARACTER_GUID);
            SetSentErrorMessage(true);
            return false;
        }

        if (objmgr.GetPlayerAccountIdByGUID(guid))
        {
            PSendSysMessage(LANG_CHARACTER_GUID_IN_USE,guid);
            SetSentErrorMessage(true);
            return false;
        }
    }

    switch (PlayerDumpReader().LoadDump(file, account_id, name, guid))
    {
        case DUMP_SUCCESS:
            PSendSysMessage(LANG_COMMAND_IMPORT_SUCCESS);
            break;
        case DUMP_FILE_OPEN_ERROR:
            PSendSysMessage(LANG_FILE_OPEN_FAIL,file);
            SetSentErrorMessage(true);
            return false;
        case DUMP_FILE_BROKEN:
            PSendSysMessage(LANG_DUMP_BROKEN,file);
            SetSentErrorMessage(true);
            return false;
        case DUMP_TOO_MANY_CHARS:
            PSendSysMessage(LANG_ACCOUNT_CHARACTER_LIST_FULL,account_name.c_str(),account_id);
            SetSentErrorMessage(true);
            return false;
        default:
            PSendSysMessage(LANG_COMMAND_IMPORT_FAILED);
            SetSentErrorMessage(true);
            return false;
    }

    return true;
}

bool ChatHandler::HandleWritePDumpCommand(const char *args)
{
    if (!*args)
        return false;

    char* file = strtok((char*)args, " ");
    char* p2 = strtok(NULL, " ");

    if (!file || !p2)
        return false;

    uint32 guid;
    // character name can't start from number
    if (isNumeric(p2))
        guid = atoi(p2);
    else
    {
        std::string name = p2;

        if (!normalizePlayerName (name))
        {
            SendSysMessage(LANG_PLAYER_NOT_FOUND);
            SetSentErrorMessage(true);
            return false;
        }

        guid = objmgr.GetPlayerGUIDByName(name);
    }

    if (!objmgr.GetPlayerAccountIdByGUID(guid))
    {
        PSendSysMessage(LANG_PLAYER_NOT_FOUND);
        SetSentErrorMessage(true);
        return false;
    }

    switch (PlayerDumpWriter().WriteDump(file, guid))
    {
        case DUMP_SUCCESS:
            PSendSysMessage(LANG_COMMAND_EXPORT_SUCCESS);
            break;
        case DUMP_FILE_OPEN_ERROR:
            PSendSysMessage(LANG_FILE_OPEN_FAIL,file);
            SetSentErrorMessage(true);
            return false;
        default:
            PSendSysMessage(LANG_COMMAND_EXPORT_FAILED);
            SetSentErrorMessage(true);
            return false;
    }

    return true;
}

bool ChatHandler::HandleServerShutDownCancelCommand(const char* /*args*/)
{
    sWorld.ShutdownCancel();
    return true;
}

bool ChatHandler::HandleServerIdleRestartCommand(const char *args)
{
    if (!*args)
        return false;

    char* time_str = strtok ((char*) args, " ");
    char* exitcode_str = strtok (NULL, "");

    int32 time = atoi (time_str);

    // Prevent interpret wrong arg value as 0 secs shutdown time
    if ((time == 0 && (time_str[0] != '0' || time_str[1] != '\0')) || time < 0)
        return false;

    if (exitcode_str)
    {
        int32 exitcode = atoi (exitcode_str);

        // Handle atoi() errors
        if (exitcode == 0 && (exitcode_str[0] != '0' || exitcode_str[1] != '\0'))
            return false;

        // Exit code should be in range of 0-125, 126-255 is used
        // in many shells for their own return codes and code > 255
        // is not supported in many others
        if (exitcode < 0 || exitcode > 125)
            return false;

        sWorld.ShutdownServ (time, SHUTDOWN_MASK_RESTART|SHUTDOWN_MASK_IDLE, exitcode);
    }
    else
        sWorld.ShutdownServ(time,SHUTDOWN_MASK_RESTART|SHUTDOWN_MASK_IDLE,RESTART_EXIT_CODE);
    return true;
}


bool ChatHandler::HandleServerIdleShutDownCommand(const char *args)
{
    if (!*args)
        return false;

    char* time_str = strtok ((char*) args, " ");
    char* exitcode_str = strtok (NULL, "");

    int32 time = atoi (time_str);

    // Prevent interpret wrong arg value as 0 secs shutdown time
    if ((time == 0 && (time_str[0] != '0' || time_str[1] != '\0')) || time < 0)
        return false;

    if (exitcode_str)
    {
        int32 exitcode = atoi (exitcode_str);

        // Handle atoi() errors
        if (exitcode == 0 && (exitcode_str[0] != '0' || exitcode_str[1] != '\0'))
            return false;

        // Exit code should be in range of 0-125, 126-255 is used
        // in many shells for their own return codes and code > 255
        // is not supported in many others
        if (exitcode < 0 || exitcode > 125)
            return false;

        sWorld.ShutdownServ (time, SHUTDOWN_MASK_IDLE, exitcode);
    }
    else
        sWorld.ShutdownServ(time,SHUTDOWN_MASK_IDLE,SHUTDOWN_EXIT_CODE);
    return true;
}

bool ChatHandler::HandleServerRestartCommand(const char *args)
{
    if (!*args)
        return false;

    char* time_str = strtok ((char*) args, " ");
    char* exitcode_str = strtok (NULL, "");

    int32 time = atoi (time_str);

    // Prevent interpret wrong arg value as 0 secs shutdown time
    if ((time == 0 && (time_str[0] != '0' || time_str[1] != '\0')) || time < 0)
        return false;

    if (exitcode_str)
    {
        int32 exitcode = atoi (exitcode_str);

        // Handle atoi() errors
        if (exitcode == 0 && (exitcode_str[0] != '0' || exitcode_str[1] != '\0'))
            return false;

        // Exit code should be in range of 0-125, 126-255 is used
        // in many shells for their own return codes and code > 255
        // is not supported in many others
        if (exitcode < 0 || exitcode > 125)
            return false;

        sWorld.ShutdownServ (time, SHUTDOWN_MASK_RESTART, exitcode);
    }
    else
        sWorld.ShutdownServ(time, SHUTDOWN_MASK_RESTART, RESTART_EXIT_CODE);
    return true;
}

bool ChatHandler::HandleServerShutDownCommand(const char *args)
{
    if (!*args)
        return false;

    char* time_str = strtok ((char*) args, " ");
    char* exitcode_str = strtok (NULL, "");

    int32 time = atoi (time_str);

    // Prevent interpret wrong arg value as 0 secs shutdown time
    if ((time == 0 && (time_str[0] != '0' || time_str[1] != '\0')) || time < 0)
        return false;

    if (exitcode_str)
    {
        int32 exitcode = atoi (exitcode_str);

        // Handle atoi() errors
        if (exitcode == 0 && (exitcode_str[0] != '0' || exitcode_str[1] != '\0'))
            return false;

        // Exit code should be in range of 0-125, 126-255 is used
        // in many shells for their own return codes and code > 255
        // is not supported in many others
        if (exitcode < 0 || exitcode > 125)
            return false;

        sWorld.ShutdownServ (time, 0, exitcode);
    }
    else
        sWorld.ShutdownServ(time,0,SHUTDOWN_EXIT_CODE);
    return true;
}

bool ChatHandler::HandleServerPLimitCommand(const char *args)
{
    if (*args)
    {
        char* param = strtok((char*)args, " ");
        if (!param)
            return false;

        int l = strlen(param);

        if (strncmp(param,"player",l) == 0)
            sWorld.SetPlayerSecurityLimit(SEC_PLAYER);
        else if (strncmp(param,"moderator",l) == 0)
            sWorld.SetPlayerSecurityLimit(SEC_MODERATOR);
        else if (strncmp(param,"gamemaster",l) == 0)
            sWorld.SetPlayerSecurityLimit(SEC_GAMEMASTER);
        else if (strncmp(param,"administrator",l) == 0)
            sWorld.SetPlayerSecurityLimit(SEC_ADMINISTRATOR);
        else if (strncmp(param,"reset",l) == 0)
            sWorld.SetPlayerLimit(sConfig.GetIntDefault("PlayerLimit", DEFAULT_PLAYER_LIMIT));
        else
        {
            int val = atoi(param);
            if (val < 0)
                sWorld.SetPlayerSecurityLimit(AccountTypes(uint32(-val)));
            else
                sWorld.SetPlayerLimit(val);
        }

        // kick all low security level players
        if (sWorld.GetPlayerSecurityLimit() > SEC_PLAYER)
            sWorld.KickAllLess(sWorld.GetPlayerSecurityLimit());
    }

    uint32 pLimit = sWorld.GetPlayerAmountLimit();
    AccountTypes allowedAccountType = sWorld.GetPlayerSecurityLimit();
    char const* secName = "";
    switch (allowedAccountType)
    {
        case SEC_PLAYER:        secName = "Player";        break;
        case SEC_MODERATOR:     secName = "Moderator";     break;
        case SEC_GAMEMASTER:    secName = "Gamemaster";    break;
        case SEC_ADMINISTRATOR: secName = "Administrator"; break;
        default:                secName = "<unknown>";     break;
    }

    PSendSysMessage("Player limits: amount %u, min. security level %s.",pLimit,secName);

    return true;
}

bool ChatHandler::HandleServerSetMotdCommand(const char *args)
{
    sWorld.SetMotd(args);
    PSendSysMessage(LANG_MOTD_NEW, args);
    return true;
}