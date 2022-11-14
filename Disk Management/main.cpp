#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <cstring>

#define DISK_SIZE 256
#define DISK_SIM_FILE "DISK_SIM_FILE.txt"
using namespace std;

#include "fsFile.h"
#include "FileDescriptor.h"
#include "fsDisk.h"


int main() {
    int blockSize;
    string fileName;
    char str_to_write[DISK_SIZE];
    char str_to_read[DISK_SIZE];
    int size_to_read;
    int _fd;
    int writed;

    fsDisk *fs = new fsDisk();

    int cmd_ ;
    while(1){
        cin >> cmd_;
        switch (cmd_){

            case 0: //Exit
            delete fs;
            exit(0);
            break;

            case 1: //list file
            fs->listAll();
            break;

            case 2: //format
            cin >> blockSize;
            fs->fsFormat(blockSize);
            break;

            case 3: //create - file
            cin>> fileName;
            _fd = fs->CreateFile(fileName);
            cout << "CreatFile: " << fileName << " with File Descriptor #: " << _fd << endl;
            break;

            case 4: //open file
            cin>> fileName;
            _fd = fs->OpenFile(fileName);
            cout << "OpenFile: " << fileName << " with File Descriptor #: " << _fd << endl;
            break;

            case 5: // close file
            cin >> _fd;
            fileName = fs->CloseFile(_fd);
            cout << "CloseFile: " << fileName << " with File Descriptor #: " << _fd << endl;
            break;

            case 6: // write file
            cin>> _fd;
            cin>>str_to_write;
            writed = fs->WriteToFile (_fd , str_to_write , strlen(str_to_write));
            cout << "wrote: " << writed << " Char's into File Descriptor #: " << _fd << endl;
            break;

            case 7: //read file
            cin>> _fd;
            cin>>size_to_read;
            fs->ReadFromFile (_fd , str_to_read , size_to_read);
            cout << "ReadFromFile: " << str_to_read << endl;
            break;

            case 8: //delete file
            cin>>fileName;
            _fd = fs->DelFile(fileName);
            cout << "DeleteFile: " << fileName << " with File Descriptor #: " << _fd << endl;
            break;

            case 9:
               fs->printBitVector();
            break;

            default:
                break;
        }
    }

}


