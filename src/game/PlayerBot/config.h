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

#ifndef PB_CONFIG_H
#define PB_CONFIG_H

#include "Platform/CompilerDefs.h"

// Format is YYYYMMDDRR where RR is the change in the conf file
// for that day.
#define PLAYERBOT_CONF_VERSION    2018101401

#if PLATFORM == PLATFORM_WINDOWS
#define _D_PLAYERBOT_CONFIG "playerbot.conf"
#else
#define _D_PLAYERBOT_CONFIG  SYSCONFDIR "playerbot.conf"
#endif
#define _PLAYERBOT_CONFIG  MaNGOS::Singleton<std::string>::Instance()

#endif
