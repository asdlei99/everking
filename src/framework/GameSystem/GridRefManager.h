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

#ifndef _GRIDREFMANAGER
#define _GRIDREFMANAGER

#include "Utilities/LinkedReference/RefManager.h"

template<class OBJECT> class GridReference;

template<class OBJECT>
class GridRefManager : public RefManager<GridRefManager<OBJECT>, OBJECT>
{
    public:

        typedef LinkedListHead::Iterator< GridReference<OBJECT> > iterator;

        GridReference<OBJECT>* getFirst()
        {
            return (GridReference<OBJECT>*)RefManager<GridRefManager<OBJECT>, OBJECT>::getFirst();
        }

        GridReference<OBJECT>* getLast()
        {
            return (GridReference<OBJECT>*)RefManager<GridRefManager<OBJECT>, OBJECT>::getLast();
        }

        iterator begin() { return iterator(getFirst()); }
        iterator end() { return iterator(nullptr); }
        iterator rbegin() { return iterator(getLast()); }
        iterator rend() { return iterator(nullptr); }
};
#endif
