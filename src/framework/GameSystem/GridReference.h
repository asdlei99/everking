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

#ifndef _GRIDREFERENCE_H
#define _GRIDREFERENCE_H

#include "Utilities/LinkedReference/Reference.h"

template<class OBJECT> class GridRefManager;

template<class OBJECT>
class GridReference : public Reference<GridRefManager<OBJECT>, OBJECT>
{
    protected:

        void targetObjectBuildLink() override
        {
            // called from link()
            this->getTarget()->insertFirst(this);
            this->getTarget()->incSize();
        }

        void targetObjectDestroyLink() override
        {
            // called from unlink()
            if (this->isValid())
                this->getTarget()->decSize();
        }

        void sourceObjectDestroyLink() override
        {
            // called from invalidate()
            this->getTarget()->decSize();
        }

    public:

        GridReference()
            : Reference<GridRefManager<OBJECT>, OBJECT>()
        {
        }

        ~GridReference()
        {
            this->unlink();
        }

        GridReference* next()
        {
            return (GridReference*)Reference<GridRefManager<OBJECT>, OBJECT>::next();
        }
};

#endif
