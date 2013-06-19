/*
 * Darmix-Core Copyright (C) 2013  Deremix
 * Integrated Files: CREDITS.md and LICENSE.md
 */

/* ScriptData
Name: Instance_Scarlet_Monastery
Complete(%): 50
Comment:
Category: Scarlet Monastery
EndScriptData */

#include "ScriptPCH.h"
#include "scarlet_monastery.h"

#define ENTRY_PUMPKIN_SHRINE    186267
#define ENTRY_HORSEMAN          23682
#define ENTRY_HEAD              23775
#define ENTRY_PUMPKIN           23694

#define MAX_ENCOUNTER 2

struct instance_scarlet_monastery : public ScriptedInstance
{
    instance_scarlet_monastery(Map* pMap) : ScriptedInstance(pMap) {Initialize();};

    uint64 PumpkinShrineGUID;
    uint64 HorsemanGUID;
    uint64 HeadGUID;
    std::set<uint64> HorsemanAdds;

    uint64 MograineGUID;
    uint64 WhitemaneGUID;
    uint64 VorrelGUID;
    uint64 DoorHighInquisitorGUID;

    uint32 m_auiEncounter[MAX_ENCOUNTER];

    void Initialize()
    {
        memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));

        PumpkinShrineGUID  = 0;
        HorsemanGUID = 0;
        HeadGUID = 0;
        HorsemanAdds.clear();

        MograineGUID = 0;
        WhitemaneGUID = 0;
        VorrelGUID = 0;
        DoorHighInquisitorGUID = 0;
    }

    void OnGameObjectCreate(GameObject* pGo, bool /*add*/)
    {
        switch (pGo->GetEntry())
        {
        case ENTRY_PUMPKIN_SHRINE: PumpkinShrineGUID = pGo->GetGUID();break;
        case 104600: DoorHighInquisitorGUID = pGo->GetGUID(); break;
        }
    }

    void OnCreatureCreate(Creature* pCreature, bool /*add*/)
    {
        switch (pCreature->GetEntry())
        {
            case ENTRY_HORSEMAN:    HorsemanGUID = pCreature->GetGUID(); break;
            case ENTRY_HEAD:        HeadGUID = pCreature->GetGUID(); break;
            case ENTRY_PUMPKIN:     HorsemanAdds.insert(pCreature->GetGUID());break;
            case 3976: MograineGUID = pCreature->GetGUID(); break;
            case 3977: WhitemaneGUID = pCreature->GetGUID(); break;
            case 3981: VorrelGUID = pCreature->GetGUID(); break;
        }
    }

    void SetData(uint32 type, uint32 data)
    {
        switch (type)
        {
        case TYPE_MOGRAINE_AND_WHITE_EVENT:
            if (data == IN_PROGRESS)
                DoUseDoorOrButton(DoorHighInquisitorGUID);
            if (data == FAIL)
                DoUseDoorOrButton(DoorHighInquisitorGUID);

            m_auiEncounter[0] = data;
            break;
        case GAMEOBJECT_PUMPKIN_SHRINE:
            HandleGameObject(PumpkinShrineGUID, false);
            break;
        case DATA_HORSEMAN_EVENT:
            m_auiEncounter[1] = data;
            if (data == DONE)
            {
                for (std::set<uint64>::const_iterator itr = HorsemanAdds.begin(); itr != HorsemanAdds.end(); ++itr)
                {
                    Creature* add = instance->GetCreature(*itr);
                    if (add && add->isAlive())
                        add->Kill(add);
                }
                HorsemanAdds.clear();
                HandleGameObject(PumpkinShrineGUID, false);
            }
            break;
        }
    }

    uint64 GetData64(uint32 type)
    {
        switch (type)
        {
            //case GAMEOBJECT_PUMPKIN_SHRINE:   return PumpkinShrineGUID;
            //case DATA_HORSEMAN:               return HorsemanGUID;
            //case DATA_HEAD:                   return HeadGUID;
            case DATA_MOGRAINE:             return MograineGUID;
            case DATA_WHITEMANE:            return WhitemaneGUID;
            case DATA_VORREL:               return VorrelGUID;
            case DATA_DOOR_WHITEMANE:       return DoorHighInquisitorGUID;
        }
        return 0;
    }

    uint32 GetData(uint32 type)
    {
        if (type == TYPE_MOGRAINE_AND_WHITE_EVENT)
            return m_auiEncounter[0];
        if (type == DATA_HORSEMAN_EVENT)
            return m_auiEncounter[1];
        return 0;
    }
};

InstanceData* GetInstanceData_instance_scarlet_monastery(Map* pMap)
{
    return new instance_scarlet_monastery(pMap);
}

void AddSC_instance_scarlet_monastery()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "instance_scarlet_monastery";
    newscript->GetInstanceData = &GetInstanceData_instance_scarlet_monastery;
    newscript->RegisterSelf();
}

