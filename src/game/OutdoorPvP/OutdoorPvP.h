/*
 * This file is part of Everking. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef OUTDOOR_PVP_H
#define OUTDOOR_PVP_H

#include "Common.h"
#include "Entities/EntitiesMgr.h"
#include "Entities/ObjectGuid.h"
#include "Globals/SharedDefines.h"
#include "OutdoorPvPMgr.h"

class WorldPacket;

enum CapturePointArtKits
{
    CAPTURE_ARTKIT_ALLIANCE = 2,
    CAPTURE_ARTKIT_HORDE    = 1,
    CAPTURE_ARTKIT_NEUTRAL  = 21
};

enum CapturePointAnimations
{
    CAPTURE_ANIM_ALLIANCE   = 1,
    CAPTURE_ANIM_HORDE      = 0,
    CAPTURE_ANIM_NEUTRAL    = 2
};

typedef std::map<ObjectGuid /*playerGuid*/, bool /*isMainZone*/> GuidZoneMap;

class OutdoorPvP
{
        friend class OutdoorPvPMgr;

    public:
        OutdoorPvP() : m_isBattlefield(false) {}
        virtual ~OutdoorPvP() {}

        // called when the zone is initialized
        virtual void FillInitialWorldStates(WorldPacket& /*data*/, uint32& /*count*/) {}

        // Process Capture event
        virtual bool HandleEvent(uint32 /*eventId*/, Object* /*source*/, Object* /*target*/) { return false; }

        // handle capture objective complete
        virtual void HandleObjectiveComplete(uint32 /*eventId*/, const PlayerList& /*players*/, Team /*team*/) {}

        // Called when a creature is created
        virtual void HandleCreatureCreate(Creature* /*creature*/) {}

        // Called when a gameobject is created or removed
        virtual void HandleGameObjectCreate(GameObject* /*go*/);
        virtual void HandleGameObjectRemove(GameObject* /*go*/);

        // Called on creature death
        virtual void HandleCreatureDeath(Creature* /*creature*/) {}

        // called when a player uses a gameobject related to outdoor pvp events
        virtual bool HandleGameObjectUse(Player* /*player*/, GameObject* /*go*/) { return false; }

        // called when a player triggers an areatrigger
        virtual bool HandleAreaTrigger(Player* /*player*/, uint32 /*triggerId*/) { return false; }

        // called when a player drops a flag
        virtual bool HandleDropFlag(Player* /*player*/, uint32 /*spellId*/) { return false; }

        // update - called by the OutdoorPvPMgr
        virtual void Update(uint32 /*diff*/) {}

        // Handle player kill
        void HandlePlayerKill(Player* killer, Unit* victim);

        // applies buff to a team inside the specific zone
        void BuffTeam(Team team, uint32 spellId, bool remove = false, const uint32 areaId = 0);

        // send world state update to all players present
        void SendUpdateWorldState(uint32 field, uint32 value);

        // set banner visual
        void SetBannerVisual(const WorldObject* objRef, ObjectGuid goGuid, uint32 artKit, uint32 animId);
        void SetBannerVisual(GameObject* go, uint32 artKit, uint32 animId);

        // check if zone is battlefield
        bool IsBattlefield() const { return m_isBattlefield; }

        // Handle script condition fulfillment
        virtual bool IsConditionFulfilled(Player const* /*source*/, uint32 /*conditionId*/, WorldObject const* /*conditionSource*/, uint32 /*conditionSourceType*/) { return false; }

        // Handle script condition state change by an external factor
        virtual void HandleConditionStateChange(uint32 /*conditionId*/, bool /*state*/) {}

        void SetGraveYardLinkTeam(uint32 id, uint32 locKey, Team team, uint32 mapId);

        // Handle achievement criteria requirements
        virtual bool CheckAchievementCriteriaMeet(uint32 /*criteria_id*/, Player const* /*source*/, Unit const* /*target*/, uint32 /*miscvalue1*/) { return false; }

    protected:

        // Player related stuff
        virtual void HandlePlayerEnterZone(Player* /*player*/, bool /*isMainZone*/);
        virtual void HandlePlayerLeaveZone(Player* /*player*/, bool /*isMainZone*/);
        virtual void HandlePlayerEnterArea(Player* /*player*/, uint32 /*areaId*/, bool /*isMainZone*/) {}
        virtual void HandlePlayerLeaveArea(Player* /*player*/, uint32 /*areaId*/, bool /*isMainZone*/) {}

        // remove world states
        virtual void SendRemoveWorldStates(Player* /*player*/) {}

        // handle npc/player kill
        virtual void HandlePlayerKillInsideArea(Player* /*killer*/, Unit* /*victim*/) {}

        // get banner artkit based on controlling team
        uint32 GetBannerArtKit(Team team, uint32 artKitAlliance = CAPTURE_ARTKIT_ALLIANCE, uint32 artKitHorde = CAPTURE_ARTKIT_HORDE, uint32 artKitNeutral = CAPTURE_ARTKIT_NEUTRAL) const;

        // Handle gameobject spawn / despawn
        void RespawnGO(const WorldObject* objRef, ObjectGuid goGuid, bool respawn);

        // store the players inside the area
        GuidZoneMap m_zonePlayers;

        bool m_isBattlefield;
};

#endif
