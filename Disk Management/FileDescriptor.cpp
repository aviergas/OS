#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <unistd.h>

#include "FileDescriptor.h"

FileDescriptor::FileDescriptor(string FileName, FsFile *fsi) {
    file_name = FileName;
    fs_file = fsi;
    inUse = true ;
}

string FileDescriptor::getFileName() {
    return file_name;
}

bool FileDescriptor::isInUse() const {
    return inUse;
}

FsFile *FileDescriptor::getFsFile() const {
    return fs_file;
}

void FileDescriptor::setFileName(const string &fileName) {
    file_name = fileName;
}

void FileDescriptor::setFsFile(FsFile *fsFile) {
    fs_file = fsFile;
}

void FileDescriptor::setInUse(bool inUse) {
    FileDescriptor::inUse = inUse;
}

