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

#ifndef MANGOS_TYPECONTAINERVISITOR_H
#define MANGOS_TYPECONTAINERVISITOR_H

/*
 * @class TypeContainerVisitor is implemented as a visitor pattern.  It is
 * a visitor to the TypeMapContainer or ContainerMapList.  The visitor has
 * to overload its types as a visit method is called.
 */

#include "Platform/Define.h"
#include "TypeContainer.h"

// forward declaration
template<class T, class Y> class TypeContainerVisitor;

// visitor helper
template<class VISITOR, class TYPE_CONTAINER>
void VisitorHelper(VISITOR& v, TYPE_CONTAINER& c)
{
    v.Visit(c);
}

// terminate condition container map list
template<class VISITOR>
void VisitorHelper(VISITOR& /*v*/, ContainerMapList<TypeNull>& /*c*/)
{
}

template<class VISITOR, class T>
void VisitorHelper(VISITOR& v, ContainerMapList<T>& c)
{
    v.Visit(c._element);
}

// recursion container map list
template<class VISITOR, class H, class T>
void VisitorHelper(VISITOR& v, ContainerMapList<TypeList<H, T> >& c)
{
    VisitorHelper(v, c._elements);
    VisitorHelper(v, c._TailElements);
}

// for TypeMapContainer
template<class VISITOR, class OBJECT_TYPES>
void VisitorHelper(VISITOR& v, TypeMapContainer<OBJECT_TYPES>& c)
{
    VisitorHelper(v, c.GetElements());
}

template<class VISITOR, class TYPE_CONTAINER>
class TypeContainerVisitor
{
    public:

        TypeContainerVisitor(VISITOR& v)
            : i_visitor(v)
        {
        }

        void Visit(TYPE_CONTAINER& c)
        {
            VisitorHelper(i_visitor, c);
        }

        void Visit(const TYPE_CONTAINER& c) const
        {
            VisitorHelper(i_visitor, c);
        }

    private:

        VISITOR& i_visitor;
};

#endif
