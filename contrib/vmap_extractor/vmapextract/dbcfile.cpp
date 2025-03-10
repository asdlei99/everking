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

#include "dbcfile.h"
#include "mpq_libmpq04.h"
#undef min
#undef max

#include <cstdio>

DBCFile::DBCFile(const std::string& filename) : filename(filename)
{
    data = NULL;
}

bool DBCFile::open()
{
    printf("000000\n");
    MPQFile f(filename.c_str());
    printf("333333\n");
    // Need some error checking, otherwise an unhandled exception error occurs
    // if people screw with the data path.
    if (f.isEof() == true){
        printf("DBCFile::open Error: f.isEof");
        return false;
    }

    printf("1111111111111\n");
    unsigned char header[4];
    unsigned int na, nb, es, ss;

    f.read(header, 4); // File Header

    if (header[0] != 'W' || header[1] != 'D' || header[2] != 'B' || header[3] != 'C')
    {
        f.close();
        data = NULL;
        printf("Critical Error: An error occured while trying to read the DBCFile %s.", filename.c_str());
        return false;
    }

    //assert(header[0]=='W' && header[1]=='D' && header[2]=='B' && header[3] == 'C');

    f.read(&na, 4); // Number of records
    f.read(&nb, 4); // Number of fields
    f.read(&es, 4); // Size of a record
    f.read(&ss, 4); // String size

    recordSize = es;
    recordCount = na;
    fieldCount = nb;
    stringSize = ss;
    //assert(fieldCount*4 == recordSize);
    assert(fieldCount * 4 >= recordSize);

    data = new unsigned char[recordSize * recordCount + stringSize];
    stringTable = data + recordSize * recordCount;
    f.read(data, recordSize * recordCount + stringSize);
    f.close();
    return true;
}

DBCFile::~DBCFile()
{
    delete [] data;
}

DBCFile::Record DBCFile::getRecord(size_t id)
{
    assert(data);
    return Record(*this, data + id * recordSize);
}

DBCFile::Iterator DBCFile::begin()
{
    assert(data);
    return Iterator(*this, data);
}

DBCFile::Iterator DBCFile::end()
{
    assert(data);
    return Iterator(*this, stringTable);
}
