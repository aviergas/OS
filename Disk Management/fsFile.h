#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>

#ifndef EX6_FSFILE_H
#define EX6_FSFILE_H

class FsFile{
    int file_size;
    int block_in_use;
    int index_block;
    int block_size;


public:
    FsFile(int _block_size);

    ~FsFile();

    int getfile_size();

    int getFileSize() const;

    void setFileSize(int fileSize);

    int getBlockInUse() const;

    void setBlockInUse(int blockInUse);

    int getIndexBlock() const;

    void setIndexBlock(int indexBlock);

    int getBlockSize() const;

    void setBlockSize(int blockSize);

};

#endif //EX6_FSFILE_H
