#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <unistd.h>

using namespace std;

#include "fsFile.h"

#ifndef EX6_FILEDESCRIPTOR_H
#define EX6_FILEDESCRIPTOR_H

class FileDescriptor{

    string file_name ;
    FsFile* fs_file ;
    bool inUse;//True when its open and false when its close .

    public:
    FileDescriptor(string FileName , FsFile* fsi);

    string getFileName ();

    bool isInUse() const;

    FsFile *getFsFile() const;

    void setFileName(const string &fileName);

    void setFsFile(FsFile *fsFile);

    void setInUse(bool inUse);


};

#endif //EX6_FILEDESCRIPTOR_H
