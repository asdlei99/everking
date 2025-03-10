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

#ifndef MANGOS_MOVEMENTGENERATOR_H
#define MANGOS_MOVEMENTGENERATOR_H

#include "Platform/Define.h"
#include "Dynamic/FactoryHolder.h"
#include "MotionGenerators/MotionMaster.h"
#include "Util/Timer.h"
#include "Globals/SharedDefines.h"

class Unit;
class Creature;
class Player;

class MovementGenerator
{
    public:
        virtual ~MovementGenerator();

        // called before adding movement generator to motion stack
        virtual void Initialize(Unit&) = 0;
        // called aftre remove movement generator from motion stack
        virtual void Finalize(Unit&) = 0;

        // called before lost top position (before push new movement generator above)
        virtual void Interrupt(Unit&) = 0;
        // called after return movement generator to top position (after remove above movement generator)
        virtual void Reset(Unit&) = 0;

        virtual bool Update(Unit&, const uint32& time_diff) = 0;

        virtual MovementGeneratorType GetMovementGeneratorType() const = 0;
        virtual Unit* GetCurrentTarget() const { return nullptr; }

        virtual void UnitSpeedChanged() { }

        // used by Evade code for select point to evade with expected restart default movement
        virtual bool GetResetPosition(Unit&, float& /*x*/, float& /*y*/, float& /*z*/, float& /*o*/) const { return false; }

        // given destination unreachable? due to pathfinsing or other
        virtual bool IsReachable() const { return true; }

        virtual bool IsRemovedOnExpire() const { return false; }

        // used for check from Update call is movegen still be active (top movement generator)
        // after some not safe for this calls
        bool IsActive(Unit& u);
};

template<class T, class D>
class MovementGeneratorMedium : public MovementGenerator
{
    public:
        void Initialize(Unit& u) override
        {
            // u->AssertIsType<T>();
            (static_cast<D*>(this))->Initialize(*((T*)&u));
        }
        void Finalize(Unit& u) override
        {
            // u->AssertIsType<T>();
            (static_cast<D*>(this))->Finalize(*((T*)&u));
        }
        void Interrupt(Unit& u) override
        {
            // u->AssertIsType<T>();
            (static_cast<D*>(this))->Interrupt(*((T*)&u));
        }
        void Reset(Unit& u) override
        {
            // u->AssertIsType<T>();
            (static_cast<D*>(this))->Reset(*((T*)&u));
        }
        bool Update(Unit& u, const uint32& time_diff) override
        {
            // u->AssertIsType<T>();
            return (static_cast<D*>(this))->Update(*((T*)&u), time_diff);
        }
        bool GetResetPosition(Unit& u, float& x, float& y, float& z, float& o) const override
        {
            // u->AssertIsType<T>();
            return (static_cast<D const*>(this))->GetResetPosition(*((T*)&u), x, y, z, o);
        }
    public:
        // Will not link if not overridden in the generators - also not generate for T==Unit
        template <class U = T, typename std::enable_if<false == std::is_same<U, Unit>::value, U>::type* = nullptr>
        void Initialize(U& u);
        template <class U = T, typename std::enable_if<false == std::is_same<U, Unit>::value, U>::type* = nullptr>
        void Finalize(U& u);
        template <class U = T, typename std::enable_if<false == std::is_same<U, Unit>::value, U>::type* = nullptr>
        void Interrupt(U& u);
        template <class U = T, typename std::enable_if<false == std::is_same<U, Unit>::value, U>::type* = nullptr>
        void Reset(U& u);
        template <class U = T, typename std::enable_if<false == std::is_same<U, Unit>::value, U>::type* = nullptr>
        bool Update(U& u, const uint32& time_diff);

        // not need always overwrites
        template <class U = T, typename std::enable_if<false == std::is_same<U, Unit>::value, U>::type* = nullptr>
        bool GetResetPosition(U& /*u*/, float& /*x*/, float& /*y*/, float& /*z*/, float& /*o*/) const { return false; }
};

struct SelectableMovement : public FactoryHolder<MovementGenerator, MovementGeneratorType>
{
    SelectableMovement(MovementGeneratorType mgt) : FactoryHolder<MovementGenerator, MovementGeneratorType>(mgt) {}
};

template<class REAL_MOVEMENT>
struct MovementGeneratorFactory : public SelectableMovement
{
    MovementGeneratorFactory(MovementGeneratorType mgt) : SelectableMovement(mgt) {}

    MovementGenerator* Create(void*) const override;
};

typedef FactoryHolder<MovementGenerator, MovementGeneratorType> MovementGeneratorCreator;
typedef FactoryHolder<MovementGenerator, MovementGeneratorType>::FactoryHolderRegistry MovementGeneratorRegistry;
typedef FactoryHolder<MovementGenerator, MovementGeneratorType>::FactoryHolderRepository MovementGeneratorRepository;

#endif
