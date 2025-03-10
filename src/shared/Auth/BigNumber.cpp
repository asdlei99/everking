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

#include "Auth/BigNumber.h"
#include <openssl/bn.h>
#include <algorithm>

BigNumber::BigNumber()
{
    _bn = BN_new();
    _array = nullptr;
}

BigNumber::BigNumber(const BigNumber& bn)
{
    _bn = BN_dup(bn._bn);
    _array = nullptr;
}

BigNumber::BigNumber(uint32 val)
{
    _bn = BN_new();
    BN_set_word(_bn, val);
    _array = nullptr;
}

BigNumber::~BigNumber()
{
    BN_free(_bn);
    if (_array) delete[] _array;
}

void BigNumber::SetDword(uint32 val)
{
    BN_set_word(_bn, val);
}

void BigNumber::SetQword(uint64 val)
{
    BN_add_word(_bn, (uint32)(val >> 32));
    BN_lshift(_bn, _bn, 32);
    BN_add_word(_bn, (uint32)(val & 0xFFFFFFFF));
}

void BigNumber::SetBinary(const uint8* bytes, int len)
{
    uint8 t[1000];
    for (int i = 0; i < len; ++i)
        t[i] = bytes[len - 1 - i];
    BN_bin2bn(t, len, _bn);
}

int BigNumber::SetHexStr(const char* str)
{
    return BN_hex2bn(&_bn, str);
}

void BigNumber::SetRand(int numbits)
{
    BN_rand(_bn, numbits, 0, 1);
}

BigNumber& BigNumber::operator=(const BigNumber& bn)
{
    BN_copy(_bn, bn._bn);
    return *this;
}

BigNumber& BigNumber::operator+=(const BigNumber& bn)
{
    BN_add(_bn, _bn, bn._bn);
    return *this;
}

BigNumber& BigNumber::operator-=(const BigNumber& bn)
{
    BN_sub(_bn, _bn, bn._bn);
    return *this;
}

BigNumber& BigNumber::operator*=(const BigNumber& bn)
{
    BN_CTX* bnctx = BN_CTX_new();
    BN_mul(_bn, _bn, bn._bn, bnctx);
    BN_CTX_free(bnctx);

    return *this;
}

BigNumber& BigNumber::operator/=(const BigNumber& bn)
{
    BN_CTX* bnctx = BN_CTX_new();
    BN_div(_bn, nullptr, _bn, bn._bn, bnctx);
    BN_CTX_free(bnctx);

    return *this;
}

BigNumber& BigNumber::operator%=(const BigNumber& bn)
{
    BN_CTX* bnctx = BN_CTX_new();
    BN_mod(_bn, _bn, bn._bn, bnctx);
    BN_CTX_free(bnctx);

    return *this;
}

BigNumber BigNumber::Exp(const BigNumber& bn)
{
    BigNumber ret;

    BN_CTX* bnctx = BN_CTX_new();
    BN_exp(ret._bn, _bn, bn._bn, bnctx);
    BN_CTX_free(bnctx);

    return ret;
}

BigNumber BigNumber::ModExp(const BigNumber& bn1, const BigNumber& bn2)
{
    BigNumber ret;

    BN_CTX* bnctx = BN_CTX_new();
    BN_mod_exp(ret._bn, _bn, bn1._bn, bn2._bn, bnctx);
    BN_CTX_free(bnctx);

    return ret;
}

int BigNumber::GetNumBytes(void) const
{
    return BN_num_bytes(_bn);
}

uint32 BigNumber::AsDword() const
{
    return (uint32)BN_get_word(_bn);
}

bool BigNumber::isZero() const
{
    return BN_is_zero(_bn) != 0;
}

std::vector<uint8> BigNumber::AsByteArray(int minSize, bool reverse) const
{
    int length = (minSize >= GetNumBytes()) ? minSize : GetNumBytes();

    std::vector<uint8> byteArray(length);

    // If we need more bytes than length of BigNumber set the rest to 0
    if (length > GetNumBytes())
        memset((void*)byteArray.data(), 0, length);

    // Padding should add leading zeroes, not trailing
    auto const paddingOffset = length - GetNumBytes();

    BN_bn2bin(_bn, (unsigned char*)byteArray.data() + paddingOffset);

    if (reverse)
        std::reverse(byteArray.begin(), byteArray.end());

    return byteArray;
}

const char* BigNumber::AsHexStr() const
{
    return BN_bn2hex(_bn);
}

const char* BigNumber::AsDecStr() const
{
    return BN_bn2dec(_bn);
}
