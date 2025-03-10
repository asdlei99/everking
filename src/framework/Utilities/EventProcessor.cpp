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

#include "EventProcessor.h"

EventProcessor::EventProcessor()
{
    m_time = 0;
    m_aborting = false;
}

EventProcessor::~EventProcessor()
{
    KillAllEvents(true);
}

void EventProcessor::Update(uint32 p_time)
{
    // update time
    m_time += p_time;

    // main event loop
    EventList::iterator i;
    while (((i = m_events.begin()) != m_events.end()) && i->first <= m_time)
    {
        // get and remove event from queue
        BasicEvent* Event = i->second;
        m_events.erase(i);

        if (!Event->to_Abort)
        {
            if (Event->Execute(m_time, p_time))
            {
                // completely destroy event if it is not re-added
                delete Event;
            }
        }
        else
        {
            Event->Abort(m_time);
            delete Event;
        }
    }
}

void EventProcessor::KillAllEvents(bool force)
{
    // prevent event insertions
    m_aborting = true;

    // first, abort all existing events
    for (EventList::iterator i = m_events.begin(); i != m_events.end();)
    {
        EventList::iterator i_old = i;
        ++i;

        i_old->second->to_Abort = true;
        i_old->second->Abort(m_time);
        if (force || i_old->second->IsDeletable())
        {
            delete i_old->second;

            if (!force)                                     // need per-element cleanup
                m_events.erase(i_old);
        }
    }

    // fast clear event list (in force case)
    if (force)
        m_events.clear();
}

void EventProcessor::KillEvent(BasicEvent* event)
{
    for (EventList::iterator iter = m_events.begin(); iter != m_events.end();)
    {
        if (iter->second == event)
        {
            delete iter->second;
            iter = m_events.erase(iter);
        }
        else ++iter;
    }
}

void EventProcessor::AddEvent(BasicEvent* Event, uint64 e_time, bool set_addtime)
{
    if (set_addtime)
        Event->m_addTime = m_time;

    Event->m_execTime = e_time;
    m_events.insert(std::pair<uint64, BasicEvent*>(e_time, Event));
}

void EventProcessor::ModifyEventTime(BasicEvent* Event, uint64 msTime)
{
    for (auto itr = m_events.begin(); itr != m_events.end(); ++itr)
    {
        if (itr->second != Event)
            continue;

        Event->m_execTime = msTime;
        m_events.erase(itr);
        m_events.insert(std::pair<uint64, BasicEvent*>(msTime, Event));
        break;
    }
}

uint64 EventProcessor::CalculateTime(uint64 t_offset) const
{
    return m_time + t_offset;
}
