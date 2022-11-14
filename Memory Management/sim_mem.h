
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>

#ifndef EX5_SIM_MEM_H
#define EX5_SIM_MEM_H

#define MEMORY_SIZE 25
extern char main_memory[MEMORY_SIZE];

typedef struct page_descriptor
{
    int V; // valid
    int D; // dirty
    int P; // permission
    int frame; //the number of a frame if in case it is page-mapped
    int swap_index; // where the page is located in the swap file.
} page_descriptor;

class sim_mem {
public:
    int swapfile_fd; //swap file fd
    int program_fd[2]; //executable file fd
    int text_size;
    int data_size;
    int bss_size;
    int heap_stack_size;
    int num_of_pages;
    int page_size;
    int num_of_proc;
    page_descriptor **page_table; //pointer to page table

    int maxAddress;
    int arrayFreeFrame[1000];
    char arrayZeroForSwap[1000];
    int sizeWithoutTextSize;
    int nextFrameToMove;


    sim_mem(char exe_file_name1[],char exe_file_name2[], char swap_file_name1[], int text_size,
            int data_size, int bss_size, int heap_stack_size,
            int num_of_pages, int page_size, int num_of_proc);

    ~sim_mem();

    char load(int process_id, int address);

    void store(int process_id, int address, char value);
    void print_memory();

    void print_swap();

    void print_page_table();
    int findFreeFrame(int arr []);
    int findThePage(int movingFrame , int numOfProc);
    int compare1(char arr []);
    void insertCharsToArray(int frame , char arr []);

};

#endif //EX5_SIM_MEM_H
