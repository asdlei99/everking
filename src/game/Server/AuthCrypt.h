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

#ifndef _AUTHCRYPT_H
#define _AUTHCRYPT_H

#include <Common.h>
#include "Auth/SARC4.h"

class BigNumber;

class AuthCrypt
{
    public:
        AuthCrypt();

        void Init(BigNumber* K);
        void DecryptRecv(uint8*, size_t);
        void EncryptSend(uint8*, size_t);

    private:
        SARC4 _clientDecrypt;
        SARC4 _serverEncrypt;
        bool _initialized;
};
#endif
