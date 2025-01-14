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

#ifndef _ACCMGR_H
#define _ACCMGR_H

#include "Common.h"

enum AccountOpResult
{
    AOR_OK,
    AOR_NAME_TOO_LONG,
    AOR_PASS_TOO_LONG,
    AOR_NAME_ALREADY_EXIST,
    AOR_NAME_NOT_EXIST,
    AOR_DB_INTERNAL_ERROR
};

#define MAX_ACCOUNT_STR 16

class AccountMgr
{
    public:
        AccountMgr();
        ~AccountMgr();

        AccountOpResult CreateAccount(std::string username, std::string password) const;
        AccountOpResult CreateAccount(std::string username, std::string password, uint32 expansion) const;
        AccountOpResult DeleteAccount(uint32 accid) const;
        AccountOpResult ChangeUsername(uint32 accid, std::string new_uname, std::string new_passwd) const;
        AccountOpResult ChangePassword(uint32 accid, std::string new_passwd) const;
        bool CheckPassword(uint32 accid, std::string passwd) const;

        uint32 GetId(std::string username) const;
        static AccountTypes GetSecurity(uint32 acc_id);
        bool GetName(uint32 acc_id, std::string& name) const;
        uint32 GetCharactersCount(uint32 acc_id) const;
        std::string CalculateShaPassHash(std::string& name, std::string& password) const;
        uint32 GetFlags(uint32 acc_id) const;

        static bool normalizeString(std::string& utf8str);
};

#define sAccountMgr MaNGOS::Singleton<AccountMgr>::Instance()
#endif
