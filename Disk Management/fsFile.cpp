#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <unistd.h>

#include "fsFile.h"

FsFile::FsFile(int _block_size) {
    file_size = 0 ;
    block_in_use = 0 ;
    block_size = _block_size;
    index_block = -1;
}

FsFile::~FsFile() {

}

int FsFile::getfile_size() {
    return file_size;
}

int FsFile::getFileSize() const {
    return file_size;
}

void FsFile::setFileSize(int fileSize) {
    file_size = fileSize;
}

int FsFile::getBlockInUse() const {
    return block_in_use;
}

void FsFile::setBlockInUse(int blockInUse) {
    block_in_use = blockInUse;
}

int FsFile::getIndexBlock() const {
    return index_block;
}

void FsFile::setIndexBlock(int indexBlock) {
    index_block = indexBlock;
}

int FsFile::getBlockSize() const {
    return block_size;
}

void FsFile::setBlockSize(int blockSize) {
    block_size = blockSize;
}


