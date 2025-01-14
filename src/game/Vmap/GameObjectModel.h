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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef MANGOSSERVER_GAMEOBJECTMODEL_H
#define MANGOSSERVER_GAMEOBJECTMODEL_H

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>
#include <G3D/AABox.h>
#include <G3D/Ray.h>
#include "Server/DBCStructure.h"
#include "Entities//GameObject.h"

#include "Platform/Define.h"

namespace VMAP
{
    class WorldModel;
}

class GameObjectModel
{
        uint32 phasemask;
        G3D::AABox iBound;
        G3D::Matrix3 iInvRot;
        G3D::Vector3 iPos;
        //G3D::Vector3 iRot;
        float iInvScale;
        float iScale;
        VMAP::WorldModel* iModel;

        GameObjectModel() : phasemask(0), iInvScale(0), iScale(0), iModel(nullptr) {}
        bool initialize(const GameObject* const pGo, const GameObjectDisplayInfoEntry* pDisplayInfo);

    public:
        std::string name;

        const G3D::AABox& getBounds() const { return iBound; }

        ~GameObjectModel();

        const G3D::Vector3& getPosition() const { return iPos;}

        /** Enables\disables collision. */
        void disable() { phasemask = 0;}
        void enable(uint32 ph_mask) { phasemask = ph_mask;}

        bool intersectRay(const G3D::Ray& ray, float& MaxDist, bool StopAtFirstHit, uint32 phaseMask, bool ignoreM2Model) const;

        static GameObjectModel* construct(const GameObject* const pGo);

        bool Relocate(GameObject const& go);

        static void LoadGOVmapModels();
        static std::vector<std::pair<uint32, uint32>> GetTilesForGOEntry(uint32 mapId, uint32 goEntry);
        static std::pair<bool, uint32> GetTileDataForGoDisplayId(uint32 mapId, uint32 goEntry, uint32 displayId, uint32 tileX, uint32 tileY);
};
#endif
