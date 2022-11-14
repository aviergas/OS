Sim memory , Ex4
Authored by Avi Ergas
208783209

==Description==

The program simulates how the operating system uses memory, using the logical memory using main memory and the swap area.
There are a number of cases that the program deals with, transferring directly from the logical memory, changing a page that is in the physical memory, inserting and removing what swap, and basically all the possible options that the operating system actually does on the computer.

functions:
   sim_mem(char exe_file_name1[],char exe_file_name2[], char swap_file_name1[], int text_size,
            int data_size, int bss_size, int heap_stack_size,
            int num_of_pages, int page_size, int num_of_proc);  CONSTRACTUR 

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

==Program Files==
sim_mem.h- contain  all of the functions , the program support
sim_mem.cpp    realization of the functions
main.cpp- testers of the program
==How to compile?==
compile: g++ sim_mem.cpp main.cpp  -o Ex5 -lm 
run: ./Ex5


==Input:==
The program receives one or more files. 

==Output:==
What is obtained is an illustration of the transitions in memory, depending on the situation .



