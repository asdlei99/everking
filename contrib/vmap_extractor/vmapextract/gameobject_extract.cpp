#include "model.h"
#include "dbcfile.h"
#include "wmo.h"
#include "vmapexport.h"

#include <algorithm>
#include <stdio.h>

bool ExtractSingleModel(std::string& fname, StringSet& failedPaths)
{
    if (fname.length() < 4)
        return false;

    std::string extension = fname.substr(fname.length() - 4, 4);
    if (extension == ".mdx" || extension == ".MDX" || extension == ".mdl" || extension == ".MDL")
    {
        // replace .mdx -> .m2
        fname.erase(fname.length() - 2, 2);
        fname.append("2");
    }
    // >= 3.1.0 ADT MMDX section store filename.m2 filenames for corresponded .m2 file
    // nothing do

    std::string originalName = fname;

    char* name = GetPlainName((char*)fname.c_str());
    fixnamen(name, strlen(name));
    fixname2(name, strlen(name));

    std::string output(szWorkDirWmo);
    output += "/";
    output += name;

    if (FileExists(output.c_str()))
        return true;

    Model mdl(originalName);
    if (!mdl.open(failedPaths))
        return false;

    return mdl.ConvertToVMAPModel(output.c_str());
}

void ExtractGameobjectModels()
{
    printf("\n");
    printf("Extracting GameObject models...\n");
    DBCFile dbc("./dbc/GameObjectDisplayInfo.dbc");
    if (!dbc.open())
    {
        printf("Fatal error: Invalid GameObjectDisplayInfo.dbc file format!\n");
        exit(1);
    }

    std::string basepath = szWorkDirWmo;
    basepath += "/";
    std::string path;
    StringSet failedPaths;

    FILE* model_list = fopen((basepath + "temp_gameobject_models").c_str(), "wb");

    for (DBCFile::Iterator it = dbc.begin(); it != dbc.end(); ++it)
    {
        path = it->getString(1);

        if (path.length() < 4)
            continue;

        fixnamen((char*)path.c_str(), path.size());
        char* name = GetPlainName((char*)path.c_str());
        fixname2(name, strlen(name));

        char const* ch_ext = GetExtension(name);
        if (!ch_ext)
            continue;

        //strToLower(ch_ext);

        bool result = false;
        if (!strcmp(ch_ext, ".wmo"))
        {
            result = ExtractSingleWmo(path);
        }
        else if (!strcmp(ch_ext, ".mdl"))
        {
            // TODO: extract .mdl files, if needed
            continue;
        }
        else //if (!strcmp(ch_ext, ".mdx") || !strcmp(ch_ext, ".m2"))
        {
            result = ExtractSingleModel(path, failedPaths);
        }

        if (result)
        {
            uint32 displayId = it->getUInt(0);
            uint32 path_length = strlen(name);
            fwrite(&displayId, sizeof(uint32), 1, model_list);
            fwrite(&path_length, sizeof(uint32), 1, model_list);
            fwrite(name, sizeof(char), path_length, model_list);
        }
    }

    fclose(model_list);

    if (!failedPaths.empty())
    {
        printf("Warning: Some models could not be extracted, see below\n");
        for (StringSet::const_iterator itr = failedPaths.begin(); itr != failedPaths.end(); ++itr)
            printf("Could not find file of model %s\n", itr->c_str());
        printf("A few of these warnings are expected to happen, so be not alarmed!\n");
    }

    printf("Done!\n");
}
