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
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>



#include "fsDisk.h"
#include "fsFile.h"

fsDisk::fsDisk() {

    sim_disk_fd = fopen(DISK_SIM_FILE , "r+");
    assert(sim_disk_fd);


    for(int i = 0 ; i< DISK_SIZE ; i++){
        int ret_val = fseek(sim_disk_fd , i , SEEK_SET);
        ret_val = fwrite ("\0" , 1 ,1 , sim_disk_fd);
        assert(ret_val == 1);
    }

    fflush(sim_disk_fd);
    is_formated = false ;
    fclose(sim_disk_fd);

}

fsDisk::~fsDisk(){


    delete [] BitVector;

    for(int i = 0 ; i < MainDir.size() ; i++){
        if(MainDir[i]!=nullptr) {
            delete MainDir[i]->getFsFile();
            delete MainDir[i];
        }
        else
            delete MainDir[i];
    }
    MainDir.clear();
    OpenFileDescriptors.clear();

}


//---------------------------------------------------------------------------------------------------
void fsDisk::listAll() {
    int i = 0;
    sim_disk_fd = fopen(DISK_SIM_FILE, "r+");

    if (!MainDir.empty()) {
    for (i = 0; i < MainDir.size(); i++) {
        if(MainDir[i]!=nullptr) {
            cout << "index: " << i << ": FileName: " << MainDir[i]->getFileName() << " , isInUse: "
                 << MainDir[i]->isInUse()
                 << endl;
        }
        else{
            cout << "index: " << i
                 << endl;
        }
    }
}
    printf("-----------------------\n");

    if (!OpenFileDescriptors.empty()) {
        for (i = 0; i < OpenFileDescriptors.size(); i++) {
            if(OpenFileDescriptors[i]!=nullptr) {
                cout << "index: " << i << ": FileName: " << OpenFileDescriptors[i]->getFileName()
                     << endl;
            }
            else {
                cout << "index: " << i
                     << endl;
            }
        }
    }

    char bufy;
    cout<< "Disk content: ";
    for(i= 0 ; i < DISK_SIZE ; i++){
        cout<< "(";
        int ret_val = fseek(sim_disk_fd,i,SEEK_SET);
        ret_val = fread(&bufy , 1, 1 , sim_disk_fd);
        cout << bufy;
        cout<< ")";
    }

    cout<< "'" << endl ;
    fclose(sim_disk_fd);
}

//----------------------------------------------------------------------------------------------------------------

void fsDisk::decToBinary(int n , char &c){
    // array to store binary number
    int binaryNum[8];

    // counter for binary array
    int i = 0;
    while (n > 0)
    {
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }

    // printing binary array in reverse order
    for (int j = i - 1; j >= 0; j--)
    {
        if (binaryNum[j] == 1)
            c = c | 1u << j;
    }
}


// ------------------------------------------------------------------------
void fsDisk::fsFormat( int blockSize ) {
    if(is_formated){

        delete [] BitVector;

        for(int i = 0 ; i < MainDir.size() ; i++){
            if(MainDir[i]!=nullptr) {
                delete MainDir[i]->getFsFile();
                delete MainDir[i];
            }
        }
        MainDir.clear();
        OpenFileDescriptors.clear();

        sim_disk_fd = fopen(DISK_SIM_FILE , "r+");
        assert(sim_disk_fd);

        for(int i = 0 ; i< DISK_SIZE ; i++){
            int ret_val = fseek(sim_disk_fd , i , SEEK_SET);
            ret_val = fwrite ("\0" , 1 ,1 , sim_disk_fd);
            assert(ret_val == 1);
        }
        fclose(sim_disk_fd);
    }

    BitVectorSize = 0 ; // there is noting in the vector yet .
    blockSizeFromFormat = blockSize; // to save the block size for the rest of the functions .
    printf("FORMAT DISK: number of blocks: %d\n" , DISK_SIZE/blockSizeFromFormat);
    is_formated = true ; // The disk is now format !

    BitVector = new int [DISK_SIZE/blockSize];
    for(int i = 0 ; i<DISK_SIZE/blockSize ; i++){ // bitVector will tell witch block is free .
        BitVector[i]=0;
    }
}

// ------------------------------------------------------------------------
int fsDisk::CreateFile(string fileName) {

    if(!is_formated){ // if the disk is not formated .
        return -1;
    }

    int testIfNull = 0 ;

    FsFile *f = new FsFile(blockSizeFromFormat);
    FileDescriptor *fd = new FileDescriptor(fileName,f);


    if(MainDir.empty()){
        MainDir.push_back(fd);
        OpenFileDescriptors.push_back(fd);
    }
    else {
        for (int i = 0 ; i < MainDir.size(); i++) {
            if (MainDir[i] == nullptr) {
                MainDir[i] = fd;
                OpenFileDescriptors[i] = fd;
                return i;
            }
        }
        MainDir.push_back(fd);
        OpenFileDescriptors.push_back(fd);

    }
//    else{
//        for (int i = 0 ; i < OpenFileDescriptors.size(); i++) {
//            if(OpenFileDescriptors[i]==nullptr){
//               OpenFileDescriptors[i] = fd;
//               testIfNull=1;
//               break;
//            }
//        }
//        if(testIfNull==0) // Enter only if we didn't put fileDescriptor instead NULL .
//            OpenFileDescriptors.push_back(fd);
//        }

    return OpenFileDescriptors.size()-1; // returns the last index of the vector because we want the place of the new file.

}

// ------------------------------------------------------------------------
int fsDisk::OpenFile(string fileName) {
    for(int i = 0 ; i < OpenFileDescriptors.size() ; i++){//if the file is already open return file descriptor .
           if(OpenFileDescriptors[i]!=nullptr)
            if(OpenFileDescriptors[i]->getFileName()==fileName)
                return i;
    }

    for(int i = 0 ; i < MainDir.size() ; i++){//if came to this loop the file is close and we need to open him .
        if(MainDir[i]!= nullptr)
            if(MainDir[i]->getFileName()==fileName) {
            MainDir[i]->setInUse(true);
            for(int j = 0 ; j<OpenFileDescriptors.size() ; j++) {
                if (OpenFileDescriptors[j]== nullptr && i==j){
                    OpenFileDescriptors[j]=MainDir[i];
                    break;
                }
            }
            return i;
        }
    }

    return -1; // the file is not exist .

}

// ------------------------------------------------------------------------
string fsDisk::CloseFile(int fd) {
        for(int i = 0 ; i < OpenFileDescriptors.size() ; i++){
            if(i==fd) {//if i the same as file descriptor that mean we have the file in the maindir .
                string name =  OpenFileDescriptors[i]->getFileName();
                OpenFileDescriptors[i] = NULL ;

                for(int i = 0 ; i < MainDir.size() ; i++){//if came to this loop we need to change InUse to false since the file is close .
                    if(MainDir[i]->getFileName()==name) {
                        MainDir[i]->setInUse(false);
                        return MainDir[i]->getFileName();
                    }
                }
        }
    }
        return "-1";

}
// ------------------------------------------------------------------------
int fsDisk::WriteToFile(int fd, char *buf, int len ) {

    if(fd>OpenFileDescriptors.size()-1 || OpenFileDescriptors[fd] == nullptr){ // the file is not exist or not open .
            return -1;
    }

    BitVectorSize = findSpaceInBitVector(); // starting with insert the right block that free . -1 means disk full or un full but occupied blocks .

    if(OpenFileDescriptors[fd]->getFsFile()->getIndexBlock()==-1){ // the file is open but empty .
        OpenFileDescriptors[fd]->getFsFile()->setIndexBlock(BitVectorSize);// index block is now at the available index according to bitvectorsize.
        BitVector[BitVectorSize]=1;
        BitVectorSize = findSpaceInBitVector();//now the next place that available is moving .
    }


    if(checkIfTheDiskIsFull()==1){//the disk is full !
        return -1;
    }

    if(checkIfIndexBlockIsFull(fd)==1){//IndexBlock is full so we can't write to this file .
        return -1;
    }


    if(!is_formated){ // if the disk is not formated .
        return -1;
    }

    int usingBlocks = OpenFileDescriptors[fd]->getFsFile()->getBlockInUse(); // contain the using blocks
    double counterLen;
    int indexForText;
    int len1 = len;
    int numThatWeAdd = 0;//to know how much we added to unfull block .
    vector<int> blocksChanges; // a vector that contain the blocks that we need to work on .
    int moreThen9 = 0;

    if(usingBlocks%blockSizeFromFormat!=0) // to now if we need to insert text to unfull block .
        indexForText= insertToFreeSpace(fd,usingBlocks,buf);
    else
        indexForText = 0 ;


    sim_disk_fd = fopen(DISK_SIM_FILE , "r+");
    fseek(sim_disk_fd , (OpenFileDescriptors[fd]->getFsFile()->getIndexBlock()) * blockSizeFromFormat  , SEEK_SET);

    if(usingBlocks!=0 && usingBlocks+len>blockSizeFromFormat) {//if we made the unfull block to be full and there is more text to insert the other blocks .
        numThatWeAdd = blockSizeFromFormat-usingBlocks;
        usingBlocks = 0;
        if(len>blockSizeFromFormat)
            len1-=numThatWeAdd;
    }

    char bufy1; // I CHANGE FROM CHAR BUFY TO CHAR ARRAY BUFY .. DONT KNOW IF ITS ALLRIGHT .
    counterLen = (double)len1/blockSizeFromFormat ;
    int counterForBitVector = BitVectorSize;
    char c = counterForBitVector + 48;
    printf("\n%c\n" , c );
    if(counterForBitVector>9) {
        moreThen9 = 1;
    }

    if(counterLen>0 ) {
        while ((counterLen > 0 && (usingBlocks % blockSizeFromFormat) == 0)) {
            for (int i = 0;
                 i < blockSizeFromFormat; i++) { // insert the number of block that contain the texts from *buf .
                fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getFsFile()->getIndexBlock()) * blockSizeFromFormat + i,
                      SEEK_SET);
                fread(&bufy1, 1, 1, sim_disk_fd);
                if (bufy1 == '\0') {
                    fseek(sim_disk_fd,
                          (OpenFileDescriptors[fd]->getFsFile()->getIndexBlock()) * blockSizeFromFormat + i,
                          SEEK_SET);
                    if(counterForBitVector < 10)
                        fprintf(sim_disk_fd, "%d", counterForBitVector);//puts the right NUMBER instead of the char '\0' .
                    else
                        fprintf(sim_disk_fd, "%c", c);//puts the right CHAR instead of the char '\0' .
                    BitVector[counterForBitVector]=1;
                    blocksChanges.push_back(counterForBitVector);
                    counterForBitVector = findSpaceInBitVector();
                    c = counterForBitVector + 48;
                    break;
                }
            }
            counterLen--;
        }
    }

    else {
        while ((counterLen == 0 && (usingBlocks % blockSizeFromFormat) == 0)) {
            for (int i = 0;
                 i < blockSizeFromFormat; i++) { // insert the number of block that contain the texts from *buf .
                fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getFsFile()->getIndexBlock()) * blockSizeFromFormat + i,
                      SEEK_SET);
                fread(&bufy1, 1, 1, sim_disk_fd);
                if (bufy1 == '\0') {
                    fseek(sim_disk_fd,
                          (OpenFileDescriptors[fd]->getFsFile()->getIndexBlock()) * blockSizeFromFormat + i,
                          SEEK_SET);
                    if(counterForBitVector < 10)
                        fprintf(sim_disk_fd, "%d", counterForBitVector);//puts the right NUMBER instead of the char '\0' .
                    else
                        fprintf(sim_disk_fd, "%c", c);//puts the right CHAR instead of the char '\0' .
                    BitVector[counterForBitVector]=1;
                    blocksChanges.push_back(counterForBitVector);
                    counterForBitVector = findSpaceInBitVector();
                    c = counterForBitVector + 48;
                    break;
                }
            }
            counterLen--;
        }
    }

    while(!blocksChanges.empty()){
        for (int i = 0; (i < blockSizeFromFormat && buf[indexForText] != '\0' ) ; i++) {
            fseek(sim_disk_fd, BitVectorSize * blockSizeFromFormat + i, SEEK_SET);
            fprintf(sim_disk_fd, "%c", buf[indexForText]);
            indexForText++;
        }
        blocksChanges.erase(blocksChanges.begin());
        BitVector[BitVectorSize] = 1;
        BitVectorSize = blocksChanges.front();
    }

    OpenFileDescriptors[fd]->getFsFile()->setBlockInUse((len+usingBlocks-numThatWeAdd)%blockSizeFromFormat); // set how much blocks we used and base on that we can know if there is space that not stored .

    fclose(sim_disk_fd); // closing the file after using .
    return len;

}
// ------------------------------------------------------------------------
int fsDisk::DelFile( string FileName ) {

    int check = 0 ;
    int fd = 0;
    if(!MainDir.empty()) {
        for (fd = 0; fd < MainDir.size(); fd++) {
            if(MainDir[fd]!=nullptr)
                if (MainDir[fd]->getFileName() == FileName) {
                check = 1;
                break;
            }
        }
    }
    if(check!=1){ //The file is not appear in the MainDir .
        return -1;
    }

    int indexBuf = 0 ;
    char c ;
    int i = 0;
    int counterToJump; // were we want to jump in the file .
    sim_disk_fd = fopen(DISK_SIM_FILE , "r+");
    int endingLoop = 0 ;

    while(endingLoop==0) {
        if(MainDir[fd]->getFsFile()->getIndexBlock()==-1)//If the file is open but we didn't write to it yet .
            break;
        fseek(sim_disk_fd,
              (MainDir[fd]->getFsFile()->getIndexBlock()) * blockSizeFromFormat + i,
              SEEK_SET);
        fread(&c, 1, 1, sim_disk_fd);
        fseek(sim_disk_fd,
              (MainDir[fd]->getFsFile()->getIndexBlock()) * blockSizeFromFormat + i,
              SEEK_SET);
        fprintf(sim_disk_fd, "%c", '\0');//puts the char \0 instead of the number  .
        if(c=='\0'){
            endingLoop=1;
            break;
        }
        counterToJump = c-48;
        BitVector[counterToJump]=0; // now this block is free so we change from 1 to 0 .
        for (int j = 0 ; j < blockSizeFromFormat; j++) {
            fseek(sim_disk_fd,counterToJump * blockSizeFromFormat + j,SEEK_SET);
            fprintf(sim_disk_fd, "%c", '\0');//puts the char \0 instead of the char  .
            if( c=='\0') {
                break;
            }
        }
        i++;
    }
    if(MainDir[fd]->getFsFile()->getIndexBlock()!=-1) {
        BitVector[MainDir[fd]->getFsFile()->getIndexBlock()] = 0; // now this block is free so we change from 1 to 0 .
        MainDir[fd]->getFsFile()->setIndexBlock(-1);
        MainDir[fd]->getFsFile()->setBlockInUse(0);
    }

//    if(fd!=0){
//        FileDescriptor * fTemp = MainDir[0];
//        MainDir[0]=MainDir[fd];
//        MainDir[fd] = fTemp;
//    }


    for(int i = 0 ; i < OpenFileDescriptors.size(); i++) {
        if(OpenFileDescriptors[i]!=nullptr)
            if (OpenFileDescriptors[i]->getFileName() == FileName) {
                OpenFileDescriptors[i] = NULL;
                break;
            }
        }

    delete MainDir[fd]->getFsFile();
    delete MainDir[fd];
    MainDir[fd]=NULL;
    //MainDir.erase(MainDir.begin());

    fclose(sim_disk_fd);
    return fd ;
}
// ------------------------------------------------------------------------
int fsDisk::ReadFromFile(int fd, char *buf, int len ) {

    if(len>blockSizeFromFormat*blockSizeFromFormat) { // we cant read more then blocksize*blocksize , because this is the maximum size of file .
        buf[0]  = '\0';
        return -1;
    }

    if(fd>OpenFileDescriptors.size() || OpenFileDescriptors[fd] == nullptr){// the file is not exist or not open .
        printf("\nTEST\n");
        buf[0]  = '\0';
        return -1;
    }

    if(!is_formated){ // if the disk is not formated .
        buf[0]  = '\0';
        return -1;
    }



    int indexBuf = 0 ;
    char c ;
    int i = 0;
    int counterToJump; // were we want to jump in the file .
    sim_disk_fd = fopen(DISK_SIM_FILE , "r+");

    while(len>0) {
        fseek(sim_disk_fd,
              (OpenFileDescriptors[fd]->getFsFile()->getIndexBlock()) * blockSizeFromFormat + i,
              SEEK_SET);
        fread(&c, 1, 1, sim_disk_fd);
        counterToJump = c-48;
        for (int j = 0 ; j < blockSizeFromFormat; j++) {
           if(len<=0 )
               break;
            fseek(sim_disk_fd,counterToJump * blockSizeFromFormat + j,SEEK_SET);
            fread(&c, 1, 1, sim_disk_fd);

            if( c=='\0') {
                len=0;
                break;
            }
            buf[indexBuf] = c;
            indexBuf++;
            len--;
        }
        i++;
    }
    buf[indexBuf] = '\0';
    fclose(sim_disk_fd);
    return 1;
}

//-------------------------------------------------------------------------
int fsDisk::insertToFreeSpace(int fd , int offset , char *buf) {

            sim_disk_fd = fopen(DISK_SIM_FILE , "r+");
            int indexB= OpenFileDescriptors[fd]->getFsFile()->getIndexBlock();
            int indexNum;
            char c=1;
            char c1;
            int i=0 ;
            while (c!='\0' && i<blockSizeFromFormat) {
                fseek(sim_disk_fd, indexB * blockSizeFromFormat + i, SEEK_SET);//searching on the block that contains the indexs.
                fread(&c, 1, 1, sim_disk_fd);
                i++;
                if(c!='\0')
                    c1=c;
            }
            indexNum = c1-48;//now contains the number of the block that have free space .

            fseek(sim_disk_fd, indexNum * blockSizeFromFormat + offset, SEEK_SET);
            i=0;
            while(offset<blockSizeFromFormat && buf[i]!='\0') {
                fprintf(sim_disk_fd, "%c", buf[i]);
                i++;
                offset++;
            }
            fclose(sim_disk_fd);
            return i;
}

//---------------------------------------------------------------------
int fsDisk::checkIfIndexBlockIsFull(int fd) {
    sim_disk_fd = fopen(DISK_SIM_FILE , "r+");
    char temp;
    int counter; // we want to check the last dig of the file to know if he full or the last dig = \0
    fseek(sim_disk_fd, (OpenFileDescriptors[fd]->getFsFile()->getIndexBlock()) * blockSizeFromFormat +blockSizeFromFormat-1,
          SEEK_SET);//puts the pivot on the last index of the Index block .
    fread(&temp, 1, 1, sim_disk_fd);
    if (temp != '\0') {
        counter = temp-48;
        fseek(sim_disk_fd,  counter* blockSizeFromFormat+blockSizeFromFormat-1,
              SEEK_SET);
        fread(&temp, 1, 1, sim_disk_fd);
        if(temp!='\0') {
            fclose(sim_disk_fd);
            return 1;
        }
    }
    fclose(sim_disk_fd);
    return 0;
}

//-----------------------------------------------------------------------
int fsDisk::checkIfTheDiskIsFull() {
    int count = 0 ;
    for(int i = 0 ; i < DISK_SIZE/blockSizeFromFormat ; i++ ){
        if(BitVector[i]==0) {
            count++;
            return 0;
        }
    }
    return 1;
}

//----------------------------------------------------
void fsDisk::printBitVector() {

    for(int i = 0 ; i<DISK_SIZE/blockSizeFromFormat ; i++){ // bitVector will tell witch block is free .
        printf("%d.%d " , i ,BitVector[i] );
    }

}
//-----------------------------------------------------
int fsDisk::findSpaceInBitVector() {
    int i;
    for(i = 0 ; i < DISK_SIZE/blockSizeFromFormat ; i++){
        if(BitVector[i]==0)
            return i;
    }
    return -1;//there is no space left in the file , only full blocks or block that unfull but already occupied .
}
