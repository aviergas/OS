DISK_SIM_FILE , Ex6
Authored by Avi Ergas
208783209

==Description==

The function of the system is basically to simulate the arrangement and routing of the files on the disk and actually manage the disk. First we will format the disk and then we will perform defined operations such as creating a file, closing and opening a file, writing and reading from a file and of course deleting.
The file named disk_sim_file simulates the disk and is a text file to which we will write.
We have data structures that the system uses such as vectors and arrays.

functions:

FSFILE: 

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

 FILEDESCRIPTOR: 

   FileDescriptor(string FileName , FsFile* fsi);

    string getFileName ();

    bool isInUse() const;

    FsFile *getFsFile() const;

    void setFileName(const string &fileName);

    void setFsFile(FsFile *fsFile);

    void setInUse(bool inUse);

FSDISK:

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


==Program Files==
fsFile- All information on a single file
 fileDescriptor -Information about how each file is routed
fsDisk - Implementing the methods that define the possible options on the disk
main.cpp- all of the options that can do to the disk .
==How to compile?==
compile: g++ fsFile.cpp FileDescriptor.cpp fsDisk.cpp main.cpp  -o Ex6 -lm 
run: ./Ex6


==Input:==
The program receives number from 1-8 that defines the options that can do to the disk

==Output:==
Depending on the action, writing or reading from the file, deleting the file, formatting will return us a number of blocks, etc.


