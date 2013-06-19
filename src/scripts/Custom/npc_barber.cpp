/*******************************************************
 * File:'barber.cpp'
 * ScriptName:'barber'
 *******************************************************/

#include "ScriptPCH.h"

typedef struct maxStyles_struct {
    uint8 maxMale;
    uint8 maxFemale;
} maxStyles_t;

maxStyles_t maxHairStyles[MAX_RACES] =
{
    {0,0},  //                        0
    {11,18},// RACE_HUMAN           = 1,
    {6,6},  // RACE_ORC             = 2,
    {10,13},// RACE_DWARF           = 3,
    {6,6},  // RACE_NIGHTELF        = 4,
    {10,9}, // RACE_UNDEAD_PLAYER   = 5,
    {7,6},  // RACE_TAUREN          = 6,
    {6,6},  // RACE_GNOME           = 7,
    {5,4},  // RACE_TROLL           = 8,
    {0,0},  // RACE_GOBLIN          = 9,
    {9,13}, // RACE_BLOODELF        = 10,
    {7,10}, // RACE_DRAENEI         = 11
};

uint8 maxHairColor[MAX_RACES] =
{
    0,  //                            0
    9,  // RACE_HUMAN               = 1,
    7,  // RACE_ORC                 = 2,
    9,  // RACE_DWARF               = 3,
    7,  // RACE_NIGHTELF            = 4,
    9,  // RACE_UNDEAD_PLAYER       = 5,
    2,  // RACE_TAUREN              = 6,
    8,  // RACE_GNOME               = 7,
    9,  // RACE_TROLL               = 8,
    0,  // RACE_GOBLIN              = 9,
    9,  // RACE_BLOODELF            = 10,
    6,  // RACE_DRAENEI             = 11
};

maxStyles_t maxFacialFeatures[MAX_RACES] =
{
    {0,0},  //                        0
    {8,6},  // RACE_HUMAN           = 1,
    {10,6}, // RACE_ORC             = 2,
    {10,5}, // RACE_DWARF           = 3,
    {5,9},  // RACE_NIGHTELF        = 4,
    {16,7}, // RACE_UNDEAD_PLAYER   = 5,
    {6,4},  // RACE_TAUREN          = 6,
    {7,6},  // RACE_GNOME           = 7,
    {10,5}, // RACE_TROLL           = 8,
    {0,0},  // RACE_GOBLIN          = 9,
    {10,9}, // RACE_BLOODELF        = 10,
    {7,6},  // RACE_DRAENEI         = 11
};

// FCE PROTOTYPES
inline bool PlayerSitting(Player *player);
inline void ChangeEffect (Player *player);
void SelectHairStyle     (Player *player, int change);
void SelectHairColor     (Player *player, int change);
void SelectFacialFeature (Player *player, int change);
std::string GetHairStyleGossipMsg(Player *player);
std::string GetHairColorGossipMsg(Player *player);
std::string GetFeatureGossipMsg  (Player *player);

// DEFINITIONS
#define GOSSIP_SENDER_OPTION 50
#define GOSSIP_SENDER_SUBOPTION 51

/******************/
/* AUXILIARY FCES */
/******************/
inline bool PlayerSitting(Player *player)
{
    uint8 standState = player->getStandState();
    if(player->IsStandState() || standState == UNIT_STAND_STATE_SIT || standState == UNIT_STAND_STATE_SLEEP || standState == UNIT_STAND_STATE_KNEEL)
    {
        return false;
    }
    return true;
}

inline void ChangeEffect(Player *player)
{
    player->SendUpdateToPlayer(player);
    WorldPacket data(SMSG_FORCE_DISPLAY_UPDATE, 8);
    data << player->GetGUID();
    player->SendMessageToSet(&data, true);
}

void SelectHairStyle(Player *player, int change)
{
    uint8 max=maxHairStyles[player->getRace()].maxMale;
    if(player->getGender() == GENDER_FEMALE)
        max=maxHairStyles[player->getRace()].maxFemale;

    int current = player->GetByteValue(PLAYER_BYTES, 2);

    current += change;

    if(current > max)
        current = 0;
    else if(current < 0)
        current = max;

    player->SetByteValue(PLAYER_BYTES, 2, current);
    ChangeEffect(player);
}

void SelectHairColor(Player *player, int change)
{
    uint8 max=maxHairColor[player->getRace()];
    int current = player->GetByteValue(PLAYER_BYTES, 3);

    current += change;

    if(current > max)
        current = 0;
    else if(current < 0)
        current = max;

    player->SetByteValue(PLAYER_BYTES, 3, current);
    ChangeEffect(player);
}

void SelectFacialFeature(Player *player, int change)
{
    uint8 max=maxFacialFeatures[player->getRace()].maxMale;
    if(player->getGender() == GENDER_FEMALE)
        max=maxFacialFeatures[player->getRace()].maxFemale;

    int current = player->GetByteValue(PLAYER_BYTES_2, 0); 

    current += change;

    if(current > max)
        current = 0;
    else if(current < 0)
        current = max;

    player->SetByteValue(PLAYER_BYTES_2, 0, current);
    ChangeEffect(player);
}

std::string GetHairStyleGossipMsg(Player *player)
{
    if(player->getRace() == RACE_TAUREN)
        return "Zmenit styl rohu..";
    else
        return "Zmenit uces..";
}

std::string GetHairColorGossipMsg(Player *player)
{
    if(player->getRace() == RACE_TAUREN)
        return "Zmenit barvu rohu..";
    else
        return "Zmenit barvu vlasu..";
}

std::string GetFeatureGossipMsg(Player *player)
{
    uint8 pRace   = player->getRace();
    uint8 pGender = player->getGender();

    if(pGender == GENDER_FEMALE)
    {
        if(pRace == RACE_HUMAN || pRace == RACE_ORC || pRace == RACE_DWARF || pRace == RACE_GNOME || pRace == RACE_BLOODELF)
            return "Zmenit nausnice..";

        if(pRace == RACE_NIGHTELF)
            return "Zmenit tetovani obliceje..";

        if(pRace == RACE_TAUREN)
            return "Zmenit vlasy..";

        if(pRace == RACE_DRAENEI)
            return "Zmenit rohy..";
    }

    if(pRace == RACE_TROLL)
        return "Zmenit kly..";

    if(pRace == RACE_UNDEAD_PLAYER)
        return "Zmenit doplnky obliceje..";

    return "Zmenit vousy..";
}

/********************/
/* MAIN SCRIPT FCES */
/********************/
bool GossipHello_barber(Player *player, Creature *creature)
{
    int32 text = 0;

    creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);

    if(!PlayerSitting(player))
    {
        if(player->getGender() == GENDER_MALE)
            creature->Whisper("Pane, posadte se prosim.", player->GetGUID());
        else if(player->getGender() == GENDER_FEMALE)
            creature->Whisper("Oh, slecno, sednete si prosim. :)", player->GetGUID());

        player->GetSession()->SendNotification("Musite se posadit na zidli..");
        return true;
    }

    if(player->isInCombat())
    {
        if(player->getGender() == GENDER_MALE)
            creature->Whisper("Pane, obavam se ze prave bojujete! BEZTE PROSIM PRYC! POMOOOC!! >:o", player->GetGUID());
        else if(player->getGender() == GENDER_FEMALE)
            creature->Whisper("Pani, obavam se ze prave bojujete! BEZTE PROSIM PRYC! POMOOOC!! >:o", player->GetGUID());

        player->GetSession()->SendNotification("Jsi v Combatu!");
        return true;
    }

    creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, 94);

    switch(player->getRace())
    {
        case RACE_HUMAN:
            text = player->getGender() == GENDER_FEMALE ? 50013 : 50012;
            break;
        case RACE_ORC:
            text = player->getGender() == GENDER_FEMALE ? 50002 : 50001;
            break;
        case RACE_DWARF:
            text = player->getGender() == GENDER_FEMALE ? 50015 : 50014;
            break;
        case RACE_NIGHTELF:
            text = player->getGender() == GENDER_FEMALE ? 50019 : 50018;
            break;
        case RACE_UNDEAD_PLAYER:
            text = player->getGender() == GENDER_FEMALE ? 50008 : 50007;
            break;
        case RACE_TAUREN:
            text = player->getGender() == GENDER_FEMALE ? 50006 : 50005;
            break;
        case RACE_GNOME:
            text = player->getGender() == GENDER_FEMALE ? 50017 : 50016;
            break;        
        case RACE_DRAENEI:
            text = player->getGender() == GENDER_FEMALE ? 50021 : 50020;
            break;
        case RACE_TROLL:
            text = player->getGender() == GENDER_FEMALE ? 50004 : 50003;
            break;
        case RACE_BLOODELF:
            text = player->getGender() == GENDER_FEMALE ? 50010 : 50009;
            break;
        default:
            break;
    }

    if(PlayerSitting(player))
    {
        if(!player->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM))
            player->ToggleFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM);

        player->ADD_GOSSIP_ITEM( 0, GetHairStyleGossipMsg(player).c_str(),  GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 0, GetHairColorGossipMsg(player).c_str(),  GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 0, GetFeatureGossipMsg(player).c_str(),    GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
        player->SEND_GOSSIP_MENU(text, creature->GetGUID());
        creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
        return true;
    }
    else
    {
        player->CLOSE_GOSSIP_MENU();
        return true;
    }
}

bool GossipSelect_barber(Player *player, Creature *creature, uint32 sender, uint32 action)
{
    switch(action)
    {
    case GOSSIP_ACTION_INFO_DEF+1:   
        player->ADD_GOSSIP_ITEM( 0, GetHairStyleGossipMsg(player).c_str(),  GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 0, GetHairColorGossipMsg(player).c_str(),  GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 0, GetFeatureGossipMsg(player).c_str(),    GOSSIP_SENDER_OPTION, GOSSIP_ACTION_INFO_DEF + 6);
        player->SEND_GOSSIP_MENU(50023, creature->GetGUID());
        break;

    // HAIR/HORN STYLE
    case GOSSIP_ACTION_INFO_DEF+2: // next
        if(sender == GOSSIP_SENDER_SUBOPTION)
            SelectHairStyle(player,1);

    case GOSSIP_ACTION_INFO_DEF+3: // previous
        if(action == GOSSIP_ACTION_INFO_DEF+3 && sender == GOSSIP_SENDER_SUBOPTION)
            SelectHairStyle(player,-1);

        // options
        player->ADD_GOSSIP_ITEM( 0, "Dalsi >",      GOSSIP_SENDER_SUBOPTION, GOSSIP_ACTION_INFO_DEF + 2);
        player->ADD_GOSSIP_ITEM( 0, "Predchozi <",  GOSSIP_SENDER_SUBOPTION, GOSSIP_ACTION_INFO_DEF + 3);
        player->ADD_GOSSIP_ITEM( 0, "TO BERU!",     GOSSIP_SENDER_SUBOPTION, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(50024, creature->GetGUID());
        break;

    // HAIR COLOR
    case GOSSIP_ACTION_INFO_DEF+4: // next
        if(sender == GOSSIP_SENDER_SUBOPTION)
            SelectHairColor(player,1);

    case GOSSIP_ACTION_INFO_DEF+5: // previous
        if(action == GOSSIP_ACTION_INFO_DEF+5 && sender == GOSSIP_SENDER_SUBOPTION)
            SelectHairColor(player,-1);                                     

        // options
        player->ADD_GOSSIP_ITEM( 0, "Dalsi >",      GOSSIP_SENDER_SUBOPTION, GOSSIP_ACTION_INFO_DEF + 4);
        player->ADD_GOSSIP_ITEM( 0, "Predchozi <",  GOSSIP_SENDER_SUBOPTION, GOSSIP_ACTION_INFO_DEF + 5);
        player->ADD_GOSSIP_ITEM( 0, "TO BERU!",     GOSSIP_SENDER_SUBOPTION, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(50024, creature->GetGUID());
        break;

        // FACIAL FEATURE
    case GOSSIP_ACTION_INFO_DEF+6: // next
        if(sender == GOSSIP_SENDER_SUBOPTION)
            SelectFacialFeature(player,1);

    case GOSSIP_ACTION_INFO_DEF+7: // previous
        if(action == GOSSIP_ACTION_INFO_DEF+7 && sender == GOSSIP_SENDER_SUBOPTION)
            SelectFacialFeature(player,-1);

        // options
        player->ADD_GOSSIP_ITEM( 0, "Dalsi >",      GOSSIP_SENDER_SUBOPTION, GOSSIP_ACTION_INFO_DEF + 6);
        player->ADD_GOSSIP_ITEM( 0, "Predchozi <",  GOSSIP_SENDER_SUBOPTION, GOSSIP_ACTION_INFO_DEF + 7);
        player->ADD_GOSSIP_ITEM( 0, "TO BERU!",     GOSSIP_SENDER_SUBOPTION, GOSSIP_ACTION_INFO_DEF + 1);
        player->SEND_GOSSIP_MENU(50024, creature->GetGUID());
        break;

    default:
        player->CLOSE_GOSSIP_MENU();
        break;
    }
    return true;
}

void AddSC_barber()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "barber";
    newscript->pGossipHello  = &GossipHello_barber;
    newscript->pGossipSelect = &GossipSelect_barber;
    newscript->RegisterSelf();
}