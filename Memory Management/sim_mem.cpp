#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <unistd.h>

#include "sim_mem.h"
char main_memory[MEMORY_SIZE];


sim_mem::sim_mem(char *exe_file_name1, char *exe_file_name2, char *swap_file_name1, int text_size, int data_size,
                 int bss_size, int heap_stack_size, int num_of_pages, int page_size, int num_of_proc) {
    int i;
    int j;
    this->text_size = text_size;
    this->data_size = data_size;
    this->bss_size = bss_size;
    this->heap_stack_size = heap_stack_size;
    this->num_of_pages = num_of_pages;
    this->page_size = page_size;
    this->num_of_proc = num_of_proc;

    nextFrameToMove=0;


    this->maxAddress = text_size+data_size+bss_size+heap_stack_size; // I added this int , to know what is the max address .
    if(num_of_proc==1)
        this->sizeWithoutTextSize = data_size+bss_size+heap_stack_size;
    else
        this->sizeWithoutTextSize = (data_size+bss_size+heap_stack_size)*2;

    for(i=0;i<sizeWithoutTextSize;i++) {//Insert 0 to the array for the swap file .
        arrayZeroForSwap[i] = '0';
    }
    arrayZeroForSwap[i]='\0';

    for(i=0;i<MEMORY_SIZE/page_size;i++) {//Insert 0 to the array  that will say witch frame is free .
        arrayFreeFrame[i] = 0;
    }
    arrayFreeFrame[i]='\0';


    page_table = new page_descriptor *[num_of_proc]; // New memory that will contain the page tables , 1 or 2 .

    if( num_of_proc==1) {
        program_fd[0] = open(exe_file_name1, O_RDONLY  | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (program_fd[0] == -1) {
            perror("cannot open file");
            exit(EXIT_FAILURE);
        }
    }

    else {
        program_fd[0] = open(exe_file_name1, O_RDONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (program_fd[0] == -1) {
            perror("cannot open file");
            exit(EXIT_FAILURE);
        }


        program_fd[1] = open(exe_file_name2, O_RDONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (program_fd[1] == -1) {
            perror("cannot open file");
            exit(EXIT_FAILURE);
        }
    }

    swapfile_fd = open(swap_file_name1,  O_CREAT|O_RDWR , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(swapfile_fd==-1)
    {
        perror("cannot open file");
        exit(EXIT_FAILURE) ;
    }

    for (i = 0; i < num_of_proc; i++) {//Creating the page tables , according to the number of pages , insert beginning values .
        page_table[i] = new page_descriptor[num_of_pages];
        for (j = 0; j < num_of_pages; j++) {
            page_table[i][j].V = 0;
            page_table[i][j].frame = -1;
            if(j<text_size/page_size)
                page_table[i][j].P = 0;//If page text 0 else 1 .
            else
                page_table[i][j].P = 1;//If page text 0 else 1 .
            page_table[i][j].swap_index = -1;
            page_table[i][j].D = 0;
        }
    }

    for (i = 0; i < MEMORY_SIZE; i++){ // Insert 0 to the Ram .
        main_memory[i]='0';
    }

    write(swapfile_fd,arrayZeroForSwap,strlen(arrayZeroForSwap)); // Insert 0 to the swap file according to the sizes without text size .

}

sim_mem::~sim_mem() {
    if(close(program_fd[0])<0){
        perror("cannot close file");
        exit(EXIT_FAILURE) ;
    }

    if(num_of_proc==2) {
        if (close(program_fd[1]) < 0) {
            perror("cannot close file");
            exit(EXIT_FAILURE);
        }
    }

    if (close(swapfile_fd)<0) {
        perror("cannot close file");
        exit(EXIT_FAILURE) ;
    }

    for (int i = 0; i < num_of_proc ; i++) {
        delete page_table[i];
    }
    delete[] page_table;
}

char sim_mem::load(int process_id, int address) {
    if(address>=maxAddress){
        perror("Failed address");
        exit(EXIT_FAILURE);
    }

    char arrayZeroSizeOfPage [page_size];//Array that will help me to reset the swap file .
    for(int i =0;i<page_size;i++){
        arrayZeroSizeOfPage[i]='0';
    }

    int theRightPage;
    int theRightProc;
    char arrayTest[page_size];// Array for reading and writing to the swap file , testing which index in the swap is clear and more ..

    int pageNum = address/page_size;
    int offSet = address%page_size;

    if(page_table[process_id-1][pageNum].V==1) {
        return main_memory[page_table[process_id-1][pageNum].frame * page_size + offSet];
    }
    else{

        if(address<maxAddress && address>=text_size+data_size+bss_size && page_table[process_id-1][pageNum].D == 0){ // If we try to insert page from the heap stack that is not valid, Error .
            perror("Cant load from heap_stack that is not in the memory");
            return -1;
            //exit(EXIT_FAILURE);
        }

        page_table[process_id-1][pageNum].V=1;//Change valid to 1

        int freeSpace = findFreeFrame(arrayFreeFrame); // Find where is the right index to insert .

        //--------------- If the Ram is full ------------------
        if(freeSpace==-1) {
            int counter = 0;
            int i ;
            if(num_of_proc==2) {
                if (findThePage(nextFrameToMove, 0) > findThePage(nextFrameToMove,
                                                                  1)) { // Find the original page that we want to move to the swap . according to the int that contain the frame to move.
                    theRightPage = findThePage(nextFrameToMove, 0);
                    theRightProc = 0;
                } else {
                    theRightPage = findThePage(nextFrameToMove, 1);
                    theRightProc = 1;
                }
            }
            else {
                theRightPage = findThePage(nextFrameToMove, 0);
                theRightProc = 0;
            }

            page_table[theRightProc][theRightPage].V= 0 ;// The page in no longer valid .
            page_table[theRightProc][theRightPage].frame = -1; // The page is no longer in the Ram .

            if(page_table[theRightProc][theRightPage].D==1){
                while (counter*page_size < sizeWithoutTextSize) {
                    lseek(swapfile_fd, counter * page_size, SEEK_SET);
                    if (read(swapfile_fd, arrayTest, page_size) < 0) {
                        perror("cannot read file");
                        exit(EXIT_FAILURE);
                    }
                    if(compare1(arrayTest)==1){
                        insertCharsToArray(nextFrameToMove,arrayTest);
                        lseek(swapfile_fd,counter*page_size,SEEK_SET);
                        write(swapfile_fd,arrayTest, page_size);
                        //page_table[theRightProc][theRightPage].frame = -1; // The page is no longer in the Ram , he is in the swap .
                        page_table[theRightProc][theRightPage].swap_index = counter*page_size;
                       // page_table[theRightProc][theRightPage].V= 0 ;// The page in no longer valid .
                        break;
                    }
                    counter++;
                }
            }
            freeSpace=nextFrameToMove;
            nextFrameToMove++;

            if(nextFrameToMove==MEMORY_SIZE/page_size)// The algoritem that moving to pages to the swap is fifo , if we already moving all the frames we want to start over .
                nextFrameToMove = 0;

        }
        //--------------- If the Ram is full .------------------

        page_table[process_id-1][pageNum].frame=freeSpace;

        if(page_table[process_id-1][pageNum].swap_index!=-1){//If the page is in the file swap .
            lseek(swapfile_fd, page_table[process_id-1][pageNum].swap_index , SEEK_SET);
            if (read(swapfile_fd, &main_memory[freeSpace*page_size], page_size) < 0) {
                perror("cannot read file");
                exit(EXIT_FAILURE);
            }
            lseek(swapfile_fd, page_table[process_id-1][pageNum].swap_index , SEEK_SET);
            if(write(swapfile_fd,arrayZeroSizeOfPage,page_size)<0) { // Insert 0 to the swap file according to the sizes without text size .
                perror("cannot write to file");
                exit(EXIT_FAILURE);
            }
            page_table[process_id-1][pageNum].swap_index=-1; //Reset the swap index because the page in not in the swap file anymore .
            return main_memory[freeSpace*page_size+offSet];
        }

        if(address>=text_size+data_size){// If the address is from the bss .
            int count = page_size;
            int index = freeSpace*page_size;
            while(count>0) {
                main_memory[index] = '0';
                count--;
                index++;
            }
            return '0';
        }

        lseek(program_fd[process_id-1],pageNum*page_size,SEEK_SET);

        if(read(program_fd[process_id-1],&main_memory[freeSpace*page_size],page_size)<0)
        {
            perror("cannot read file");
            exit(EXIT_FAILURE);
        }
        return main_memory[freeSpace*page_size+offSet];
    }
}


void sim_mem::store(int process_id, int address, char value) {
    if(address>=maxAddress){
        perror("Failed address");
        exit(EXIT_FAILURE);
    }
    if(address<text_size){
        perror("Can't write to a text address");
        return ;
        //exit(EXIT_FAILURE);
    }

    char arrayZeroSizeOfPage [page_size];//Array that will help me to reset the swap file .
    for(int i =0;i<page_size;i++){
        arrayZeroSizeOfPage[i]='0';
    }

    int theRightPage;
    int theRightProc;
    char arrayTest[page_size];// Array for reading and writing to the swap file , testing which index in the swap is clear and more ..

    int pageNum = address/page_size;
    int offSet = address%page_size;
    if(page_table[process_id-1][pageNum].V==1) {
        main_memory[page_table[process_id-1][pageNum].frame * page_size + offSet] = value;
        page_table[process_id-1][pageNum].D=1;
    }

    else{
        page_table[process_id-1][pageNum].V=1;

        int freeSpace = findFreeFrame(arrayFreeFrame); // Find where is the right index to insert .

        if(freeSpace==-1) {// If the Ram is full .
            char arrayTest[page_size];
            int counter = 0;
            int i ;
            if(num_of_proc==2) {
                if (findThePage(nextFrameToMove, 0) > findThePage(nextFrameToMove,
                                                                  1)) { // Find the original page that we want to move to the swap . according to the int that contain the frame to move.
                    theRightPage = findThePage(nextFrameToMove, 0);
                    theRightProc = 0;
                } else {
                    theRightPage = findThePage(nextFrameToMove, 1);
                    theRightProc = 1;
                }
            }
            else {
                theRightPage = findThePage(nextFrameToMove, 0);
                theRightProc = 0;
            }


            page_table[theRightProc][theRightPage].V= 0 ;// The page in no longer valid .
            page_table[theRightProc][theRightPage].frame = -1; // The page is no longer in the Ram .

            if(page_table[theRightProc][theRightPage].D==1){

                while (counter*page_size < sizeWithoutTextSize) {
                    lseek(swapfile_fd, counter * page_size, SEEK_SET);
                    if (read(swapfile_fd, arrayTest, page_size) < 0) {
                        perror("cannot read file");
                        exit(EXIT_FAILURE);
                    }
                    if(compare1(arrayTest)==1){
                        insertCharsToArray(nextFrameToMove,arrayTest);
                        lseek(swapfile_fd,counter*page_size,SEEK_SET);
                        write(swapfile_fd,arrayTest, page_size);
                       // page_table[theRightProc][theRightPage].frame = -1; // The page is no longer in the Ram , he is in the swap .
                        page_table[theRightProc][theRightPage].swap_index = counter*page_size;
                       // page_table[theRightProc][theRightPage].V= 0 ;// The page in no longer valid .
                        break;
                    }
                    counter++;
                }
            }
            freeSpace=nextFrameToMove;

            nextFrameToMove++;

            if(nextFrameToMove==MEMORY_SIZE/page_size)// The algoritem that moving to pages to the swap is fifo , if we already moving all the frames we want to start over .
                nextFrameToMove = 0;
        }

        page_table[process_id-1][pageNum].frame=freeSpace;

        if(page_table[process_id-1][pageNum].swap_index!=-1){//If the page is in the file swap .
            lseek(swapfile_fd, page_table[process_id-1][pageNum].swap_index , SEEK_SET);
            if (read(swapfile_fd, &main_memory[freeSpace*page_size], page_size) < 0) {
                perror("cannot read file");
                exit(EXIT_FAILURE);
            }
            lseek(swapfile_fd, page_table[process_id-1][pageNum].swap_index , SEEK_SET);
            if(write(swapfile_fd,arrayZeroSizeOfPage,page_size)<0) { // Insert 0 to the swap file according to the sizes without text size .
                perror("cannot write to file");
                exit(EXIT_FAILURE);
            }
            page_table[process_id-1][pageNum].swap_index=-1; //Reset the swap index because the page in not in the swap file anymore .
            main_memory[freeSpace*page_size+offSet] = value;
            return;
        }

        if(address>=text_size+data_size){// If the address is from the bss .
            int count = page_size;
            int index = freeSpace*page_size;
            while(count>0) {
                main_memory[index] = '0';
                count--;
                index++;
            }
        }
        else {
            lseek(program_fd[process_id - 1], pageNum * page_size, SEEK_SET);

            if (read(program_fd[process_id - 1], &main_memory[freeSpace * page_size], page_size) < 0) {
                perror("cannot read file");
                exit(EXIT_FAILURE);
            }
        }

        main_memory[freeSpace*page_size+offSet] = value;
        page_table[process_id-1][pageNum].D=1;
    }
}

void sim_mem::print_memory() {
    int i;
    printf("\n Physical memory\n");
    for(i = 0; i < MEMORY_SIZE; i++) {
        printf("%d. [%c]\n", i , main_memory[i]);
    }
}

void sim_mem::print_swap() {
    char* str = (char*)malloc(this->page_size *sizeof(char));
    int i;
    printf("\n Swap memory\n");
    lseek(swapfile_fd, 0, SEEK_SET); // go to the start of the file
    while(read(swapfile_fd, str, this->page_size) == this->page_size) {
        for(i = 0; i < page_size; i++) {
            printf("%d - [%c]\t", i, str[i]);
        }
        printf("\n");
    }
    free(str);
}

void sim_mem::print_page_table() {
    int i;
    for (int j = 0; j < num_of_proc; j++) {
        printf("\n page table of process: %d \n", j);
        printf("Valid\t Dirty\t Permission \t Frame\t Swap index\n");
        for(i = 0; i < num_of_pages; i++) {
            printf("[%d]\t\t[%d]\t\t[%d]\t\t[%d]\t\t[%d]\n",
                   page_table[j][i].V,
                   page_table[j][i].D,
                   page_table[j][i].P,
                   page_table[j][i].frame ,
                   page_table[j][i].swap_index);
        }
    }
}

int sim_mem::findFreeFrame(int arr []) {
    int i=0;
    int count = MEMORY_SIZE/page_size;
    while(count>0){
        if(arr[i]==0){
            arr[i]=1;
            return i;
        }
        count--;
        i++;
    }
    return -1;
}

int sim_mem::findThePage(int movingFrame , int numOfProc){
    int i = 0;
    while (i<num_of_pages){
        if(movingFrame==page_table[numOfProc][i].frame)
            return i;
        i++;
    }
    return -1;
}

int sim_mem::compare1(char arr []){
    for(int i = 0 ; i<page_size ; i++){
        if(arr[i]!='0')
            return 0;
    }
    return 1;
}

void sim_mem::insertCharsToArray(int frame ,  char arr []) {
    int counter = frame*page_size;
    for(int i = 0 ; i < page_size ; i++){
        arr[i]=main_memory[counter];
        counter ++;
    }
}