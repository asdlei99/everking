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

#ifndef MANGOS_THREADINGMODEL_H
#define MANGOS_THREADINGMODEL_H

/**
 * @class ThreadingModel<T>
 *
 */

#include "Platform/Define.h"

namespace MaNGOS
{
    template<typename MUTEX>
    class GeneralLock
    {
        public:

            GeneralLock(MUTEX& m)
                : i_mutex(m)
            {
                i_mutex.lock();
            }

            ~GeneralLock()
            {
                i_mutex.unlock();
            }

        private:

            GeneralLock(const GeneralLock&);
            GeneralLock& operator=(const GeneralLock&);
            MUTEX& i_mutex;
    };

    template<class T>
    class SingleThreaded
    {
        public:

            struct Lock                                     // empty object
            {
                Lock()
                {
                }
                Lock(const T&)
                {
                }

                Lock(const SingleThreaded<T>&)              // for single threaded we ignore this
                {
                }
            };
    };

    template<class T, class MUTEX>
    class ObjectLevelLockable
    {
        public:

            ObjectLevelLockable()
                : i_mtx()
            {
            }

            friend class Lock;

            class Lock
            {
                public:

                    Lock(ObjectLevelLockable<T, MUTEX>& host)
                        : i_lock(host.i_mtx)
                    {
                    }

                private:

                    GeneralLock<MUTEX> i_lock;
            };

        private:

            // prevent the compiler creating a copy construct
            ObjectLevelLockable(const ObjectLevelLockable<T, MUTEX>&);
            ObjectLevelLockable<T, MUTEX>& operator=(const ObjectLevelLockable<T, MUTEX>&);

            MUTEX i_mtx;
    };

    template<class T, class MUTEX>
    class ClassLevelLockable
    {
        public:

            ClassLevelLockable()
            {
            }

            friend class Lock;

            class Lock
            {
                public:

                    Lock(const T& /*host*/)
                    {
                        ClassLevelLockable<T, MUTEX>::si_mtx.lock();
                    }

                    Lock(const ClassLevelLockable<T, MUTEX>&)
                    {
                        ClassLevelLockable<T, MUTEX>::si_mtx.lock();
                    }

                    Lock()
                    {
                        ClassLevelLockable<T, MUTEX>::si_mtx.lock();
                    }

                    ~Lock()
                    {
                        ClassLevelLockable<T, MUTEX>::si_mtx.unlock();
                    }
            };

        private:

            static MUTEX si_mtx;
    };
}

template<class T, class MUTEX> MUTEX MaNGOS::ClassLevelLockable<T, MUTEX>::si_mtx;

#define INSTANTIATE_CLASS_MUTEX(CTYPE, MUTEX) \
    template class MaNGOS::ClassLevelLockable<CTYPE, MUTEX>

#endif
