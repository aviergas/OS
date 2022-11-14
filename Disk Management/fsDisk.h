#include <iostream>
#include <vector>
#include <map>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DISK_SIZE 256
#define DISK_SIM_FILE "DISK_SIM_FILE.txt"
using namespace std;

#include "fsFile.h"
#include "FileDescriptor.h"

#ifndef EX6_FSDISK_H
#define EX6_FSDISK_H


class fsDisk{
    FILE* sim_disk_fd;
    bool is_formated;
    int BitVectorSize;
    int *BitVector;
    vector<FileDescriptor*> MainDir;
    vector<FileDescriptor*> OpenFileDescriptors;
    int blockSizeFromFormat; // I DONT KNOW IF WE ALLOW TO INSERT MORE INTS ..

public:
    fsDisk();
    ~fsDisk();

    void listAll();
    void decToBinary(int n, char &c);
    void fsFormat(int blockSize=4);
    int CreateFile(string fileName);
    int OpenFile(string fileName);
    string CloseFile(int fd);
    int WriteToFile(int fd, char *buf, int len );
    int DelFile( string FileName );
    int ReadFromFile(int fd, char *buf, int len );

    int insertToFreeSpace(int fd ,int offset , char *buf);
    int checkIfIndexBlockIsFull(int fd);
    int checkIfTheDiskIsFull();
    void printBitVector ();
    int findSpaceInBitVector();
};

#endif //EX6_FSDISK_H
