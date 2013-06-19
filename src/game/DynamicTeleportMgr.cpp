/*******************************************************
 * File:'DynamicTeleportMgr.cpp'
 * (c)2011 - Wolf Officious (ladislav.alexa@gmail.com)
 *******************************************************/

#include "DynamicTeleportMgr.h"
#include "ProgressBar.h"

bool DynamicTeleportMgr::Init()
{
    uint32 oldMSTime = getMSTime();

    mTeleportData.clear();
    mTeleportLoc.clear();

    //                                                           0            1         2     3        4     5    6           7           8           9          10
    QueryResult_AutoPtr result = WorldDatabase.Query("SELECT entry, menu_parent, menu_sub, icon, faction, name, map, position_x, position_y, position_z, position_o "
                                                     "FROM dynamic_teleporter ORDER BY entry");

    if(!result)
    {
        barGoLink bar(1);
        bar.step();

        sLog.outString();
        sLog.outString(">> Loaded 0 Dynamic Teleporter rows. DB table `dynamic_teleporter` is empty!");
        return false;
    }

    barGoLink bar(result->GetRowCount());

    uint32 counter = 0;

    do
    {
        Field *fields = result->Fetch();
        bar.step();

        TeleportData td;
        td.entry        = fields[0].GetUInt32();
        td.menu_parent  = fields[1].GetUInt32();
        td.menu_sub     = fields[2].GetInt32();
        td.icon         = fields[3].GetUInt8();
        td.faction      = fields[4].GetUInt32();
        td.name         = fields[5].GetString();

        if(td.menu_sub < 0)
        {
            TeleportLoc tl;
            tl.map          = fields[6].GetUInt32();
            tl.position_x   = fields[7].GetFloat();
            tl.position_y   = fields[8].GetFloat();
            tl.position_z   = fields[9].GetFloat();
            tl.position_o   = fields[10].GetFloat();
            mTeleportLoc[td.entry] = tl;
        }

        mTeleportData[counter] = td;
        ++counter;
    } while(result->NextRow());

    sLog.outString();
    sLog.outString(">> Loaded %u Dynamic Teleporter rows in %u ms", counter, getMSTimeDiff(oldMSTime, getMSTime()));

    _IsLoaded = true;
    return true;
}

void DynamicTeleportMgr::Update()
{
    _IsLoaded = false;
    Init();
}