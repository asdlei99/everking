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

#ifndef _WORLDMODEL_H
#define _WORLDMODEL_H

#include <G3D/HashTrait.h>
#include <G3D/Vector3.h>
#include <G3D/AABox.h>
#include <G3D/Ray.h>
#include "BIH.h"

#include "Platform/Define.h"

namespace VMAP
{
    class TreeNode;
    struct AreaInfo;
    struct GroupLocationInfo;
    struct LocationInfo;

    class MeshTriangle
    {
        public:
            MeshTriangle() : idx0(0), idx1(0), idx2(0) {}
            MeshTriangle(uint32 na, uint32 nb, uint32 nc): idx0(na), idx1(nb), idx2(nc) {}

            uint32 idx0;
            uint32 idx1;
            uint32 idx2;
    };

    class WmoLiquid
    {
        public:
            WmoLiquid(uint32 width, uint32 height, const Vector3& corner, uint32 type);
            WmoLiquid(const WmoLiquid& other);
            ~WmoLiquid();
            WmoLiquid& operator=(const WmoLiquid& other);
            bool GetLiquidHeight(const Vector3& pos, float& liqHeight) const;
            uint32 GetType() const { return iType; }
            float* GetHeightStorage() const { return iHeight; }
            uint8* GetFlagsStorage() const { return iFlags; }
            uint32 GetFileSize() const;
            bool writeToFile(FILE* wf);
            static bool readFromFile(FILE* rf, WmoLiquid*& out);
        private:
            WmoLiquid() : iTilesX(0), iTilesY(0), iType(0), iHeight(nullptr), iFlags(nullptr) {};
            uint32 iTilesX;  //!< number of tiles in x direction, each
            uint32 iTilesY;
            Vector3 iCorner; //!< the lower corner
            uint32 iType;    //!< liquid type
            float* iHeight;  //!< (tilesX + 1)*(tilesY + 1) height values
            uint8* iFlags;   //!< info if liquid tile is used
#ifdef MMAP_GENERATOR
        public:
            void getPosInfo(uint32& tilesX, uint32& tilesY, Vector3& corner) const;
#endif
    };

    /*! holding additional info for WMO group files */
    class GroupModel
    {
        public:
            GroupModel() : iMogpFlags(0), iGroupWMOID(0), iLiquid(nullptr) {}
            GroupModel(GroupModel const& other);
            GroupModel(uint32 mogpFlags, uint32 groupWMOID, AABox const& bound) :
                iBound(bound), iMogpFlags(mogpFlags), iGroupWMOID(groupWMOID), iLiquid(nullptr) {}
            ~GroupModel() { delete iLiquid; }

            //! pass mesh data to object and create BIH. Passed vectors get get swapped with old geometry!
            void setMeshData(std::vector<Vector3>& vert, std::vector<MeshTriangle>& tri);
            void setLiquidData(WmoLiquid*& liquid) { iLiquid = liquid; liquid = nullptr; }
            bool IntersectRay(const G3D::Ray& ray, float& distance, bool stopAtFirstHit, bool ignoreM2Model = false) const;
            bool IsInsideObject(const Vector3& pos, const Vector3& down, float& z_dist) const;
            bool GetLiquidLevel(const Vector3& pos, float& liqHeight) const;
            uint32 GetLiquidType() const;
            bool writeToFile(FILE* wf);
            bool readFromFile(FILE* rf);
            const G3D::AABox& GetBound() const { return iBound; }
            uint32 GetMogpFlags() const { return iMogpFlags; }
            uint32 GetWmoID() const { return iGroupWMOID; }
        protected:
            G3D::AABox iBound;
            uint32 iMogpFlags;// 0x8 outdor; 0x2000 indoor
            uint32 iGroupWMOID;
            std::vector<Vector3> vertices;
            std::vector<MeshTriangle> triangles;
            BIH meshTree;
            WmoLiquid* iLiquid;

#ifdef MMAP_GENERATOR
        public:
            void getMeshData(std::vector<Vector3>& outVertices, std::vector<MeshTriangle>& outTriangles, WmoLiquid*& liquid);
#endif
    };
    /*! Holds a model (converted M2 or WMO) in its original coordinate space */
    class WorldModel
    {
        public:
            WorldModel(): RootWMOID(0), modelFlags(0) {}

            //! pass group models to WorldModel and create BIH. Passed vector is swapped with old geometry!
            void setGroupModels(std::vector<GroupModel>& models);
            void setRootWmoID(uint32 id) { RootWMOID = id; }
            bool IntersectRay(const G3D::Ray& ray, float& distance, bool stopAtFirstHit, bool ignoreM2Model = false) const;
            bool IntersectPoint(const G3D::Vector3& p, const G3D::Vector3& down, float& dist, AreaInfo& info) const;
            bool GetLocationInfo(const G3D::Vector3& p, const G3D::Vector3& down, float& dist, GroupLocationInfo& info) const;
            bool writeFile(const std::string& filename);
            bool readFile(const std::string& filename);
            void setModelFlags(uint32 newFlags) { modelFlags = newFlags; }
            uint32 getModelFlags() const { return modelFlags; }
        protected:
            uint32 RootWMOID;
            std::vector<GroupModel> groupModels;
            BIH groupTree;
            uint32 modelFlags;

#ifdef MMAP_GENERATOR
        public:
            void getGroupModels(std::vector<GroupModel>& outGroupModels);
#endif
    };
} // namespace VMAP

#endif // _WORLDMODEL_H
