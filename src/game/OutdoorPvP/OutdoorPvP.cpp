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

#include "OutdoorPvP.h"
#include "Entities/Object.h"
#include "Entities/GameObject.h"
#include "Entities/Player.h"
#include "Globals/ObjectMgr.h"
#include "World/World.h"

/**
   Function that adds a player to the players of the affected outdoor pvp zones

   @param   player to add
   @param   whether zone is main outdoor pvp zone or a affected zone
 */
void OutdoorPvP::SetGraveYardLinkTeam(uint32 id, uint32 locKey, Team team, uint32 mapId)
{
    sWorld.GetMessager().AddMessage([=](World* world) { world->GetGraveyardManager().SetGraveYardLinkTeam(id, locKey, team); });
    sMapMgr.DoForAllMapsWithMapId(mapId, [=](Map* map) { map->GetMessager().AddMessage([=](Map* map) {map->GetGraveyardManager().SetGraveYardLinkTeam(id, locKey, team); }); });
}

void OutdoorPvP::HandlePlayerEnterZone(Player* player, bool isMainZone)
{
    m_zonePlayers[player->GetObjectGuid()] = isMainZone;
}

/**
   Function that removes a player from the players of the affected outdoor pvp zones

   @param   player to remove
   @param   whether zone is main outdoor pvp zone or a affected zone
 */
void OutdoorPvP::HandlePlayerLeaveZone(Player* player, bool isMainZone)
{
    if (m_zonePlayers.erase(player->GetObjectGuid()))
    {
        // remove the world state information from the player
        if (isMainZone && !player->GetSession()->PlayerLogout())
            SendRemoveWorldStates(player);

        sLog.outDebug("Player %s left an Outdoor PvP zone", player->GetName());
    }
}

/**
   Function that updates the world state for all the players of the outdoor pvp zone

   @param   world state to update
   @param   new world state value
 */
void OutdoorPvP::SendUpdateWorldState(uint32 field, uint32 value)
{
    for (GuidZoneMap::const_iterator itr = m_zonePlayers.begin(); itr != m_zonePlayers.end(); ++itr)
    {
        // only send world state update to main zone
        if (!itr->second)
            continue;

        if (Player* player = sObjectMgr.GetPlayer(itr->first))
            player->SendUpdateWorldState(field, value);
    }
}

void OutdoorPvP::HandleGameObjectCreate(GameObject* go)
{
    // set initial data and activate capture points
    if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
    {
        CapturePointSliderMap const* capturePoints = sOutdoorPvPMgr.GetCapturePointSliderMap();
        CapturePointSliderMap::const_iterator itr = capturePoints->find(go->GetEntry());
        if (itr != capturePoints->end())
            go->SetCapturePointSlider(itr->second.Value, itr->second.IsLocked);
        else
            go->SetCapturePointSlider(go->GetGOInfo()->capturePoint.startingValue, false);
    }
}

void OutdoorPvP::HandleGameObjectRemove(GameObject* go)
{
    // save capture point slider value (negative value if locked)
    if (go->GetGOInfo()->type == GAMEOBJECT_TYPE_CAPTURE_POINT)
    {
        CapturePointSlider value(go->GetCapturePointSliderValue(), go->GetLootState() != GO_ACTIVATED);
        sOutdoorPvPMgr.SetCapturePointSlider(go->GetEntry(), value);
    }
}

/**
   Function that handles kills in the main outdoor pvp zones

   @param   player who killed another unit
   @param   victim who was killed
 */
void OutdoorPvP::HandlePlayerKill(Player* killer, Unit* victim)
{
    if (Group* group = killer->GetGroup())
    {
        for (GroupReference* itr = group->GetFirstMember(); itr != nullptr; itr = itr->next())
        {
            Player* groupMember = itr->getSource();

            if (!groupMember)
                continue;

            // skip if too far away
            if (!groupMember->IsAtGroupRewardDistance(victim))
                continue;

            if (victim->IsTrivialForTarget(groupMember))
                continue;

            // creature kills must be notified, even if not inside objective / not outdoor pvp active
            // player kills only count if active and inside objective
            if (groupMember->CanUseCapturePoint())
                HandlePlayerKillInsideArea(groupMember, victim);
        }
    }
    else
    {
        // creature kills must be notified, even if not inside objective / not outdoor pvp active
        if (killer && killer->CanUseCapturePoint() && !victim->IsTrivialForTarget(killer))
            HandlePlayerKillInsideArea(killer, victim);
    }
}

// apply a team buff for the main and affected zones
void OutdoorPvP::BuffTeam(Team team, uint32 spellId, bool remove /*= false*/, const uint32 areaId /*= 0*/)
{
    for (GuidZoneMap::const_iterator itr = m_zonePlayers.begin(); itr != m_zonePlayers.end(); ++itr)
    {
        Player* player = sObjectMgr.GetPlayer(itr->first);
        if (player && player->GetTeam() == team)
        {
            // validate area id for buffs which aren't applied to the entire zone
            if (areaId && player->GetAreaId() != areaId)
                continue;

            if (remove)
            {
                ObjectGuid guid = player->GetObjectGuid();
                player->GetMap()->GetMessager().AddMessage([guid, spellId](Map* map) -> void
                {
                    if (Player* player = map->GetPlayer(guid))
                        player->RemoveAuraHolderFromStack(spellId);
                });
            }
            else
            {
                ObjectGuid guid = player->GetObjectGuid();
                player->GetMap()->GetMessager().AddMessage([guid, spellId](Map* map) -> void
                {
                    if(Player* player = map->GetPlayer(guid))
                        player->CastSpell(player, spellId, TRIGGERED_OLD_TRIGGERED);
                });
            }
        }
    }
}

uint32 OutdoorPvP::GetBannerArtKit(Team team, uint32 artKitAlliance /*= CAPTURE_ARTKIT_ALLIANCE*/, uint32 artKitHorde /*= CAPTURE_ARTKIT_HORDE*/, uint32 artKitNeutral /*= CAPTURE_ARTKIT_NEUTRAL*/) const
{
    switch (team)
    {
        case ALLIANCE:
            return artKitAlliance;
        case HORDE:
            return artKitHorde;
        default:
            return artKitNeutral;
    }
}

void OutdoorPvP::SetBannerVisual(const WorldObject* objRef, ObjectGuid goGuid, uint32 artKit, uint32 animId)
{
    if (GameObject* go = objRef->GetMap()->GetGameObject(goGuid))
        SetBannerVisual(go, artKit, animId);
}

void OutdoorPvP::SetBannerVisual(GameObject* go, uint32 artKit, uint32 animId)
{
    go->SendGameObjectCustomAnim(go->GetObjectGuid(), animId);
    go->SetGoArtKit(artKit);
    go->Refresh();
}

void OutdoorPvP::RespawnGO(const WorldObject* objRef, ObjectGuid goGuid, bool respawn)
{
    if (GameObject* go = objRef->GetMap()->GetGameObject(goGuid))
    {
        go->SetRespawnTime(7 * DAY);

        if (respawn)
            go->Refresh();
        else if (go->IsSpawned())
            go->ForcedDespawn();
    }
}
