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

bool ChatHandler::HandleAHBotOptionsCommand(const char *args)
{
    uint32 ahMapID = 0;
    char * opt = strtok((char*)args, " ");
    char * ahMapIdStr = strtok(NULL, " ");
    if (ahMapIdStr)
    {
        ahMapID = (uint32) strtoul(ahMapIdStr, NULL, 0);
    }
    if (!opt)
    {
        PSendSysMessage("Syntax is: ahbotoptions $option $ahMapID (2, 6 or 7) $parameter");
        PSendSysMessage("Try ahbotoptions help to see a list of options.");
        return false;
    }
    int l = strlen(opt);

    if (strncmp(opt,"help",l) == 0)
    {
        PSendSysMessage("AHBot commands:");
        PSendSysMessage("ahexpire");
        PSendSysMessage("minitems");
        PSendSysMessage("maxitems");
        //PSendSysMessage("");
        //PSendSysMessage("");
        PSendSysMessage("percentages");
        PSendSysMessage("minprice");
        PSendSysMessage("maxprice");
        PSendSysMessage("minbidprice");
        PSendSysMessage("maxbidprice");
        PSendSysMessage("maxstack");
        PSendSysMessage("buyerprice");
        PSendSysMessage("bidinterval");
        PSendSysMessage("bidsperinterval");
        return true;
    }
    else if (strncmp(opt,"ahexpire",l) == 0)
    {
        if (!ahMapIdStr)
        {
            PSendSysMessage("Syntax is: ahbotoptions ahexpire $ahMapID (2, 6 or 7)");
            return false;
        }
        auctionbot.Commands(0, ahMapID, NULL, NULL);
    }
    else if (strncmp(opt,"minitems",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            PSendSysMessage("Syntax is: ahbotoptions minitems $ahMapID (2, 6 or 7) $minItems");
            return false;
        }
        auctionbot.Commands(1, ahMapID, NULL, param1);
    }
    else if (strncmp(opt,"maxitems",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            PSendSysMessage("Syntax is: ahbotoptions maxitems $ahMapID (2, 6 or 7) $maxItems");
            return false;
        }
        auctionbot.Commands(2, ahMapID, NULL, param1);
    }
    else if (strncmp(opt,"mintime",l) == 0)
    {
        PSendSysMessage("ahbotoptions mintime has been deprecated");
        return false;
        /*
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            PSendSysMessage("Syntax is: ahbotoptions mintime $ahMapID (2, 6 or 7) $mintime");
            return false;
        }
        auctionbot.Commands(3, ahMapID, NULL, param1);
        */
    }
    else if (strncmp(opt,"maxtime",l) == 0)
    {
        PSendSysMessage("ahbotoptions maxtime has been deprecated");
        return false;
        /*
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            PSendSysMessage("Syntax is: ahbotoptions maxtime $ahMapID (2, 6 or 7) $maxtime");
            return false;
        }
        auctionbot.Commands(4, ahMapID, NULL, param1);
        */
    }
    else if (strncmp(opt,"percentages",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        char * param3 = strtok(NULL, " ");
        char * param4 = strtok(NULL, " ");
        char * param5 = strtok(NULL, " ");
        char * param6 = strtok(NULL, " ");
        char * param7 = strtok(NULL, " ");
        char * param8 = strtok(NULL, " ");
        char * param9 = strtok(NULL, " ");
        char * param10 = strtok(NULL, " ");
        char * param11 = strtok(NULL, " ");
        char * param12 = strtok(NULL, " ");
        char * param13 = strtok(NULL, " ");
        char * param14 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param14))
        {
            PSendSysMessage("Syntax is: ahbotoptions percentages $ahMapID (2, 6 or 7) $1 $2 $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14");
            PSendSysMessage("1 GreyTradeGoods 2 WhiteTradeGoods 3 GreenTradeGoods 4 BlueTradeGoods 5 PurpleTradeGoods");
            PSendSysMessage("6 OrangeTradeGoods 7 YellowTradeGoods 8 GreyItems 9 WhiteItems 10 GreenItems 11 BlueItems");
            PSendSysMessage("12 PurpleItems 13 OrangeItems 14 YellowItems");
            PSendSysMessage("The total must add up to 100%");
            return false;
        }
        uint32 greytg = (uint32) strtoul(param1, NULL, 0);
        uint32 whitetg = (uint32) strtoul(param2, NULL, 0);
        uint32 greentg = (uint32) strtoul(param3, NULL, 0);
        uint32 bluetg = (uint32) strtoul(param3, NULL, 0);
        uint32 purpletg = (uint32) strtoul(param5, NULL, 0);
        uint32 orangetg = (uint32) strtoul(param6, NULL, 0);
        uint32 yellowtg = (uint32) strtoul(param7, NULL, 0);
        uint32 greyi = (uint32) strtoul(param8, NULL, 0);
        uint32 whitei = (uint32) strtoul(param9, NULL, 0);
        uint32 greeni = (uint32) strtoul(param10, NULL, 0);
        uint32 bluei = (uint32) strtoul(param11, NULL, 0);
        uint32 purplei = (uint32) strtoul(param12, NULL, 0);
        uint32 orangei = (uint32) strtoul(param13, NULL, 0);
        uint32 yellowi = (uint32) strtoul(param14, NULL, 0);
        uint32 totalPercent = greytg + whitetg + greentg + bluetg + purpletg + orangetg + yellowtg + greyi + whitei + greeni + bluei + purplei + orangei + yellowi;
        if ((totalPercent == 0) || (totalPercent != 100))
        {
            PSendSysMessage("Syntax is: ahbotoptions percentages $ahMapID (2, 6 or 7) $1 $2 $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14");
            PSendSysMessage("1 GreyTradeGoods 2 WhiteTradeGoods 3 GreenTradeGoods 4 BlueTradeGoods 5 PurpleTradeGoods");
            PSendSysMessage("6 OrangeTradeGoods 7 YellowTradeGoods 8 GreyItems 9 WhiteItems 10 GreenItems 11 BlueItems");
            PSendSysMessage("12 PurpleItems 13 OrangeItems 14 YellowItems");
            PSendSysMessage("The total must add up to 100%");
            return false;
        }
        char param[100];
        param[0] = '\0';
        strcat(param, param1);
        strcat(param, " ");
        strcat(param, param2);
        strcat(param, " ");
        strcat(param, param3);
        strcat(param, " ");
        strcat(param, param4);
        strcat(param, " ");
        strcat(param, param5);
        strcat(param, " ");
        strcat(param, param6);
        strcat(param, " ");
        strcat(param, param7);
        strcat(param, " ");
        strcat(param, param8);
        strcat(param, " ");
        strcat(param, param9);
        strcat(param, " ");
        strcat(param, param10);
        strcat(param, " ");
        strcat(param, param11);
        strcat(param, " ");
        strcat(param, param12);
        strcat(param, " ");
        strcat(param, param13);
        strcat(param, " ");
        strcat(param, param14);
        auctionbot.Commands(5, ahMapID, NULL, param);
    }
    else if (strncmp(opt,"minprice",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            PSendSysMessage("Syntax is: ahbotoptions minprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(6, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            PSendSysMessage("Syntax is: ahbotoptions minprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
    }
    else if (strncmp(opt,"maxprice",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            PSendSysMessage("Syntax is: ahbotoptions maxprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(7, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            PSendSysMessage("Syntax is: ahbotoptions maxprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
    }
    else if (strncmp(opt,"minbidprice",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            PSendSysMessage("Syntax is: ahbotoptions minbidprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
        uint32 minBidPrice = (uint32) strtoul(param2, NULL, 0);
        if ((minBidPrice < 1) || (minBidPrice > 100))
        {
            PSendSysMessage("The min bid price multiplier must be between 1 and 100");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(8, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            PSendSysMessage("Syntax is: ahbotoptions minbidprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
    }
    else if (strncmp(opt,"maxbidprice",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            PSendSysMessage("Syntax is: ahbotoptions maxbidprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
        uint32 maxBidPrice = (uint32) strtoul(param2, NULL, 0);
        if ((maxBidPrice < 1) || (maxBidPrice > 100))
        {
            PSendSysMessage("The max bid price multiplier must be between 1 and 100");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(9, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            PSendSysMessage("Syntax is: ahbotoptions max bidprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $price");
            return false;
        }
    }
    else if (strncmp(opt,"maxstack",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            PSendSysMessage("Syntax is: ahbotoptions maxstack $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $value");
            return false;
        }
        uint32 maxStack = (uint32) strtoul(param2, NULL, 0);
        if (maxStack < 0)
        {
            PSendSysMessage("maxstack can't be a negative number.");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(10, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            PSendSysMessage("Syntax is: ahbotoptions maxstack $ahMapID (2, 6 or 7) $color (grey, white, green, blue, purple, orange or yellow) $value");
            return false;
        }
    }
    else if (strncmp(opt,"buyerprice",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        char * param2 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1) || (!param2))
        {
            PSendSysMessage("Syntax is: ahbotoptions buyerprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue or purple) $price");
            return false;
        }
        if (strncmp(param1,"grey",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_GREY, param2);
        }
        else if (strncmp(param1,"white",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_WHITE, param2);
        }
        else if (strncmp(param1,"green",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_GREEN, param2);
        }
        else if (strncmp(param1,"blue",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_BLUE, param2);
        }
        else if (strncmp(param1,"purple",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_PURPLE, param2);
        }
        else if (strncmp(param1,"orange",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_ORANGE, param2);
        }
        else if (strncmp(param1,"yellow",l) == 0)
        {
            auctionbot.Commands(11, ahMapID, AHB_YELLOW, param2);
        }
        else
        {
            PSendSysMessage("Syntax is: ahbotoptions buyerprice $ahMapID (2, 6 or 7) $color (grey, white, green, blue or purple) $price");
            return false;
        }
    }
    else if (strncmp(opt,"bidinterval",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            PSendSysMessage("Syntax is: ahbotoptions bidinterval $ahMapID (2, 6 or 7) $interval(in minutes)");
            return false;
        }
        auctionbot.Commands(12, ahMapID, NULL, param1);
    }
    else if (strncmp(opt,"bidsperinterval",l) == 0)
    {
        char * param1 = strtok(NULL, " ");
        if ((!ahMapIdStr) || (!param1))
        {
            PSendSysMessage("Syntax is: ahbotoptions bidsperinterval $ahMapID (2, 6 or 7) $bids");
            return false;
        }
        auctionbot.Commands(13, ahMapID, NULL, param1);
    }
    else
    {
        PSendSysMessage("Syntax is: ahbotoptions $option $ahMapID (2, 6 or 7) $parameter");
        PSendSysMessage("Try ahbotoptions help to see a list of options.");
        return false;
    }
    return true;
}

bool ChatHandler::HandleAllowMovementCommand(const char* /*args*/)
{
    if (sWorld.getAllowMovement())
    {
        sWorld.SetAllowMovement(false);
        SendSysMessage(LANG_CREATURE_MOVE_DISABLED);
    }
    else
    {
        sWorld.SetAllowMovement(true);
        SendSysMessage(LANG_CREATURE_MOVE_ENABLED);
    }
    return true;
}

bool ChatHandler::HandleMaxSkillCommand(const char* /*args*/)
{
    Player* SelectedPlayer = getSelectedPlayer();
    if (!SelectedPlayer)
    {
        SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
    }

    // each skills that have max skill value dependent from level seted to current level max skill value
    SelectedPlayer->UpdateSkillsToMaxSkillsForLevel();
    return true;
}

bool ChatHandler::HandleSetSkillCommand(const char *args)
{
    // number or [name] Shift-click form |color|Hskill:skill_id|h[name]|h|r
    char* skill_p = extractKeyFromLink((char*)args,"Hskill");
    if (!skill_p)
        return false;

    char *level_p = strtok (NULL, " ");

    if (!level_p)
        return false;

    char *max_p   = strtok (NULL, " ");

    int32 skill = atoi(skill_p);
    if (skill <= 0)
    {
        PSendSysMessage(LANG_INVALID_SKILL_ID, skill);
        SetSentErrorMessage(true);
        return false;
    }

    int32 level = atol (level_p);

    Player* target = getSelectedPlayer();
    if (!target)
    {
        SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
    }

    SkillLineEntry const* sl = sSkillLineStore.LookupEntry(skill);
    if (!sl)
    {
        PSendSysMessage(LANG_INVALID_SKILL_ID, skill);
        SetSentErrorMessage(true);
        return false;
    }

    if (!target->GetSkillValue(skill))
    {
        PSendSysMessage(LANG_SET_SKILL_ERROR, target->GetName(), skill, sl->name[m_session->GetSessionDbcLocale()]);
        SetSentErrorMessage(true);
        return false;
    }

    int32 max   = max_p ? atol (max_p) : target->GetPureMaxSkillValue(skill);

    if (level <= 0 || level > max || max <= 0)
        return false;

    target->SetSkill(skill, level, max);
    PSendSysMessage(LANG_SET_SKILL, skill, sl->name[m_session->GetSessionDbcLocale()], target->GetName(), level, max);

    return true;
}

bool ChatHandler::HandleCooldownCommand(const char *args)
{
    Player* target = getSelectedPlayer();
    if (!target)
    {
        SendSysMessage(LANG_PLAYER_NOT_FOUND);
        SetSentErrorMessage(true);
        return false;
    }

    if (!*args)
    {
        target->RemoveAllSpellCooldown();
        PSendSysMessage(LANG_REMOVEALL_COOLDOWN, target->GetName());
    }
    else
    {
        // number or [name] Shift-click form |color|Hspell:spell_id|h[name]|h|r or Htalent form
        uint32 spell_id = extractSpellIdFromLink((char*)args);
        if (!spell_id)
            return false;

        if (!sSpellStore.LookupEntry(spell_id))
        {
            PSendSysMessage(LANG_UNKNOWN_SPELL, target == m_session->GetPlayer() ? GetBlizzLikeString(LANG_YOU) : target->GetName());
            SetSentErrorMessage(true);
            return false;
        }

        target->RemoveSpellCooldown(spell_id,true);
        PSendSysMessage(LANG_REMOVE_COOLDOWN, spell_id, target == m_session->GetPlayer() ? GetBlizzLikeString(LANG_YOU) : target->GetName());
    }
    return true;
}

static void learnAllHighRanks(Player* player, uint32 spellid)
{
    SpellChainNode const* node;
    do
    {
        node = spellmgr.GetSpellChainNode(spellid);
        player->learnSpell(spellid);
        if (!node)
            break;
        spellid=node->next;
    }
    while (node->next);
}

bool ChatHandler::HandleLearnAllMyTalentsCommand(const char* /*args*/)
{
    Player* player = m_session->GetPlayer();
    uint32 classMask = player->getClassMask();

    for (uint32 i = 0; i < sTalentStore.GetNumRows(); i++)
    {
        TalentEntry const *talentInfo = sTalentStore.LookupEntry(i);
        if (!talentInfo)
            continue;

        TalentTabEntry const *talentTabInfo = sTalentTabStore.LookupEntry(talentInfo->TalentTab);
        if (!talentTabInfo)
            continue;

        if ((classMask & talentTabInfo->ClassMask) == 0)
            continue;

        // search highest talent rank
        uint32 spellid = 0;

        for (int rank = MAX_TALENT_RANK - 1; rank >= 0; --rank)
        {
            if (talentInfo->RankID[rank] != 0)
            {
                spellid = talentInfo->RankID[rank];
                break;
            }
        }

        if (!spellid)                                        // ??? none spells in talent
            continue;

        SpellEntry const* spellInfo = sSpellStore.LookupEntry(spellid);
        if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo,m_session->GetPlayer(),false))
            continue;

        // learn highest rank of talent
        player->learnSpell(spellid);

        // and learn all non-talent spell ranks (recursive by tree)
        learnAllHighRanks(player,spellid);
    }

    SendSysMessage(LANG_COMMAND_LEARN_CLASS_TALENTS);
    return true;
}

bool ChatHandler::HandleLearnAllLangCommand(const char* /*args*/)
{
    // skipping UNIVERSAL language (0)
    for (uint8 i = 1; i < LANGUAGES_COUNT; ++i)
        m_session->GetPlayer()->learnSpell(lang_description[i].spell_id);

    SendSysMessage(LANG_COMMAND_LEARN_ALL_LANG);
    return true;
}

bool ChatHandler::HandleLearnAllDefaultCommand(const char *args)
{
    char* pName = strtok((char*)args, "");
    Player* player = NULL;
    if (pName)
    {
        std::string name = pName;

        if (!normalizePlayerName(name))
        {
            SendSysMessage(LANG_PLAYER_NOT_FOUND);
            SetSentErrorMessage(true);
            return false;
        }

        player = ObjectAccessor::Instance().FindPlayerByName(name.c_str());
    }
    else
        player = getSelectedPlayer();

    if (!player)
    {
        SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
    }

    player->learnDefaultSpells();
    player->learnQuestRewardedSpells();

    PSendSysMessage(LANG_COMMAND_LEARN_ALL_DEFAULT_AND_QUEST,player->GetName());
    return true;
}

bool ChatHandler::HandleListItemCommand(const char *args)
{
    if (!*args)
        return false;

    char* cId = extractKeyFromLink((char*)args,"Hitem");
    if (!cId)
        return false;

    uint32 item_id = atol(cId);
    if (!item_id)
    {
        PSendSysMessage(LANG_COMMAND_ITEMIDINVALID, item_id);
        SetSentErrorMessage(true);
        return false;
    }

    ItemPrototype const* itemProto = objmgr.GetItemPrototype(item_id);
    if (!itemProto)
    {
        PSendSysMessage(LANG_COMMAND_ITEMIDINVALID, item_id);
        SetSentErrorMessage(true);
        return false;
    }

    char* c_count = strtok(NULL, " ");
    int count = c_count ? atol(c_count) : 10;

    if (count < 0)
        return false;

    // inventory case
    uint32 inv_count = 0;
    QueryResult_AutoPtr result = CharacterDatabase.PQuery("SELECT COUNT(item_template) FROM character_inventory WHERE item_template='%u'",item_id);
    if (result)
        inv_count = (*result)[0].GetUInt32();

    result=CharacterDatabase.PQuery(
    //          0        1             2             3        4                  5
        "SELECT ci.item, cibag.slot AS bag, ci.slot, ci.guid, characters.account,characters.name "
        "FROM character_inventory AS ci LEFT JOIN character_inventory AS cibag ON (cibag.item=ci.bag),characters "
        "WHERE ci.item_template='%u' AND ci.guid = characters.guid LIMIT %u ",
        item_id,uint32(count));

    if (result)
    {
        do
        {
            Field *fields = result->Fetch();
            uint32 item_guid = fields[0].GetUInt32();
            uint32 item_bag = fields[1].GetUInt32();
            uint32 item_slot = fields[2].GetUInt32();
            uint32 owner_guid = fields[3].GetUInt32();
            uint32 owner_acc = fields[4].GetUInt32();
            std::string owner_name = fields[5].GetCppString();

            char const* item_pos = 0;
            if (Player::IsEquipmentPos(item_bag,item_slot))
                item_pos = "[equipped]";
            else if (Player::IsInventoryPos(item_bag,item_slot))
                item_pos = "[in inventory]";
            else if (Player::IsBankPos(item_bag,item_slot))
                item_pos = "[in bank]";
            else
                item_pos = "";

            PSendSysMessage(LANG_ITEMLIST_SLOT,
                item_guid,owner_name.c_str(),owner_guid,owner_acc,item_pos);
        } while (result->NextRow());

        int64 res_count = result->GetRowCount();

        if (count > res_count)
            count-=res_count;
        else if (count)
            count = 0;
    }

    // mail case
    uint32 mail_count = 0;
    result=CharacterDatabase.PQuery("SELECT COUNT(item_template) FROM mail_items WHERE item_template='%u'", item_id);
    if (result)
        mail_count = (*result)[0].GetUInt32();

    if (count > 0)
    {
        result=CharacterDatabase.PQuery(
        //          0                     1            2              3               4            5               6
            "SELECT mail_items.item_guid, mail.sender, mail.receiver, char_s.account, char_s.name, char_r.account, char_r.name "
            "FROM mail,mail_items,characters as char_s,characters as char_r "
            "WHERE mail_items.item_template='%u' AND char_s.guid = mail.sender AND char_r.guid = mail.receiver AND mail.id=mail_items.mail_id LIMIT %u",
            item_id,uint32(count));
    }
    else
        result = QueryResult_AutoPtr(NULL);

    if (result)
    {
        do
        {
            Field *fields = result->Fetch();
            uint32 item_guid        = fields[0].GetUInt32();
            uint32 item_s           = fields[1].GetUInt32();
            uint32 item_r           = fields[2].GetUInt32();
            uint32 item_s_acc       = fields[3].GetUInt32();
            std::string item_s_name = fields[4].GetCppString();
            uint32 item_r_acc       = fields[5].GetUInt32();
            std::string item_r_name = fields[6].GetCppString();

            char const* item_pos = "[in mail]";

            PSendSysMessage(LANG_ITEMLIST_MAIL,
                item_guid,item_s_name.c_str(),item_s,item_s_acc,item_r_name.c_str(),item_r,item_r_acc,item_pos);
        } while (result->NextRow());

        int64 res_count = result->GetRowCount();

        if (count > res_count)
            count-=res_count;
        else if (count)
            count = 0;
    }

    // auction case
    uint32 auc_count = 0;
    result=CharacterDatabase.PQuery("SELECT COUNT(item_template) FROM auctionhouse WHERE item_template='%u'",item_id);
    if (result)
        auc_count = (*result)[0].GetUInt32();

    if (count > 0)
    {
        result=CharacterDatabase.PQuery(
        //           0                      1                       2                   3
            "SELECT  auctionhouse.itemguid, auctionhouse.itemowner, characters.account, characters.name "
            "FROM auctionhouse,characters WHERE auctionhouse.item_template='%u' AND characters.guid = auctionhouse.itemowner LIMIT %u",
            item_id,uint32(count));
    }
    else
        result = QueryResult_AutoPtr(NULL);

    if (result)
    {
        do
        {
            Field *fields = result->Fetch();
            uint32 item_guid       = fields[0].GetUInt32();
            uint32 owner           = fields[1].GetUInt32();
            uint32 owner_acc       = fields[2].GetUInt32();
            std::string owner_name = fields[3].GetCppString();

            char const* item_pos = "[in auction]";

            PSendSysMessage(LANG_ITEMLIST_AUCTION, item_guid, owner_name.c_str(), owner, owner_acc,item_pos);
        } while (result->NextRow());
    }

    // guild bank case
    uint32 guild_count = 0;
    result=CharacterDatabase.PQuery("SELECT COUNT(item_entry) FROM guild_bank_item WHERE item_entry='%u'",item_id);
    if (result)
        guild_count = (*result)[0].GetUInt32();

    result=CharacterDatabase.PQuery(
        //      0             1           2
        "SELECT gi.item_guid, gi.guildid, guild.name "
        "FROM guild_bank_item AS gi, guild WHERE gi.item_entry='%u' AND gi.guildid = guild.guildid LIMIT %u ",
        item_id,uint32(count));

    if (result)
    {
        do
        {
            Field *fields = result->Fetch();
            uint32 item_guid = fields[0].GetUInt32();
            uint32 guild_guid = fields[1].GetUInt32();
            std::string guild_name = fields[2].GetCppString();

            char const* item_pos = "[in guild bank]";

            PSendSysMessage(LANG_ITEMLIST_GUILD,item_guid,guild_name.c_str(),guild_guid,item_pos);
        } while (result->NextRow());

        int64 res_count = result->GetRowCount();

        if (count > res_count)
            count-=res_count;
        else if (count)
            count = 0;
    }

    if (inv_count+mail_count+auc_count+guild_count == 0)
    {
        SendSysMessage(LANG_COMMAND_NOITEMFOUND);
        SetSentErrorMessage(true);
        return false;
    }

    PSendSysMessage(LANG_COMMAND_LISTITEMMESSAGE,item_id,inv_count+mail_count+auc_count+guild_count,inv_count,mail_count,auc_count,guild_count);

    return true;
}

bool ChatHandler::HandleListObjectCommand(const char *args)
{
    if (!*args)
        return false;

    // number or [name] Shift-click form |color|Hgameobject_entry:go_id|h[name]|h|r
    char* cId = extractKeyFromLink((char*)args,"Hgameobject_entry");
    if (!cId)
        return false;

    uint32 go_id = atol(cId);
    if (!go_id)
    {
        PSendSysMessage(LANG_COMMAND_LISTOBJINVALIDID, go_id);
        SetSentErrorMessage(true);
        return false;
    }

    GameObjectInfo const * gInfo = objmgr.GetGameObjectInfo(go_id);
    if (!gInfo)
    {
        PSendSysMessage(LANG_COMMAND_LISTOBJINVALIDID, go_id);
        SetSentErrorMessage(true);
        return false;
    }

    char* c_count = strtok(NULL, " ");
    int count = c_count ? atol(c_count) : 10;

    if (count < 0)
        return false;

    uint32 obj_count = 0;
    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT COUNT(guid) FROM gameobject WHERE id='%u'",go_id);
    if (result)
        obj_count = (*result)[0].GetUInt32();

    if (m_session)
    {
        Player* pl = m_session->GetPlayer();
        result = WorldDatabase.PQuery("SELECT guid, position_x, position_y, position_z, map, id, (POW(position_x - '%f', 2) + POW(position_y - '%f', 2) + POW(position_z - '%f', 2)) AS order_ FROM gameobject WHERE id = '%u' ORDER BY order_ ASC LIMIT %u",
            pl->GetPositionX(), pl->GetPositionY(), pl->GetPositionZ(),go_id,uint32(count));
    }
    else
        result = WorldDatabase.PQuery("SELECT guid, position_x, position_y, position_z, map, id FROM gameobject WHERE id = '%u' LIMIT %u",
            go_id,uint32(count));

    if (result)
    {
        do
        {
            Field *fields = result->Fetch();
            uint32 guid = fields[0].GetUInt32();
            float x = fields[1].GetFloat();
            float y = fields[2].GetFloat();
            float z = fields[3].GetFloat();
            int mapid = fields[4].GetUInt16();
            uint32 entry = fields[5].GetUInt32();

            if (m_session)
                PSendSysMessage(LANG_GO_LIST_CHAT, guid, entry, guid, gInfo->name, x, y, z, mapid);
            else
                PSendSysMessage(LANG_GO_LIST_CONSOLE, guid, gInfo->name, x, y, z, mapid);
        } while (result->NextRow());
    }

    PSendSysMessage(LANG_COMMAND_LISTOBJMESSAGE,go_id,obj_count);
    return true;
}

bool ChatHandler::HandleNearObjectCommand(const char *args)
{
    float distance = (!*args) ? 10 : atol(args);
    uint32 count = 0;

    Player* pl = m_session->GetPlayer();
    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT guid, id, position_x, position_y, position_z, map, "
        "(POW(position_x - '%f', 2) + POW(position_y - '%f', 2) + POW(position_z - '%f', 2)) AS order_ "
        "FROM gameobject WHERE map='%u' AND (POW(position_x - '%f', 2) + POW(position_y - '%f', 2) + POW(position_z - '%f', 2)) <= '%f' ORDER BY order_",
        pl->GetPositionX(), pl->GetPositionY(), pl->GetPositionZ(),
        pl->GetMapId(),pl->GetPositionX(), pl->GetPositionY(), pl->GetPositionZ(),distance*distance);

    if (result)
    {
        do
        {
            Field *fields = result->Fetch();
            uint32 guid = fields[0].GetUInt32();
            uint32 entry = fields[1].GetUInt32();
            float x = fields[2].GetFloat();
            float y = fields[3].GetFloat();
            float z = fields[4].GetFloat();
            int mapid = fields[5].GetUInt16();

            GameObjectInfo const * gInfo = objmgr.GetGameObjectInfo(entry);

            if (!gInfo)
                continue;

            PSendSysMessage(LANG_GO_LIST_CHAT, guid, entry, guid, gInfo->name, x, y, z, mapid);

            ++count;
        } while (result->NextRow());
    }

    PSendSysMessage(LANG_COMMAND_NEAROBJMESSAGE,distance,count);
    return true;
}

bool ChatHandler::HandleObjectStateCommand(const char *args)
{
    // number or [name] Shift-click form |color|Hgameobject:go_id|h[name]|h|r
    char* cId = extractKeyFromLink((char*)args, "Hgameobject");
    if (!cId)
        return false;

    uint32 lowguid = atoi(cId);
    if (!lowguid)
        return false;

    GameObject* gobj = NULL;

    if (GameObjectData const* goData = objmgr.GetGOData(lowguid))
        gobj = GetObjectGlobalyWithGuidOrNearWithDbGuid(lowguid, goData->id);

    if (!gobj)
    {
        PSendSysMessage(LANG_COMMAND_OBJNOTFOUND, lowguid);
        SetSentErrorMessage(true);
        return false;
    }

    char* cstate = strtok(NULL, " ");
    if (!cstate)
        return false;

    int32 state = atoi(cstate);
    if (state < 0)
        gobj->SendObjectDeSpawnAnim(gobj->GetGUID());
    else
        gobj->SetGoState((GOState)state);

    return true;

    return true;
}

bool ChatHandler::HandleListCreatureCommand(const char *args)
{
    if (!*args)
        return false;

    // number or [name] Shift-click form |color|Hcreature_entry:creature_id|h[name]|h|r
    char* cId = extractKeyFromLink((char*)args,"Hcreature_entry");
    if (!cId)
        return false;

    uint32 cr_id = atol(cId);
    if (!cr_id)
    {
        PSendSysMessage(LANG_COMMAND_INVALIDCREATUREID, cr_id);
        SetSentErrorMessage(true);
        return false;
    }

    CreatureInfo const* cInfo = objmgr.GetCreatureTemplate(cr_id);
    if (!cInfo)
    {
        PSendSysMessage(LANG_COMMAND_INVALIDCREATUREID, cr_id);
        SetSentErrorMessage(true);
        return false;
    }

    char* c_count = strtok(NULL, " ");
    int count = c_count ? atol(c_count) : 10;

    if (count < 0)
        return false;

    uint32 cr_count = 0;
    QueryResult_AutoPtr result = WorldDatabase.PQuery("SELECT COUNT(guid) FROM creature WHERE id='%u'",cr_id);
    if (result)
        cr_count = (*result)[0].GetUInt32();

    if (m_session)
    {
        Player* pl = m_session->GetPlayer();
        result = WorldDatabase.PQuery("SELECT guid, position_x, position_y, position_z, map, (POW(position_x - '%f', 2) + POW(position_y - '%f', 2) + POW(position_z - '%f', 2)) AS order_ FROM creature WHERE id = '%u' ORDER BY order_ ASC LIMIT %u",
            pl->GetPositionX(), pl->GetPositionY(), pl->GetPositionZ(), cr_id,uint32(count));
    }
    else
        result = WorldDatabase.PQuery("SELECT guid, position_x, position_y, position_z, map FROM creature WHERE id = '%u' LIMIT %u",
            cr_id,uint32(count));

    if (result)
    {
        do
        {
            Field *fields = result->Fetch();
            uint32 guid = fields[0].GetUInt32();
            float x = fields[1].GetFloat();
            float y = fields[2].GetFloat();
            float z = fields[3].GetFloat();
            int mapid = fields[4].GetUInt16();

            if  (m_session)
                PSendSysMessage(LANG_CREATURE_LIST_CHAT, guid, guid, cInfo->Name, x, y, z, mapid);
            else
                PSendSysMessage(LANG_CREATURE_LIST_CONSOLE, guid, cInfo->Name, x, y, z, mapid);
        } while (result->NextRow());
    }

    PSendSysMessage(LANG_COMMAND_LISTCREATUREMESSAGE,cr_id,cr_count);
    return true;
}

bool ChatHandler::HandleLookupItemCommand(const char *args)
{
    if (!*args)
        return false;

    std::string namepart = args;
    std::wstring wnamepart;

    // converting string that we try to find to lower case
    if (!Utf8toWStr(namepart,wnamepart))
        return false;

    wstrToLower(wnamepart);

    bool found = false;

    // Search in item_template
    for (uint32 id = 0; id < sItemStorage.MaxEntry; id++)
    {
        ItemPrototype const *pProto = sItemStorage.LookupEntry<ItemPrototype >(id);
        if (!pProto)
            continue;

        int loc_idx = m_session ? m_session->GetSessionDbLocaleIndex() : objmgr.GetDBCLocaleIndex();
        if (loc_idx >= 0)
        {
            ItemLocale const *il = objmgr.GetItemLocale(pProto->ItemId);
            if (il)
            {
                if (il->Name.size() > loc_idx && !il->Name[loc_idx].empty())
                {
                    std::string name = il->Name[loc_idx];

                    if (Utf8FitTo(name, wnamepart))
                    {
                        if (m_session)
                            PSendSysMessage(LANG_ITEM_LIST_CHAT, id, id, name.c_str());
                        else
                            PSendSysMessage(LANG_ITEM_LIST_CONSOLE, id, name.c_str());

                        if (!found)
                            found = true;

                        continue;
                    }
                }
            }
        }

        std::string name = pProto->Name1;
        if (name.empty())
            continue;

        if (Utf8FitTo(name, wnamepart))
        {
            if (m_session)
                PSendSysMessage(LANG_ITEM_LIST_CHAT, id, id, name.c_str());
            else
                PSendSysMessage(LANG_ITEM_LIST_CONSOLE, id, name.c_str());

            if (!found)
                found = true;
        }
    }

    if (!found)
        SendSysMessage(LANG_COMMAND_NOITEMFOUND);

    return true;
}

bool ChatHandler::HandleLookupItemSetCommand(const char *args)
{
    if (!*args)
        return false;

    std::string namepart = args;
    std::wstring wnamepart;

    if (!Utf8toWStr(namepart,wnamepart))
        return false;

    // converting string that we try to find to lower case
    wstrToLower(wnamepart);

    bool found = false;

    // Search in ItemSet.dbc
    for (uint32 id = 0; id < sItemSetStore.GetNumRows(); id++)
    {
        ItemSetEntry const *set = sItemSetStore.LookupEntry(id);
        if (set)
        {
            int loc = m_session ? m_session->GetSessionDbcLocale() : sWorld.GetDefaultDbcLocale();
            std::string name = set->name[loc];
            if (name.empty())
                continue;

            if (!Utf8FitTo(name, wnamepart))
            {
                loc = 0;
                for (; loc < MAX_LOCALE; ++loc)
                {
                    if (m_session && loc == m_session->GetSessionDbcLocale())
                        continue;

                    name = set->name[loc];
                    if (name.empty())
                        continue;

                    if (Utf8FitTo(name, wnamepart))
                        break;
                }
            }

            if (loc < MAX_LOCALE)
            {
                // send item set in "id - [namedlink locale]" format
                if (m_session)
                    PSendSysMessage(LANG_ITEMSET_LIST_CHAT,id,id,name.c_str(),localeNames[loc]);
                else
                    PSendSysMessage(LANG_ITEMSET_LIST_CONSOLE,id,name.c_str(),localeNames[loc]);

                if (!found)
                    found = true;
            }
        }
    }
    if (!found)
        SendSysMessage(LANG_COMMAND_NOITEMSETFOUND);
    return true;
}

bool ChatHandler::HandleLookupSkillCommand(const char *args)
{
    if (!*args)
        return false;

    // can be NULL in console call
    Player* target = getSelectedPlayer();

    std::string namepart = args;
    std::wstring wnamepart;

    if (!Utf8toWStr(namepart,wnamepart))
        return false;

    // converting string that we try to find to lower case
    wstrToLower(wnamepart);

    bool found = false;

    // Search in SkillLine.dbc
    for (uint32 id = 0; id < sSkillLineStore.GetNumRows(); id++)
    {
        SkillLineEntry const *skillInfo = sSkillLineStore.LookupEntry(id);
        if (skillInfo)
        {
            int loc = m_session ? m_session->GetSessionDbcLocale() : sWorld.GetDefaultDbcLocale();
            std::string name = skillInfo->name[loc];
            if (name.empty())
                continue;

            if (!Utf8FitTo(name, wnamepart))
            {
                loc = 0;
                for (; loc < MAX_LOCALE; ++loc)
                {
                    if (m_session && loc == m_session->GetSessionDbcLocale())
                        continue;

                    name = skillInfo->name[loc];
                    if (name.empty())
                        continue;

                    if (Utf8FitTo(name, wnamepart))
                        break;
                }
            }

            if (loc < MAX_LOCALE)
            {
                char const* knownStr = "";
                if (target && target->HasSkill(id))
                    knownStr = GetBlizzLikeString(LANG_KNOWN);

                // send skill in "id - [namedlink locale]" format
                if (m_session)
                    PSendSysMessage(LANG_SKILL_LIST_CHAT,id,id,name.c_str(),localeNames[loc],knownStr);
                else
                    PSendSysMessage(LANG_SKILL_LIST_CONSOLE,id,name.c_str(),localeNames[loc],knownStr);

                if (!found)
                    found = true;
            }
        }
    }
    if (!found)
        SendSysMessage(LANG_COMMAND_NOSKILLFOUND);
    return true;
}

bool ChatHandler::HandleLookupSpellCommand(const char *args)
{
    if (!*args)
        return false;

    // can be NULL at console call
    Player* target = getSelectedPlayer();

    std::string namepart = args;
    std::wstring wnamepart;

    if (!Utf8toWStr(namepart,wnamepart))
        return false;

    // converting string that we try to find to lower case
    wstrToLower(wnamepart);

    bool found = false;

    // Search in Spell.dbc
    for (uint32 id = 0; id < sSpellStore.GetNumRows(); id++)
    {
        SpellEntry const *spellInfo = sSpellStore.LookupEntry(id);
        if (spellInfo)
        {
            int loc = m_session ? m_session->GetSessionDbcLocale() : sWorld.GetDefaultDbcLocale();
            std::string name = spellInfo->SpellName[loc];
            if (name.empty())
                continue;

            if (!Utf8FitTo(name, wnamepart))
            {
                loc = 0;
                for (; loc < MAX_LOCALE; ++loc)
                {
                    if (m_session && loc == m_session->GetSessionDbcLocale())
                        continue;

                    name = spellInfo->SpellName[loc];
                    if (name.empty())
                        continue;

                    if (Utf8FitTo(name, wnamepart))
                        break;
                }
            }

            if (loc < MAX_LOCALE)
            {
                bool known = target && target->HasSpell(id);
                bool learn = (spellInfo->Effect[0] == SPELL_EFFECT_LEARN_SPELL);

                uint32 talentCost = GetTalentSpellCost(id);

                bool talent = (talentCost > 0);
                bool passive = IsPassiveSpell(id);
                bool active = target && (target->HasAura(id,0) || target->HasAura(id,1) || target->HasAura(id,2));

                // unit32 used to prevent interpreting uint8 as char at output
                // find rank of learned spell for learning spell, or talent rank
                uint32 rank = talentCost ? talentCost : spellmgr.GetSpellRank(learn ? spellInfo->EffectTriggerSpell[0] : id);

                // send spell in "id - [name, rank N] [talent] [passive] [learn] [known]" format
                std::ostringstream ss;
                if (m_session)
                    ss << id << " - |cffffffff|Hspell:" << id << "|h[" << name;
                else
                    ss << id << " - " << name;

                // include rank in link name
                if (rank)
                    ss << GetBlizzLikeString(LANG_SPELL_RANK) << rank;

                if (m_session)
                    ss << " " << localeNames[loc] << "]|h|r";
                else
                    ss << " " << localeNames[loc];

                if (talent)
                    ss << GetBlizzLikeString(LANG_TALENT);
                if (passive)
                    ss << GetBlizzLikeString(LANG_PASSIVE);
                if (learn)
                    ss << GetBlizzLikeString(LANG_LEARN);
                if (known)
                    ss << GetBlizzLikeString(LANG_KNOWN);
                if (active)
                    ss << GetBlizzLikeString(LANG_ACTIVE);

                SendSysMessage(ss.str().c_str());

                if (!found)
                    found = true;
            }
        }
    }
    if (!found)
        SendSysMessage(LANG_COMMAND_NOSPELLFOUND);
    return true;
}

bool ChatHandler::HandleLookupQuestCommand(const char *args)
{
    if (!*args)
        return false;

    // can be NULL at console call
    Player* target = getSelectedPlayer();

    std::string namepart = args;
    std::wstring wnamepart;

    // converting string that we try to find to lower case
    if (!Utf8toWStr(namepart,wnamepart))
        return false;

    wstrToLower(wnamepart);

    bool found = false;

    ObjectMgr::QuestMap const& qTemplates = objmgr.GetQuestTemplates();
    for (ObjectMgr::QuestMap::const_iterator iter = qTemplates.begin(); iter != qTemplates.end(); ++iter)
    {
        Quest * qinfo = iter->second;

        int loc_idx = m_session ? m_session->GetSessionDbLocaleIndex() : objmgr.GetDBCLocaleIndex();
        if (loc_idx >= 0)
        {
            QuestLocale const *il = objmgr.GetQuestLocale(qinfo->GetQuestId());
            if (il)
            {
                if (il->Title.size() > loc_idx && !il->Title[loc_idx].empty())
                {
                    std::string title = il->Title[loc_idx];

                    if (Utf8FitTo(title, wnamepart))
                    {
                        char const* statusStr = "";

                        if (target)
                        {
                            QuestStatus status = target->GetQuestStatus(qinfo->GetQuestId());

                            if (status == QUEST_STATUS_COMPLETE)
                            {
                                if (target->GetQuestRewardStatus(qinfo->GetQuestId()))
                                    statusStr = GetBlizzLikeString(LANG_COMMAND_QUEST_REWARDED);
                                else
                                    statusStr = GetBlizzLikeString(LANG_COMMAND_QUEST_COMPLETE);
                            }
                            else if (status == QUEST_STATUS_INCOMPLETE)
                                statusStr = GetBlizzLikeString(LANG_COMMAND_QUEST_ACTIVE);
                        }

                        if (m_session)
                            PSendSysMessage(LANG_QUEST_LIST_CHAT,qinfo->GetQuestId(),qinfo->GetQuestId(),title.c_str(),statusStr);
                        else
                            PSendSysMessage(LANG_QUEST_LIST_CONSOLE,qinfo->GetQuestId(),title.c_str(),statusStr);

                        if (!found)
                            found = true;

                        continue;
                    }
                }
            }
        }

        std::string title = qinfo->GetTitle();
        if (title.empty())
            continue;

        if (Utf8FitTo(title, wnamepart))
        {
            char const* statusStr = "";

            if (target)
            {
                QuestStatus status = target->GetQuestStatus(qinfo->GetQuestId());

                if (status == QUEST_STATUS_COMPLETE)
                {
                    if (target->GetQuestRewardStatus(qinfo->GetQuestId()))
                        statusStr = GetBlizzLikeString(LANG_COMMAND_QUEST_REWARDED);
                    else
                        statusStr = GetBlizzLikeString(LANG_COMMAND_QUEST_COMPLETE);
                }
                else if (status == QUEST_STATUS_INCOMPLETE)
                    statusStr = GetBlizzLikeString(LANG_COMMAND_QUEST_ACTIVE);
            }

            if (m_session)
                PSendSysMessage(LANG_QUEST_LIST_CHAT,qinfo->GetQuestId(),qinfo->GetQuestId(),title.c_str(),statusStr);
            else
                PSendSysMessage(LANG_QUEST_LIST_CONSOLE,qinfo->GetQuestId(),title.c_str(),statusStr);

            if (!found)
                found = true;
        }
    }

    if (!found)
        SendSysMessage(LANG_COMMAND_NOQUESTFOUND);

    return true;
}

bool ChatHandler::HandleLookupCreatureCommand(const char *args)
{
    if (!*args)
        return false;

    std::string namepart = args;
    std::wstring wnamepart;

    // converting string that we try to find to lower case
    if (!Utf8toWStr (namepart,wnamepart))
        return false;

    wstrToLower (wnamepart);

    bool found = false;

    for (uint32 id = 0; id< sCreatureStorage.MaxEntry; ++id)
    {
        CreatureInfo const* cInfo = sCreatureStorage.LookupEntry<CreatureInfo> (id);
        if (!cInfo)
            continue;

        int loc_idx = m_session ? m_session->GetSessionDbLocaleIndex() : objmgr.GetDBCLocaleIndex();
        if (loc_idx >= 0)
        {
            CreatureLocale const *cl = objmgr.GetCreatureLocale (id);
            if (cl)
            {
                if (cl->Name.size() > loc_idx && !cl->Name[loc_idx].empty ())
                {
                    std::string name = cl->Name[loc_idx];

                    if (Utf8FitTo (name, wnamepart))
                    {
                        if (m_session)
                            PSendSysMessage (LANG_CREATURE_ENTRY_LIST_CHAT, id, id, name.c_str ());
                        else
                            PSendSysMessage (LANG_CREATURE_ENTRY_LIST_CONSOLE, id, name.c_str ());

                        if (!found)
                            found = true;

                        continue;
                    }
                }
            }
        }

        std::string name = cInfo->Name;
        if (name.empty ())
            continue;

        if (Utf8FitTo(name, wnamepart))
        {
            if (m_session)
                PSendSysMessage (LANG_CREATURE_ENTRY_LIST_CHAT, id, id, name.c_str ());
            else
                PSendSysMessage (LANG_CREATURE_ENTRY_LIST_CONSOLE, id, name.c_str ());

            if (!found)
                found = true;
        }
    }

    if (!found)
        SendSysMessage (LANG_COMMAND_NOCREATUREFOUND);

    return true;
}

bool ChatHandler::HandleLookupObjectCommand(const char *args)
{
    if (!*args)
        return false;

    std::string namepart = args;
    std::wstring wnamepart;

    // converting string that we try to find to lower case
    if (!Utf8toWStr(namepart,wnamepart))
        return false;

    wstrToLower(wnamepart);

    bool found = false;

    for (uint32 id = 0; id< sGOStorage.MaxEntry; id++)
    {
        GameObjectInfo const* gInfo = sGOStorage.LookupEntry<GameObjectInfo>(id);
        if (!gInfo)
            continue;

        int loc_idx = m_session ? m_session->GetSessionDbLocaleIndex() : objmgr.GetDBCLocaleIndex();
        if (loc_idx >= 0)
        {
            GameObjectLocale const *gl = objmgr.GetGameObjectLocale(id);
            if (gl)
            {
                if (gl->Name.size() > loc_idx && !gl->Name[loc_idx].empty())
                {
                    std::string name = gl->Name[loc_idx];

                    if (Utf8FitTo(name, wnamepart))
                    {
                        if (m_session)
                            PSendSysMessage(LANG_GO_ENTRY_LIST_CHAT, id, id, name.c_str());
                        else
                            PSendSysMessage(LANG_GO_ENTRY_LIST_CONSOLE, id, name.c_str());

                        if (!found)
                            found = true;

                        continue;
                    }
                }
            }
        }

        std::string name = gInfo->name;
        if (name.empty())
            continue;

        if (Utf8FitTo(name, wnamepart))
        {
            if (m_session)
                PSendSysMessage(LANG_GO_ENTRY_LIST_CHAT, id, id, name.c_str());
            else
                PSendSysMessage(LANG_GO_ENTRY_LIST_CONSOLE, id, name.c_str());

            if (!found)
                found = true;
        }
    }

    if (!found)
        SendSysMessage(LANG_COMMAND_NOGAMEOBJECTFOUND);

    return true;
}

/** \brief GM command level 3 - Create a guild.
 *
 * This command allows a GM (level 3) to create a guild.
 *
 * The "args" parameter contains the name of the guild leader
 * and then the name of the guild.
 *
 */

bool ChatHandler::HandleAddWeaponCommand(const char* /*args*/)
{
    /*if (!*args)
        return false;

    uint64 guid = m_session->GetPlayer()->GetSelection();
    if (guid == 0)
    {
        SendSysMessage(LANG_NO_SELECTION);
        return true;
    }

    Creature* pCreature = ObjectAccessor::GetCreature(*m_session->GetPlayer(), guid);

    if (!pCreature)
    {
        SendSysMessage(LANG_SELECT_CREATURE);
        return true;
    }

    char* pSlotID = strtok((char*)args, " ");
    if (!pSlotID)
        return false;

    char* pItemID = strtok(NULL, " ");
    if (!pItemID)
        return false;

    uint32 ItemID = atoi(pItemID);
    uint32 SlotID = atoi(pSlotID);

    ItemPrototype* tmpItem = objmgr.GetItemPrototype(ItemID);

    bool added = false;
    if (tmpItem)
    {
        switch (SlotID)
        {
            case 1:
                pCreature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY, ItemID);
                added = true;
                break;
            case 2:
                pCreature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_01, ItemID);
                added = true;
                break;
            case 3:
                pCreature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_DISPLAY_02, ItemID);
                added = true;
                break;
            default:
                PSendSysMessage(LANG_ITEM_SLOT_NOT_EXIST,SlotID);
                added = false;
                break;
        }
        if (added)
        {
            PSendSysMessage(LANG_ITEM_ADDED_TO_SLOT,ItemID,tmpItem->Name1,SlotID);
        }
    }
    else
    {
        PSendSysMessage(LANG_ITEM_NOT_FOUND,ItemID);
        return true;
    }
    */
    return true;
}

bool ChatHandler::HandleDieCommand(const char* /*args*/)
{
    Unit* target = getSelectedUnit();

    if (!target || !m_session->GetPlayer()->GetSelection())
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    if (target->isAlive())
    {
        if (sWorld.getConfig(CONFIG_DIE_COMMAND_MODE)) 
            m_session->GetPlayer()->Kill(target); 
        else 
            m_session->GetPlayer()->DealDamage(target, target->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false); 
    }

    return true;
}

bool ChatHandler::HandleModifyArenaCommand(const char * args)
{
    if (!*args)
        return false;

    Player* target = getSelectedPlayer();
    if (!target)
    {
        SendSysMessage(LANG_PLAYER_NOT_FOUND);
        SetSentErrorMessage(true);
        return false;
    }

    int32 amount = (uint32)atoi(args);

    target->ModifyArenaPoints(amount);

    PSendSysMessage(LANG_COMMAND_MODIFY_ARENA, target->GetName(), target->GetArenaPoints());

    return true;
}

bool ChatHandler::HandleReviveCommand(const char *args)
{
    Player* player = NULL;
    uint32 player_guid = 0;

    if (*args)
    {
        std::string name = args;
        if (!normalizePlayerName(name))
        {
            SendSysMessage(LANG_PLAYER_NOT_FOUND);
            SetSentErrorMessage(true);
            return false;
        }

        player = ObjectAccessor::Instance().FindPlayerByName(name.c_str());
        if (!player)
            player_guid = objmgr.GetPlayerGUIDByName(name);
    }
    else
        player = getSelectedPlayer();

    if (player)
    {
        player->ResurrectPlayer(player->GetSession()->GetSecurity() > SEC_PLAYER ? 1.0f : 0.5f);
        player->SpawnCorpseBones();
        player->SaveToDB();
    }
    else if (player_guid)
    {
        // will resurrected at login without corpse
        ObjectAccessor::Instance().ConvertCorpseForPlayer(player_guid);
    }
    else
    {
        SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
    }

    return true;
}

bool ChatHandler::HandleLinkGraveCommand(const char *args)
{
    if (!*args)
        return false;

    char* px = strtok((char*)args, " ");
    if (!px)
        return false;

    uint32 g_id = (uint32)atoi(px);

    uint32 g_team;

    char* px2 = strtok(NULL, " ");

    if (!px2)
        g_team = 0;
    else if (strncmp(px2,"horde",6) == 0)
        g_team = HORDE;
    else if (strncmp(px2,"alliance",9) == 0)
        g_team = ALLIANCE;
    else
        return false;

    WorldSafeLocsEntry const* graveyard =  sWorldSafeLocsStore.LookupEntry(g_id);

    if (!graveyard)
    {
        PSendSysMessage(LANG_COMMAND_GRAVEYARDNOEXIST, g_id);
        SetSentErrorMessage(true);
        return false;
    }

    Player* player = m_session->GetPlayer();

    uint32 zoneId = player->GetZoneId();

    AreaTableEntry const *areaEntry = GetAreaEntryByAreaID(zoneId);
    if (!areaEntry || areaEntry->zone != 0)
    {
        PSendSysMessage(LANG_COMMAND_GRAVEYARDWRONGZONE, g_id,zoneId);
        SetSentErrorMessage(true);
        return false;
    }

    if (objmgr.AddGraveYardLink(g_id,player->GetZoneId(),g_team))
        PSendSysMessage(LANG_COMMAND_GRAVEYARDLINKED, g_id,zoneId);
    else
        PSendSysMessage(LANG_COMMAND_GRAVEYARDALRLINKED, g_id,zoneId);

    return true;
}

bool ChatHandler::HandleNearGraveCommand(const char *args)
{
    uint32 g_team;

    size_t argslen = strlen(args);

    if (!*args)
        g_team = 0;
    else if (strncmp((char*)args,"horde",argslen) == 0)
        g_team = HORDE;
    else if (strncmp((char*)args,"alliance",argslen) == 0)
        g_team = ALLIANCE;
    else
        return false;

    Player* player = m_session->GetPlayer();

    WorldSafeLocsEntry const* graveyard = objmgr.GetClosestGraveYard(
        player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(),player->GetMapId(),g_team);

    if (graveyard)
    {
        uint32 g_id = graveyard->ID;

        GraveYardData const* data = objmgr.FindGraveYardData(g_id,player->GetZoneId());
        if (!data)
        {
            PSendSysMessage(LANG_COMMAND_GRAVEYARDERROR,g_id);
            SetSentErrorMessage(true);
            return false;
        }

        g_team = data->team;

        std::string team_name = GetBlizzLikeString(LANG_COMMAND_GRAVEYARD_NOTEAM);

        if (g_team == 0)
            team_name = GetBlizzLikeString(LANG_COMMAND_GRAVEYARD_ANY);
        else if (g_team == HORDE)
            team_name = GetBlizzLikeString(LANG_COMMAND_GRAVEYARD_HORDE);
        else if (g_team == ALLIANCE)
            team_name = GetBlizzLikeString(LANG_COMMAND_GRAVEYARD_ALLIANCE);

        PSendSysMessage(LANG_COMMAND_GRAVEYARDNEAREST, g_id,team_name.c_str(),player->GetZoneId());
    }
    else
    {
        std::string team_name;

        if (g_team == 0)
            team_name = GetBlizzLikeString(LANG_COMMAND_GRAVEYARD_ANY);
        else if (g_team == HORDE)
            team_name = GetBlizzLikeString(LANG_COMMAND_GRAVEYARD_HORDE);
        else if (g_team == ALLIANCE)
            team_name = GetBlizzLikeString(LANG_COMMAND_GRAVEYARD_ALLIANCE);

        if (g_team == ~uint32(0))
            PSendSysMessage(LANG_COMMAND_ZONENOGRAVEYARDS, player->GetZoneId());
        else
            PSendSysMessage(LANG_COMMAND_ZONENOGRAFACTION, player->GetZoneId(),team_name.c_str());
    }

    return true;
}

//play npc emote
bool ChatHandler::HandleNpcPlayEmoteCommand(const char *args)
{
    uint32 emote = atoi((char*)args);

    Creature* target = getSelectedCreature();
    if (!target)
    {
        SendSysMessage(LANG_SELECT_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    target->SetUInt32Value(UNIT_NPC_EMOTESTATE,emote);

    return true;
}

bool ChatHandler::HandleNpcInfoCommand(const char* /*args*/)
{
    Creature* target = getSelectedCreature();

    if (!target)
    {
        SendSysMessage(LANG_SELECT_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    uint32 faction = target->getFaction();
    uint32 npcflags = target->GetUInt32Value(UNIT_NPC_FLAGS);
    uint32 displayid = target->GetDisplayId();
    uint32 nativeid = target->GetNativeDisplayId();
    uint32 Entry = target->GetEntry();
    CreatureInfo const* cInfo = target->GetCreatureInfo();

    int32 curRespawnDelay = target->GetRespawnTimeEx()-time(NULL);
    if (curRespawnDelay < 0)
        curRespawnDelay = 0;
    std::string curRespawnDelayStr = secsToTimeString(curRespawnDelay,true);
    std::string defRespawnDelayStr = secsToTimeString(target->GetRespawnDelay(),true);

    PSendSysMessage(LANG_NPCINFO_CHAR,  target->GetDBTableGUIDLow(), faction, npcflags, Entry, displayid, nativeid);
    PSendSysMessage(LANG_NPCINFO_LEVEL, target->getLevel());
    PSendSysMessage(LANG_NPCINFO_HEALTH,target->GetCreateHealth(), target->GetMaxHealth(), target->GetHealth());
    PSendSysMessage(LANG_NPCINFO_FLAGS, target->GetUInt32Value(UNIT_FIELD_FLAGS), target->GetUInt32Value(UNIT_DYNAMIC_FLAGS), target->getFaction());
    PSendSysMessage(LANG_COMMAND_RAWPAWNTIMES, defRespawnDelayStr.c_str(),curRespawnDelayStr.c_str());
    PSendSysMessage(LANG_NPCINFO_LOOT,  cInfo->lootid,cInfo->pickpocketLootId,cInfo->SkinLootId);
    PSendSysMessage(LANG_NPCINFO_DUNGEON_ID, target->GetInstanceId());
    PSendSysMessage(LANG_NPCINFO_POSITION,float(target->GetPositionX()), float(target->GetPositionY()), float(target->GetPositionZ()));
    if (const CreatureData* const linked = target->GetLinkedRespawnCreatureData())
        if (CreatureInfo const *master = GetCreatureInfo(linked->id))
            PSendSysMessage(LANG_NPCINFO_LINKGUID, objmgr.GetLinkedRespawnGuid(target->GetDBTableGUIDLow()), linked->id, master->Name);

    if ((npcflags & UNIT_NPC_FLAG_VENDOR))
    {
        SendSysMessage(LANG_NPCINFO_VENDOR);
    }
    if ((npcflags & UNIT_NPC_FLAG_TRAINER))
    {
        SendSysMessage(LANG_NPCINFO_TRAINER);
    }

    return true;
}

bool ChatHandler::HandleExploreCheatCommand(const char *args)
{
    if (!*args)
        return false;

    int flag = atoi((char*)args);

    Player* chr = getSelectedPlayer();
    if (chr == NULL)
    {
        SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
    }

    if (flag != 0)
    {
        PSendSysMessage(LANG_YOU_SET_EXPLORE_ALL, chr->GetName());
        if (needReportToTarget(chr))
            ChatHandler(chr).PSendSysMessage(LANG_YOURS_EXPLORE_SET_ALL,GetName());
    }
    else
    {
        PSendSysMessage(LANG_YOU_SET_EXPLORE_NOTHING, chr->GetName());
        if (needReportToTarget(chr))
            ChatHandler(chr).PSendSysMessage(LANG_YOURS_EXPLORE_SET_NOTHING,GetName());
    }

    for (uint8 i=0; i<128; ++i)
    {
        if (flag != 0)
        {
            m_session->GetPlayer()->SetFlag(PLAYER_EXPLORED_ZONES_1+i,0xFFFFFFFF);
        }
        else
        {
            m_session->GetPlayer()->SetFlag(PLAYER_EXPLORED_ZONES_1+i,0);
        }
    }

    return true;
}

bool ChatHandler::HandleHoverCommand(const char *args)
{
    char* px = strtok((char*)args, " ");
    uint32 flag;
    if (!px)
        flag = 1;
    else
        flag = atoi(px);

    m_session->GetPlayer()->SetHover(flag);

    if (flag)
        SendSysMessage(LANG_HOVER_ENABLED);
    else
        SendSysMessage(LANG_HOVER_DISABLED);

    return true;
}

bool ChatHandler::HandleWaterwalkCommand(const char *args)
{
    if (!*args)
        return false;

    Player* player = getSelectedPlayer();
    if (!player)
    {
        PSendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
    }

    if (strncmp(args, "on", 3) == 0)
        player->SetMovement(MOVE_WATER_WALK);               // ON
    else if (strncmp(args, "off", 4) == 0)
        player->SetMovement(MOVE_LAND_WALK);                // OFF
    else
    {
        SendSysMessage(LANG_USE_BOL);
        return false;
    }

    PSendSysMessage(LANG_YOU_SET_WATERWALK, args, player->GetName());
    if (needReportToTarget(player))
        ChatHandler(player).PSendSysMessage(LANG_YOUR_WATERWALK_SET, args, GetName());
    return true;

}

bool ChatHandler::HandleShowAreaCommand(const char *args)
{
    if (!*args)
        return false;

    int area = atoi((char*)args);

    Player* chr = getSelectedPlayer();
    if (chr == NULL)
    {
        SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
    }

    int offset = area / 32;
    uint32 val = (uint32)(1 << (area % 32));

    if (offset >= 128)
    {
        SendSysMessage(LANG_BAD_VALUE);
        SetSentErrorMessage(true);
        return false;
    }

    uint32 currFields = chr->GetUInt32Value(PLAYER_EXPLORED_ZONES_1 + offset);
    chr->SetUInt32Value(PLAYER_EXPLORED_ZONES_1 + offset, (uint32)(currFields | val));

    SendSysMessage(LANG_EXPLORE_AREA);
    return true;
}

bool ChatHandler::HandleHideAreaCommand(const char *args)
{
    if (!*args)
        return false;

    int area = atoi((char*)args);

    Player* chr = getSelectedPlayer();
    if (chr == NULL)
    {
        SendSysMessage(LANG_NO_CHAR_SELECTED);
        SetSentErrorMessage(true);
        return false;
    }

    int offset = area / 32;
    uint32 val = (uint32)(1 << (area % 32));

    if (offset >= 128)
    {
        SendSysMessage(LANG_BAD_VALUE);
        SetSentErrorMessage(true);
        return false;
    }

    uint32 currFields = chr->GetUInt32Value(PLAYER_EXPLORED_ZONES_1 + offset);
    chr->SetUInt32Value(PLAYER_EXPLORED_ZONES_1 + offset, (uint32)(currFields ^ val));

    SendSysMessage(LANG_UNEXPLORE_AREA);
    return true;
}

bool ChatHandler::HandleUpdate(const char *args)
{
    if (!*args)
        return false;

    uint32 updateIndex;
    uint32 value;

    char* pUpdateIndex = strtok((char*)args, " ");

    Unit* chr = getSelectedUnit();
    if (chr == NULL)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    if (!pUpdateIndex)
    {
        return true;
    }
    updateIndex = atoi(pUpdateIndex);
    //check updateIndex
    if (chr->GetTypeId() == TYPEID_PLAYER)
    {
        if (updateIndex>=PLAYER_END) return true;
    }
    else
    {
        if (updateIndex>=UNIT_END) return true;
    }

    char*  pvalue = strtok(NULL, " ");
    if (!pvalue)
    {
        value=chr->GetUInt32Value(updateIndex);

        PSendSysMessage(LANG_UPDATE, chr->GetGUIDLow(),updateIndex,value);
        return true;
    }

    value=atoi(pvalue);

    PSendSysMessage(LANG_UPDATE_CHANGE, chr->GetGUIDLow(),updateIndex,value);

    chr->SetUInt32Value(updateIndex,value);

    return true;
}

bool ChatHandler::HandleBankCommand(const char* /*args*/)
{
    m_session->SendShowBank(m_session->GetPlayer()->GetGUID());

    return true;
}

bool ChatHandler::HandleChangeWeather(const char *args)
{
    if (!*args)
        return false;

    //Weather is OFF
    if (!sWorld.getConfig(CONFIG_WEATHER))
    {
        SendSysMessage(LANG_WEATHER_DISABLED);
        SetSentErrorMessage(true);
        return false;
    }

    //*Change the weather of a cell
    char* px = strtok((char*)args, " ");
    char* py = strtok(NULL, " ");

    if (!px || !py)
        return false;

    uint32 type = (uint32)atoi(px);                         //0 to 3, 0: fine, 1: rain, 2: snow, 3: sand
    float grade = (float)atof(py);                          //0 to 1, sending -1 is instand good weather

    Player* player = m_session->GetPlayer();
    uint32 zoneid = player->GetZoneId();

    Weather* wth = sWorld.FindWeather(zoneid);

    if (!wth)
        wth = sWorld.AddWeather(zoneid);
    if (!wth)
    {
        SendSysMessage(LANG_NO_WEATHER);
        SetSentErrorMessage(true);
        return false;
    }

    wth->SetWeather(WeatherType(type), grade);

    return true;
}

bool ChatHandler::HandleSetValue(const char *args)
{
    if (!*args)
        return false;

    char* px = strtok((char*)args, " ");
    char* py = strtok(NULL, " ");
    char* pz = strtok(NULL, " ");

    if (!px || !py)
        return false;

    Unit* target = getSelectedUnit();
    if (!target)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    uint64 guid = target->GetGUID();

    uint32 Opcode = (uint32)atoi(px);
    if (Opcode >= target->GetValuesCount())
    {
        PSendSysMessage(LANG_TOO_BIG_INDEX, Opcode, GUID_LOPART(guid), target->GetValuesCount());
        return false;
    }
    uint32 iValue;
    float fValue;
    bool isint32 = true;
    if (pz)
        isint32 = (bool)atoi(pz);
    if (isint32)
    {
        iValue = (uint32)atoi(py);
     // sLog.outDebug(GetBlizzLikeString(LANG_SET_UINT), GUID_LOPART(guid), Opcode, iValue);
        target->SetUInt32Value(Opcode , iValue);
        PSendSysMessage(LANG_SET_UINT_FIELD, GUID_LOPART(guid), Opcode,iValue);
    }
    else
    {
        fValue = (float)atof(py);
     // sLog.outDebug(GetBlizzLikeString(LANG_SET_FLOAT), GUID_LOPART(guid), Opcode, fValue);
        target->SetFloatValue(Opcode , fValue);
        PSendSysMessage(LANG_SET_FLOAT_FIELD, GUID_LOPART(guid), Opcode,fValue);
    }

    return true;
}

bool ChatHandler::HandleGetValue(const char *args)
{
    if (!*args)
        return false;

    char* px = strtok((char*)args, " ");
    char* pz = strtok(NULL, " ");

    if (!px)
        return false;

    Unit* target = getSelectedUnit();
    if (!target)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    uint64 guid = target->GetGUID();

    uint32 Opcode = (uint32)atoi(px);
    if (Opcode >= target->GetValuesCount())
    {
        PSendSysMessage(LANG_TOO_BIG_INDEX, Opcode, GUID_LOPART(guid), target->GetValuesCount());
        return false;
    }
    uint32 iValue;
    float fValue;
    bool isint32 = true;
    if (pz)
        isint32 = (bool)atoi(pz);

    if (isint32)
    {
        iValue = target->GetUInt32Value(Opcode);
     // sLog.outDebug(GetBlizzLikeString(LANG_GET_UINT), GUID_LOPART(guid), Opcode, iValue);
        PSendSysMessage(LANG_GET_UINT_FIELD, GUID_LOPART(guid), Opcode,    iValue);
    }
    else
    {
        fValue = target->GetFloatValue(Opcode);
     // sLog.outDebug(GetBlizzLikeString(LANG_GET_FLOAT), GUID_LOPART(guid), Opcode, fValue);
        PSendSysMessage(LANG_GET_FLOAT_FIELD, GUID_LOPART(guid), Opcode, fValue);
    }

    return true;
}

bool ChatHandler::HandleSet32Bit(const char *args)
{
    if (!*args)
        return false;

    char* px = strtok((char*)args, " ");
    char* py = strtok(NULL, " ");

    if (!px || !py)
        return false;

    uint32 Opcode = (uint32)atoi(px);
    uint32 Value = (uint32)atoi(py);
    if (Value > 32)                                         //uint32 = 32 bits
        return false;

 // sLog.outDebug(GetBlizzLikeString(LANG_SET_32BIT), Opcode, Value);

    m_session->GetPlayer()->SetUInt32Value(Opcode , 2^Value);

    PSendSysMessage(LANG_SET_32BIT_FIELD, Opcode,1);
    return true;
}

bool ChatHandler::HandleAddTeleCommand(const char * args)
{
    if (!*args)
        return false;

    Player* player=m_session->GetPlayer();
    if (!player)
        return false;

    std::string name = args;

    if (objmgr.GetGameTele(name))
    {
        SendSysMessage(LANG_COMMAND_TP_ALREADYEXIST);
        SetSentErrorMessage(true);
        return false;
    }

    GameTele tele;
    tele.position_x  = player->GetPositionX();
    tele.position_y  = player->GetPositionY();
    tele.position_z  = player->GetPositionZ();
    tele.orientation = player->GetOrientation();
    tele.mapId       = player->GetMapId();
    tele.name        = name;

    if (objmgr.AddGameTele(tele))
    {
        SendSysMessage(LANG_COMMAND_TP_ADDED);
    }
    else
    {
        SendSysMessage(LANG_COMMAND_TP_ADDEDERR);
        SetSentErrorMessage(true);
        return false;
    }

    return true;
}

bool ChatHandler::HandleDelTeleCommand(const char * args)
{
    if (!*args)
        return false;

    std::string name = args;

    if (!objmgr.DeleteGameTele(name))
    {
        SendSysMessage(LANG_COMMAND_TELE_NOTFOUND);
        SetSentErrorMessage(true);
        return false;
    }

    SendSysMessage(LANG_COMMAND_TP_DELETED);
    return true;
}

bool ChatHandler::HandleListAurasCommand (const char * /*args*/)
{
    Unit* unit = getSelectedUnit();
    if (!unit)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    char const* talentStr = GetBlizzLikeString(LANG_TALENT);
    char const* passiveStr = GetBlizzLikeString(LANG_PASSIVE);

    Unit::AuraMap const& uAuras = unit->GetAuras();
    PSendSysMessage(LANG_COMMAND_TARGET_LISTAURAS, uAuras.size());
    for (Unit::AuraMap::const_iterator itr = uAuras.begin(); itr != uAuras.end(); ++itr)
    {
        bool talent = GetTalentSpellCost(itr->second->GetId()) > 0;
        PSendSysMessage(LANG_COMMAND_TARGET_AURADETAIL, itr->second->GetId(), itr->second->GetEffIndex(),
            itr->second->GetModifier()->m_auraname, itr->second->GetAuraDuration(), itr->second->GetAuraMaxDuration(),
            itr->second->GetSpellProto()->SpellName[m_session->GetSessionDbcLocale()],
            (itr->second->IsPassive() ? passiveStr : ""),(talent ? talentStr : ""),
            IS_PLAYER_GUID(itr->second->GetCasterGUID()) ? "player" : "creature",GUID_LOPART(itr->second->GetCasterGUID()));
    }
    for (uint16 i = 0; i < TOTAL_AURAS; ++i)
    {
        Unit::AuraList const& uAuraList = unit->GetAurasByType(AuraType(i));
        if (uAuraList.empty()) continue;
        PSendSysMessage(LANG_COMMAND_TARGET_LISTAURATYPE, uAuraList.size(), i);
        for (Unit::AuraList::const_iterator itr = uAuraList.begin(); itr != uAuraList.end(); ++itr)
        {
            bool talent = GetTalentSpellCost((*itr)->GetId()) > 0;
            PSendSysMessage(LANG_COMMAND_TARGET_AURASIMPLE, (*itr)->GetId(), (*itr)->GetEffIndex(),
                (*itr)->GetSpellProto()->SpellName[m_session->GetSessionDbcLocale()],((*itr)->IsPassive() ? passiveStr : ""),(talent ? talentStr : ""),
                IS_PLAYER_GUID((*itr)->GetCasterGUID()) ? "player" : "creature",GUID_LOPART((*itr)->GetCasterGUID()));
        }
    }
    return true;
}

bool ChatHandler::HandleBanAccountCommand(const char *args)
{
    return HandleBanHelper(BAN_ACCOUNT,args);
}

bool ChatHandler::HandleBanCharacterCommand(const char *args)
{
    return HandleBanHelper(BAN_CHARACTER,args);
}

bool ChatHandler::HandleBanIPCommand(const char *args)
{
    return HandleBanHelper(BAN_IP,args);
}

bool ChatHandler::HandleBanHelper(BanMode mode, const char *args)
{
    if (!*args)
        return false;

	std::string baner = "Console";
	if (m_session)
		baner = m_session->GetPlayer()->GetName();

    char* cnameOrIP = strtok ((char*)args, " ");
    if (!cnameOrIP)
        return false;

    std::string nameOrIP = cnameOrIP;

    char* duration = strtok (NULL," ");
    if (!duration || !atoi(duration))
        return false;

    char* reason = strtok (NULL,"");
    if (!reason)
        return false;

    switch (mode)
    {
        case BAN_ACCOUNT:
            if (!AccountMgr::normalizeString(nameOrIP))
            {
                PSendSysMessage(LANG_ACCOUNT_NOT_EXIST,nameOrIP.c_str());
                SetSentErrorMessage(true);
                return false;
            }
            break;
        case BAN_CHARACTER:
            if (!normalizePlayerName(nameOrIP))
            {
                SendSysMessage(LANG_PLAYER_NOT_FOUND);
                SetSentErrorMessage(true);
                return false;
            }
            break;
        case BAN_IP:
            if (!IsIPAddress(nameOrIP.c_str()))
                return false;
            break;
    }

    switch (sWorld.BanAccount(mode, nameOrIP, duration, reason,m_session ? m_session->GetPlayerName() : ""))
    {
        case BAN_SUCCESS:
            if (atoi(duration)>0)
			{
				if (sWorld.getConfig(CONFIG_SHOW_KICK_IN_WORLD) == 1)
				{
					if (mode == BAN_CHARACTER)
						sWorld.SendWorldText(LANG_COMMAND_BANCHAMESSAGE, nameOrIP.c_str(), baner.c_str(), secsToTimeString(TimeStringToSecs(duration),true).c_str(), reason);
					else if (mode == BAN_IP)
						sWorld.SendWorldText(LANG_COMMAND_BANIPMESSAGE, nameOrIP.c_str(), baner.c_str(), secsToTimeString(TimeStringToSecs(duration),true).c_str(), reason);
					else
						sWorld.SendWorldText(LANG_COMMAND_BANACCMESSAGE, nameOrIP.c_str(), baner.c_str(), secsToTimeString(TimeStringToSecs(duration),true).c_str(), reason);
				}
				else
				{
					PSendSysMessage(LANG_BAN_YOUBANNED,nameOrIP.c_str(),secsToTimeString(TimeStringToSecs(duration),true).c_str(),reason);
				}
			}
            else
			{
				if (sWorld.getConfig(CONFIG_SHOW_KICK_IN_WORLD) == 1)
				{
					if (mode == BAN_CHARACTER)
						sWorld.SendWorldText(LANG_COMMAND_BANCHAPERMAMESSAGE, nameOrIP.c_str(), baner.c_str(), reason);
					else if (mode == BAN_IP)
						sWorld.SendWorldText(LANG_COMMAND_BANIPPERMAMESSAGE, nameOrIP.c_str(), baner.c_str(), reason);
					else
						sWorld.SendWorldText(LANG_COMMAND_BANACCPERMAMESSAGE, nameOrIP.c_str(), baner.c_str(), reason);
				}
				else
				{
					PSendSysMessage(LANG_BAN_YOUPERMBANNED,nameOrIP.c_str(),reason);
				}
			}
            break;
        case BAN_SYNTAX_ERROR:
            return false;
        case BAN_NOTFOUND:
            switch (mode)
            {
                default:
                    PSendSysMessage(LANG_BAN_NOTFOUND,"account",nameOrIP.c_str());
                    break;
                case BAN_CHARACTER:
                    PSendSysMessage(LANG_BAN_NOTFOUND,"character",nameOrIP.c_str());
                    break;
                case BAN_IP:
                    PSendSysMessage(LANG_BAN_NOTFOUND,"ip",nameOrIP.c_str());
                    break;
            }
            SetSentErrorMessage(true);
            return false;
    }

    return true;
}

bool ChatHandler::HandleRespawnCommand(const char* /*args*/)
{
    Player* pl = m_session->GetPlayer();

    // accept only explicitly selected target (not implicitly self targeting case)
    Unit* target = getSelectedUnit();
    if (pl->GetSelection() && target)
    {
        if (target->GetTypeId() != TYPEID_UNIT)
        {
            SendSysMessage(LANG_SELECT_CREATURE);
            SetSentErrorMessage(true);
            return false;
        }

        if (target->isDead())
            target->ToCreature()->Respawn();
        return true;
    }

    CellPair p(BlizzLike::ComputeCellPair(pl->GetPositionX(), pl->GetPositionY()));
    Cell cell(p);
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();

    BlizzLike::RespawnDo u_do;
    BlizzLike::WorldObjectWorker<BlizzLike::RespawnDo> worker(u_do);

    TypeContainerVisitor<BlizzLike::WorldObjectWorker<BlizzLike::RespawnDo>, GridTypeMapContainer > obj_worker(worker);
    cell.Visit(p, obj_worker, *pl->GetMap());

    return true;
}

bool ChatHandler::HandleFlyModeCommand(const char *args)
{
    if (!*args)
        return false;

    Unit* unit = getSelectedUnit();
    if (!unit || (unit->GetTypeId() != TYPEID_PLAYER))
        unit = m_session->GetPlayer();

    WorldPacket data(12);
    if (strncmp(args, "on", 3) == 0)
        data.SetOpcode(SMSG_MOVE_SET_CAN_FLY);
    else if (strncmp(args, "off", 4) == 0)
        data.SetOpcode(SMSG_MOVE_UNSET_CAN_FLY);
    else
    {
        SendSysMessage(LANG_USE_BOL);
        return false;
    }
    data << unit->GetPackGUID();
    data << uint32(0);                                      // unknown
    unit->SendMessageToSet(&data, true);
    PSendSysMessage(LANG_COMMAND_FLYMODE_STATUS, unit->GetName(), args);
    return true;
}

bool ChatHandler::HandleNpcChangeEntryCommand(const char *args)
{
    if (!*args)
        return false;

    uint32 newEntryNum = atoi(args);
    if (!newEntryNum)
        return false;

    Unit* unit = getSelectedUnit();
    if (!unit || unit->GetTypeId() != TYPEID_UNIT)
    {
        SendSysMessage(LANG_SELECT_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }
    Creature* creature = unit->ToCreature();
    if (creature->UpdateEntry(newEntryNum))
        SendSysMessage(LANG_DONE);
    else
        SendSysMessage(LANG_ERROR);
    return true;
}

bool ChatHandler::HandleMovegensCommand(const char* /*args*/)
{
    Unit* unit = getSelectedUnit();
    if (!unit)
    {
        SendSysMessage(LANG_SELECT_CHAR_OR_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    PSendSysMessage(LANG_MOVEGENS_LIST,(unit->GetTypeId() == TYPEID_PLAYER ? "Player" : "Creature"),unit->GetGUIDLow());

    MotionMaster* mm = unit->GetMotionMaster();
    for (uint8 i = 0; i < MAX_MOTION_SLOT; ++i)
    {
        MovementGenerator* mg = mm->GetMotionSlot(i);
        if (!mg)
        {
            SendSysMessage("Empty");
            continue;
        }
        switch (mg->GetMovementGeneratorType())
        {
            case IDLE_MOTION_TYPE:          SendSysMessage(LANG_MOVEGENS_IDLE);          break;
            case RANDOM_MOTION_TYPE:        SendSysMessage(LANG_MOVEGENS_RANDOM);        break;
            case WAYPOINT_MOTION_TYPE:      SendSysMessage(LANG_MOVEGENS_WAYPOINT);      break;
            case ANIMAL_RANDOM_MOTION_TYPE: SendSysMessage(LANG_MOVEGENS_ANIMAL_RANDOM); break;
            case CONFUSED_MOTION_TYPE:      SendSysMessage(LANG_MOVEGENS_CONFUSED);      break;
            case TARGETED_MOTION_TYPE:
            {
                if (unit->GetTypeId() == TYPEID_PLAYER)
                {
                    TargetedMovementGenerator<Player> const* mgen = static_cast<TargetedMovementGenerator<Player> const*>(mg);
                    Unit* target = mgen->GetTarget();
                    if (target)
                        PSendSysMessage(LANG_MOVEGENS_TARGETED_PLAYER,target->GetName(),target->GetGUIDLow());
                    else
                        SendSysMessage(LANG_MOVEGENS_TARGETED_NULL);
                }
                else
                {
                    TargetedMovementGenerator<Creature> const* mgen = static_cast<TargetedMovementGenerator<Creature> const*>(mg);
                    Unit* target = mgen->GetTarget();
                    if (target)
                        PSendSysMessage(LANG_MOVEGENS_TARGETED_CREATURE,target->GetName(),target->GetGUIDLow());
                    else
                        SendSysMessage(LANG_MOVEGENS_TARGETED_NULL);
                }
                break;
            }
            case HOME_MOTION_TYPE:
                if (unit->GetTypeId() == TYPEID_UNIT)
                {
                    float x,y,z;
                    mg->GetDestination(x,y,z);
                    PSendSysMessage(LANG_MOVEGENS_HOME_CREATURE,x,y,z);
                }
                else
                    SendSysMessage(LANG_MOVEGENS_HOME_PLAYER);
                break;
            case FLIGHT_MOTION_TYPE:   SendSysMessage(LANG_MOVEGENS_FLIGHT);  break;
            case POINT_MOTION_TYPE:
            {
                float x,y,z;
                mg->GetDestination(x,y,z);
                PSendSysMessage(LANG_MOVEGENS_POINT,x,y,z);
                break;
            }
            case FLEEING_MOTION_TYPE:  SendSysMessage(LANG_MOVEGENS_FEAR);    break;
            case DISTRACT_MOTION_TYPE: SendSysMessage(LANG_MOVEGENS_DISTRACT);  break;
            default:
                PSendSysMessage(LANG_MOVEGENS_UNKNOWN,mg->GetMovementGeneratorType());
                break;
        }
    }
    return true;
}

/*
ComeToMe command REQUIRED for 3rd party scripting library to have access to PointMovementGenerator
Without this function 3rd party scripting library will get linking errors (unresolved external)
when attempting to use the PointMovementGenerator
*/
bool ChatHandler::HandleComeToMeCommand(const char *args)
{
    char* newFlagStr = strtok((char*)args, " ");

    if (!newFlagStr)
        return false;

    uint32 newFlags = (uint32)strtoul(newFlagStr, NULL, 0);

    Creature* caster = getSelectedCreature();
    if (!caster)
    {
        m_session->GetPlayer()->SetUnitMovementFlags(newFlags);
        SendSysMessage(LANG_SELECT_CREATURE);
        SetSentErrorMessage(true);
        return false;
    }

    caster->SetUnitMovementFlags(newFlags);

    Player* pl = m_session->GetPlayer();

    caster->GetMotionMaster()->MovePoint(0, pl->GetPositionX(), pl->GetPositionY(), pl->GetPositionZ());
    return true;
}

// Display the list of GMs
bool ChatHandler::HandleGMListFullCommand(const char* /*args*/)
{
    // Get the accounts with GM Level >0
    QueryResult_AutoPtr result = LoginDatabase.Query("SELECT a.username,aa.gmlevel FROM account a, account_access aa WHERE a.id=aa.id AND aa.gmlevel > 0");
    if (result)
    {
        SendSysMessage(LANG_GMLIST);
        SendSysMessage(" ======================== ");
        SendSysMessage(LANG_GMLIST_HEADER);
        SendSysMessage(" ======================== ");

        // Circle through them. Display username and GM level
        do
        {
            Field *fields = result->Fetch();
            PSendSysMessage("|%15s|%6s|", fields[0].GetString(),fields[1].GetString());
        }while (result->NextRow());

        PSendSysMessage(" ======================== ");
    }
    else
        PSendSysMessage(LANG_GMLIST_EMPTY);
    return true;
}

// Set/Unset the expansion level for an account
//Send items by mail
// Send a message to a player in game
bool ChatHandler::HandleSendMessageCommand(const char *args)
{
    // Get the command line arguments
    char* name_str = strtok((char*)args, " ");
    char* msg_str = strtok(NULL, "");

    if (!name_str || !msg_str)
        return false;

    std::string name = name_str;

    if (!normalizePlayerName(name))
        return false;

    // Find the player and check that he is not logging out.
    Player* rPlayer = ObjectAccessor::Instance().FindPlayerByName(name.c_str());
    if (!rPlayer)
    {
        SendSysMessage(LANG_PLAYER_NOT_FOUND);
        SetSentErrorMessage(true);
        return false;
    }

    if (rPlayer->GetSession()->isLogingOut())
    {
        SendSysMessage(LANG_PLAYER_NOT_FOUND);
        SetSentErrorMessage(true);
        return false;
    }

    // Send the message
    // Use SendAreaTriggerMessage for fastest delivery.
    rPlayer->GetSession()->SendAreaTriggerMessage("%s", msg_str);
    rPlayer->GetSession()->SendAreaTriggerMessage("|cffff0000[Message from administrator]:|r");

    //Confirmation message
    PSendSysMessage(LANG_SENDMESSAGE,name.c_str(),msg_str);
    return true;
}

/*------------------------------------------
 *-------------blizzlike----------------------
 *-------------------------------------*/

bool ChatHandler::HandlePlayAllCommand(const char *args)
{
    if (!*args)
        return false;

    uint32 soundId = atoi((char*)args);

    if (!sSoundEntriesStore.LookupEntry(soundId))
    {
        PSendSysMessage(LANG_SOUND_NOT_EXIST, soundId);
        SetSentErrorMessage(true);
        return false;
    }

    WorldPacket data(SMSG_PLAY_SOUND, 4);
    data << uint32(soundId) << m_session->GetPlayer()->GetGUID();
    sWorld.SendGlobalMessage(&data);

    PSendSysMessage(LANG_COMMAND_PLAYED_TO_ALL, soundId);
    return true;
}

bool ChatHandler::HandleFreezeCommand(const char *args)
{
    std::string name;
    Player* player;
    char *TargetName = strtok((char*)args, " "); //get entered name
    if (!TargetName) //if no name entered use target
    {
        player = getSelectedPlayer();
        if (player) //prevent crash with creature as target
        {
            name = player->GetName();
            normalizePlayerName(name);
        }
    }
    else // if name entered
    {
        name = TargetName;
        normalizePlayerName(name);
        player = ObjectAccessor::Instance().FindPlayerByName(name.c_str()); //get player by name
    }

    if (!player)
    {
        SendSysMessage(LANG_COMMAND_FREEZE_WRONG);
        return true;
    }

    if (player == m_session->GetPlayer())
    {
        SendSysMessage(LANG_COMMAND_FREEZE_ERROR);
        return true;
    }

    //effect
    if (player && player != m_session->GetPlayer())
    {
        PSendSysMessage(LANG_COMMAND_FREEZE,name.c_str());

        //stop combat + make player unattackable + duel stop + stop some spells
        player->setFaction(35);
        player->CombatStop();
        if (player->IsNonMeleeSpellCasted(true))
            player->InterruptNonMeleeSpells(true);
        player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        player->SetUInt32Value(PLAYER_DUEL_TEAM, 1);

        //if player class = hunter || warlock remove pet if alive
        if ((player->getClass() == CLASS_HUNTER) || (player->getClass() == CLASS_WARLOCK))
        {
            if (Pet *pet = player->GetPet())
            {
                pet->SavePetToDB(PET_SAVE_AS_CURRENT);
                // not let dismiss dead pet
                if (pet && pet->isAlive())
                    player->RemovePet(pet,PET_SAVE_NOT_IN_SLOT);
            }
        }

        //m_session->GetPlayer()->CastSpell(player,spellID,false);
        if (SpellEntry const *spellInfo = sSpellStore.LookupEntry(9454))
        {
            for (uint32 i = 0;i<3;i++)
            {
                uint8 eff = spellInfo->Effect[i];
                if (eff>=TOTAL_SPELL_EFFECTS)
                    continue;
                if (eff == SPELL_EFFECT_APPLY_AREA_AURA_PARTY || eff == SPELL_EFFECT_APPLY_AURA ||
                    eff == SPELL_EFFECT_PERSISTENT_AREA_AURA || eff == SPELL_EFFECT_APPLY_AREA_AURA_FRIEND ||
                    eff == SPELL_EFFECT_APPLY_AREA_AURA_ENEMY)
                {
                    Aura *Aur = CreateAura(spellInfo, i, NULL, player);
                    player->AddAura(Aur);
                }
            }
        }

        //save player
        player->SaveToDB();
    }
    return true;
}

bool ChatHandler::HandleUnFreezeCommand(const char *args)
{
    std::string name;
    Player* player;
    char *TargetName = strtok((char*)args, " "); //get entered name
    if (!TargetName) //if no name entered use target
    {
        player = getSelectedPlayer();
        if (player) //prevent crash with creature as target
            name = player->GetName();
    }

    else // if name entered
    {
        name = TargetName;
        normalizePlayerName(name);
        player = ObjectAccessor::Instance().FindPlayerByName(name.c_str()); //get player by name
    }

    //effect
    if (player)
    {
        PSendSysMessage(LANG_COMMAND_UNFREEZE,name.c_str());

        //Reset player faction + allow combat + allow duels
        player->setFactionForRace(player->getRace());
        player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

        //allow movement and spells
        player->RemoveAurasDueToSpell(9454);

        //save player
        player->SaveToDB();
    }

    if (!player)
    {
        if (TargetName)
        {
            //check for offline players
            QueryResult_AutoPtr result = CharacterDatabase.PQuery("SELECT characters.guid FROM characters WHERE characters.name = '%s'",name.c_str());
            if (!result)
            {
                SendSysMessage(LANG_COMMAND_FREEZE_WRONG);
                return true;
            }
            //if player found: delete his freeze aura
            Field *fields=result->Fetch();
            uint64 pguid = fields[0].GetUInt64();

            CharacterDatabase.PQuery("DELETE FROM character_aura WHERE character_aura.spell = 9454 AND character_aura.guid = '%u'",pguid);
            PSendSysMessage(LANG_COMMAND_UNFREEZE,name.c_str());
            return true;
        }
        else
        {
            SendSysMessage(LANG_COMMAND_FREEZE_WRONG);
            return true;
        }
    }

    return true;
}

bool ChatHandler::HandleListFreezeCommand(const char * /*args*/)
{
    //Get names from DB
    QueryResult_AutoPtr result = CharacterDatabase.Query("SELECT characters.name FROM characters LEFT JOIN character_aura ON (characters.guid = character_aura.guid) WHERE character_aura.spell = 9454");
    if (!result)
    {
        SendSysMessage(LANG_COMMAND_NO_FROZEN_PLAYERS);
        return true;
    }
    //Header of the names
    PSendSysMessage(LANG_COMMAND_LIST_FREEZE);

    //Output of the results
    do
    {
        Field *fields = result->Fetch();
        std::string fplayers = fields[0].GetCppString();
        PSendSysMessage(LANG_COMMAND_FROZEN_PLAYERS,fplayers.c_str());
    } while (result->NextRow());

    return true;
}

bool ChatHandler::HandleGroupLeaderCommand(const char *args)
{
    Player* plr  = NULL;
    Group* group = NULL;
    uint64 guid  = 0;
    char* cname  = strtok((char*)args, " ");

    if (GetPlayerGroupAndGUIDByName(cname, plr, group, guid))
        if (group && group->GetLeaderGUID() != guid)
            group->ChangeLeader(guid);

    return true;
}

bool ChatHandler::HandleGroupDisbandCommand(const char *args)
{
    Player* plr  = NULL;
    Group* group = NULL;
    uint64 guid  = 0;
    char* cname  = strtok((char*)args, " ");

    if (GetPlayerGroupAndGUIDByName(cname, plr, group, guid))
        if (group)
            group->Disband();

    return true;
}

bool ChatHandler::HandleGroupRemoveCommand(const char *args)
{
    Player* plr  = NULL;
    Group* group = NULL;
    uint64 guid  = 0;
    char* cname  = strtok((char*)args, " ");

    if (GetPlayerGroupAndGUIDByName(cname, plr, group, guid, true))
        if (group)
            group->RemoveMember(guid, 0);

    return true;
}

bool ChatHandler::HandlePossessCommand(const char * /*args*/)
{
    Unit* pUnit = getSelectedUnit();
    if (!pUnit)
        return false;

    m_session->GetPlayer()->CastSpell(pUnit, 530, true);
    return true;
}

bool ChatHandler::HandleUnPossessCommand(const char * /*args*/)
{
    Unit* pUnit = getSelectedUnit();
    if (!pUnit)
        pUnit = m_session->GetPlayer();

    pUnit->RemoveSpellsCausingAura(SPELL_AURA_MOD_CHARM);
    pUnit->RemoveSpellsCausingAura(SPELL_AURA_MOD_POSSESS_PET);
    pUnit->RemoveSpellsCausingAura(SPELL_AURA_MOD_POSSESS);

    return true;
}

bool ChatHandler::HandleBindSightCommand(const char * /*args*/)
{
    Unit* pUnit = getSelectedUnit();
    if (!pUnit)
        return false;

    m_session->GetPlayer()->CastSpell(pUnit, 6277, true);
    return true;
}

bool ChatHandler::HandleUnbindSightCommand(const char * /*args*/)
{
    if (m_session->GetPlayer()->isPossessing())
        return false;

    m_session->GetPlayer()->StopCastingBindSight();
    return true;
}

bool ChatHandler::HandleMmap(const char* args)
{
    if (*args)
    {
        std::string argstr = (char*)args;
        if (argstr == "on")
        {
            sWorld.setConfig(CONFIG_BOOL_MMAP_ENABLED, true);
            SendSysMessage("WORLD: mmaps are now ENABLED (individual map settings still in effect)");
        }
        else
        {
            sWorld.setConfig(CONFIG_BOOL_MMAP_ENABLED, false);
            SendSysMessage("WORLD: mmaps are now DISABLED");
        }
        return true;
    }

    bool on = sWorld.getConfig(CONFIG_BOOL_MMAP_ENABLED);
    PSendSysMessage("mmaps are %sabled", on ? "en" : "dis");

    return true;
}

bool ChatHandler::HandleMmapTestArea(const char* args)
{
    float radius = 40.0f;

    if (*args)
        radius = (float)atof((char*)args);

    CellPair pair(BlizzLike::ComputeCellPair( m_session->GetPlayer()->GetPositionX(), m_session->GetPlayer()->GetPositionY()) );
    Cell cell(pair);
    cell.SetNoCreate();

    std::list<Creature*> creatureList;

    BlizzLike::AnyUnitInObjectRangeCheck go_check(m_session->GetPlayer(), radius);
    BlizzLike::CreatureListSearcher<BlizzLike::AnyUnitInObjectRangeCheck> go_search(creatureList, go_check);
    TypeContainerVisitor<BlizzLike::CreatureListSearcher<BlizzLike::AnyUnitInObjectRangeCheck>, GridTypeMapContainer> go_visit(go_search);

    // Get Creatures
    cell.Visit(pair, go_visit, *(m_session->GetPlayer()->GetMap()), *(m_session->GetPlayer()), radius);

    if (!creatureList.empty())
    {
        PSendSysMessage("Found %i Creatures.", creatureList.size());

        uint32 paths = 0;
        uint32 uStartTime = getMSTime();

        float gx,gy,gz;
        m_session->GetPlayer()->GetPosition(gx,gy,gz);
        for (std::list<Creature*>::iterator itr = creatureList.begin(); itr != creatureList.end(); ++itr)
        {
            PathInfo((*itr), gx, gy, gz);
            ++paths;
        }

        uint32 uPathLoadTime = getMSTimeDiff(uStartTime, getMSTime());
        PSendSysMessage("Generated %i paths in %i ms", paths, uPathLoadTime);
    }
    else
    {
        PSendSysMessage("No creatures in %f yard range.", radius);
    }

    return true;
}