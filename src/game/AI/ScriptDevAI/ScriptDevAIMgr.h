/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef SC_SCRIPTMGR_H
#define SC_SCRIPTMGR_H

#include "Common.h"
#include "DBCStructure.h"
#include "SQLStorages.h"

class Player;
class Creature;
class CreatureAI;
class InstanceData;
class Quest;
class Item;
class GameObject;
class SpellCastTargets;
class Map;
class Unit;
class WorldObject;
class Aura;
class Object;
class ObjectGuid;

// *********************************************************
// ************** Some defines used globally ***************

// Basic defines
#define VISIBLE_RANGE       (166.0f)                        // MAX visible range (size of grid)
#define DEFAULT_TEXT        "<ScriptDev2 Text Entry Missing!>"

/* Escort Factions
 * TODO: find better namings and definitions.
 * N=Neutral, A=Alliance, H=Horde.
 * NEUTRAL or FRIEND = Hostility to player surroundings (not a good definition)
 * ACTIVE or PASSIVE = Hostility to environment surroundings.
 */
enum EscortFaction
{
    FACTION_ESCORT_A_NEUTRAL_PASSIVE    = 10,
    FACTION_ESCORT_H_NEUTRAL_PASSIVE    = 33,
    FACTION_ESCORT_N_NEUTRAL_PASSIVE    = 113,

    FACTION_ESCORT_A_NEUTRAL_ACTIVE     = 231,
    FACTION_ESCORT_H_NEUTRAL_ACTIVE     = 232,
    FACTION_ESCORT_N_NEUTRAL_ACTIVE     = 250,

    FACTION_ESCORT_N_FRIEND_PASSIVE     = 290,
    FACTION_ESCORT_N_FRIEND_ACTIVE      = 495,

    FACTION_ESCORT_A_PASSIVE            = 774,
    FACTION_ESCORT_H_PASSIVE            = 775,

    FACTION_ESCORT_N_ACTIVE             = 1986,
    FACTION_ESCORT_H_ACTIVE             = 2046
};

// *********************************************************
// ************* Some structures used globally *************

struct Script
{
    Script() :
        pGossipHello(NULL), pGossipHelloGO(NULL), pGossipSelect(NULL), pGossipSelectGO(NULL),
        pGossipSelectWithCode(NULL), pGossipSelectGOWithCode(NULL),
        pDialogStatusNPC(NULL), pDialogStatusGO(NULL),
        pQuestAcceptNPC(NULL), pQuestAcceptGO(NULL), pQuestAcceptItem(NULL),
        pQuestRewardedNPC(NULL), pQuestRewardedGO(NULL),
        pGOUse(NULL), pItemUse(NULL), pAreaTrigger(NULL), pNpcSpellClick(NULL), pProcessEventId(NULL),
        pEffectDummyNPC(NULL), pEffectDummyGO(NULL), pEffectDummyItem(NULL), pEffectScriptEffectNPC(NULL),
        pEffectAuraDummy(NULL), GetAI(NULL), GetInstanceData(NULL)
    {}

    std::string Name;

    bool (*pGossipHello)(Player*, Creature*);
    bool (*pGossipHelloGO)(Player*, GameObject*);
    bool (*pGossipSelect)(Player*, Creature*, uint32, uint32);
    bool (*pGossipSelectGO)(Player*, GameObject*, uint32, uint32);
    bool (*pGossipSelectWithCode)(Player*, Creature*, uint32, uint32, const char*);
    bool (*pGossipSelectGOWithCode)(Player*, GameObject*, uint32, uint32, const char*);
    uint32(*pDialogStatusNPC)(const Player*, const Creature*);
    uint32(*pDialogStatusGO)(const Player*, const GameObject*);
    bool (*pQuestAcceptNPC)(Player*, Creature*, Quest const*);
    bool (*pQuestAcceptGO)(Player*, GameObject*, Quest const*);
    bool (*pQuestAcceptItem)(Player*, Item*, Quest const*);
    bool (*pQuestRewardedNPC)(Player*, Creature*, Quest const*);
    bool (*pQuestRewardedGO)(Player*, GameObject*, Quest const*);
    bool (*pGOUse)(Player*, GameObject*);
    bool (*pItemUse)(Player*, Item*, SpellCastTargets const&);
    bool (*pAreaTrigger)(Player*, AreaTriggerEntry const*);
    bool (*pNpcSpellClick)(Player*, Creature*, uint32);
    bool (*pProcessEventId)(uint32, Object*, Object*, bool);
    bool (*pEffectDummyNPC)(Unit*, uint32, SpellEffectIndex, Creature*, ObjectGuid);
    bool (*pEffectDummyGO)(Unit*, uint32, SpellEffectIndex, GameObject*, ObjectGuid);
    bool (*pEffectDummyItem)(Unit*, uint32, SpellEffectIndex, Item*, ObjectGuid);
    bool (*pEffectScriptEffectNPC)(Unit*, uint32, SpellEffectIndex, Creature*, ObjectGuid);
    bool (*pEffectAuraDummy)(const Aura*, bool);

    CreatureAI* (*GetAI)(Creature*);
    InstanceData* (*GetInstanceData)(Map*);

    void RegisterSelf(bool bReportError = true);
};

class ScriptDevAIMgr
{
public:
    ScriptDevAIMgr() : num_sc_scripts(0) {}
    ~ScriptDevAIMgr();

    void Initialize();
    void LoadScriptNames();
    void LoadAreaTriggerScripts();
    void LoadEventIdScripts();

    bool OnGossipHello(Player* pPlayer, Creature* pCreature);
    bool OnGossipHello(Player* pPlayer, GameObject* pGameObject);
    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 sender, uint32 action, const char* code);
    bool OnGossipSelect(Player* pPlayer, GameObject* pGameObject, uint32 sender, uint32 action, const char* code);
    bool OnQuestAccept(Player* pPlayer, Creature* pCreature, Quest const* pQuest);
    bool OnQuestAccept(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest);
    bool OnQuestAccept(Player* pPlayer, Item* pItem, Quest const* pQuest);
    bool OnQuestRewarded(Player* pPlayer, Creature* pCreature, Quest const* pQuest);
    bool OnQuestRewarded(Player* pPlayer, GameObject* pGameObject, Quest const* pQuest);
    uint32 GetDialogStatus(const Player* pPlayer, const Creature* pCreature) const;
    uint32 GetDialogStatus(const Player* pPlayer, const GameObject* pGameObject) const;
    bool OnGameObjectUse(Player* pPlayer, GameObject* pGameObject);
    bool OnItemUse(Player* pPlayer, Item* pItem, SpellCastTargets const& targets);
    bool OnAreaTrigger(Player* pPlayer, AreaTriggerEntry const* atEntry);
    bool OnProcessEvent(uint32 eventId, Object* pSource, Object* pTarget, bool isStart);
    bool OnEffectDummy(Unit* pCaster, uint32 spellId, SpellEffectIndex effIndex, Creature* pTarget, ObjectGuid originalCasterGuid);
    bool OnEffectDummy(Unit* pCaster, uint32 spellId, SpellEffectIndex effIndex, GameObject* pTarget, ObjectGuid originalCasterGuid);
    bool OnEffectDummy(Unit* pCaster, uint32 spellId, SpellEffectIndex effIndex, Item* pTarget, ObjectGuid originalCasterGuid);
    bool OnEffectScriptEffect(Unit* pCaster, uint32 spellId, SpellEffectIndex effIndex, Creature* pTarget, ObjectGuid originalCasterGuid);
    bool OnAuraDummy(Aura const* pAura, bool apply);
    bool OnNpcSpellClick(Player* pPlayer, Creature* pClickedCreature, uint32 spellId);

    void AddScript(uint32 id, Script* script);
    Script* GetScript(uint32 id) const;
    const char* GetScriptName(uint32 id) const { return id < m_scriptNames.size() ? m_scriptNames[id].c_str() : ""; }
    uint32 GetScriptId(const char* name) const;
    uint32 GetScriptIdsCount() const { return m_scriptNames.size(); }

    CreatureAI* GetCreatureAI(Creature* creature);

    InstanceData* CreateInstanceData(Map* pMap);
    uint32 GetAreaTriggerScriptId(uint32 triggerId) const;
    uint32 GetEventIdScriptId(uint32 eventId) const;

private:
    typedef std::vector<Script*> SDScriptVec;
    typedef std::vector<std::string> ScriptNameMap;
    typedef std::unordered_map<uint32, uint32> AreaTriggerScriptMap;
    typedef std::unordered_map<uint32, uint32> EventIdScriptMap;

    int num_sc_scripts;
    SDScriptVec m_scripts;

    AreaTriggerScriptMap    m_AreaTriggerScripts;
    EventIdScriptMap        m_EventIdScripts;

    ScriptNameMap           m_scriptNames;
};

// *********************************************************
// ************* Some functions used globally **************

// Generic scripting text function
void DoScriptText(int32 iTextEntry, WorldObject* pSource, Unit* pTarget = nullptr);
void DoOrSimulateScriptTextForMap(int32 iTextEntry, uint32 uiCreatureEntry, Map* pMap, Creature* pCreatureSource = nullptr, Unit* pTarget = nullptr);

#define sScriptDevAIMgr MaNGOS::Singleton<ScriptDevAIMgr>::Instance()

#endif
