
#include <iostream>
#define MEMORY_SIZE 25
extern char main_memory[MEMORY_SIZE];
#include "sim_mem.h"

int main() {
//sim_mem mem_sm((char*)"exec_file" ,(char*) "exec_file1" , (char*)"swap_file" , 25,50,25,25,25,5,2);
    sim_mem mem_sm1((char*)"exec_file" ,(char*) "" , (char*)"swap_file" , 5,5,5,20,7,5,1);


    char val = mem_sm1.load (1, 10);
    printf("\n%c\n" , val);
    val = mem_sm1.load (1, 15);

    mem_sm1.store (1, 4 , '&');

//    val = mem_sm1.load (1, 5);
//    printf("\n%c\n" , val);
////    val = mem_sm1.load (2, 6);
////    printf("\n%c\n" , val);
////    mem_sm1.print_memory();
////    mem_sm1.print_page_table();
//   // mem_sm1.store (2, 16 , '&');
//    mem_sm1.store (1, 21 , '!');
////    mem_sm1.print_memory();
////    mem_sm1.print_page_table();
//    mem_sm1.store(1,11,'z');
//    mem_sm1.store(1,5,'y');
//    mem_sm1.store(1,25 , '%');
////    mem_sm1.print_swap();
////    mem_sm1.print_memory();
////    mem_sm1.print_page_table();
//    mem_sm1.store(1,31,'*');
//    mem_sm1.store(1,32,'*');
//   // mem_sm1.store(2,6,'^');
//    mem_sm1.store (1, 7,'+');
//    val = mem_sm1.load(1,0);
//    printf("\n%c\n" , val);
//    val = mem_sm1.load(1,11);
//    printf("\n%c\n" , val);
//    mem_sm1.store(1,16, '~');
//
//
//    mem_sm1.store(1,4,'^');


    mem_sm1.print_swap();
    mem_sm1.print_memory();
    mem_sm1.print_page_table();
}




////---------------------------Main from friend ---------------


/*
#include "sim_mem.h"
using namespace std;
#define MEMORY_SIZE 30
extern char main_memory[MEMORY_SIZE];

int main()
{
    sim_mem mem_sm((char*)"exec_file", (char*)"ex2" , (char*)"swap_file" ,25, 50, 25,25, 25, 5, 2);

    printf("text: %d\t", mem_sm.text_size);
    printf("data: %d\t", mem_sm.data_size);
    printf("bss: %d \t", mem_sm.bss_size);
    printf("heap+stack: %d\n", mem_sm.heap_stack_size);
    printf("pages: %d\tsize:%d\n", mem_sm.num_of_pages, mem_sm.page_size);


    // load procces 1 txt to main memory

    mem_sm.load(1, 0);    mem_sm.load(1, 5);
    mem_sm.load(1, 10);    mem_sm.load(1, 15);
    mem_sm.load(1, 20);


    // store procces 2 data to main memory

    mem_sm.store(2, 50, '(');   mem_sm.store(2, 54, ')');
    mem_sm.store(2, 25, '~');   mem_sm.store(2, 29, '~');
   mem_sm.store(2, 30, '_');   mem_sm.store(2, 34, '_');
    mem_sm.store(2, 35, '|');   mem_sm.store(2, 39, '|');

    mem_sm.store(2, 40, ';');   mem_sm.store(2, 44, ';');
    mem_sm.store(2, 45, '=');   mem_sm.store(2, 49, '=');



    // store procces 1 heap to main memory (mallocs)


    mem_sm.store(1, 100, '/');   mem_sm.store(1, 104, '/');
    mem_sm.store(1, 105, 'p');   mem_sm.store(1, 109, 'q');
    mem_sm.store(1, 110, 'B');   mem_sm.store(1, 114, 'B');
   mem_sm.store(1, 115, '5');   mem_sm.store(1, 119, '5');
    mem_sm.store(1, 120, '~');   mem_sm.store(1, 124, '~');



    // load 1 page from p1 txt to memory

    mem_sm.load(1,0);


    // // try to load from heap
    mem_sm.load(2, 100);        // ERROR

    // // load from data p2

    mem_sm.load(2, 50);


    // // load from p1 heap
    mem_sm.load(1,100);




    /////p1 try store to txt (which is in memory)
    mem_sm.store(2,0,'T');          //      should we be allowed to store ?


    // p2 store (bring from swap)
    mem_sm.store(2, 27, 'v');

    //////p1 store in data
    mem_sm.store(1, 25, 'u');   mem_sm.store(1, 29, 'u');
    mem_sm.store(1, 30, '1');   mem_sm.store(1, 34, '1');
    mem_sm.store(1, 35, '+');   mem_sm.store(1, 39, '+');
    mem_sm.store(1, 40, '2');   mem_sm.store(1, 44, '2');


    // p1 store to BSS
    mem_sm.store(2, 75, 'V');   mem_sm.store(2, 79, 'V');
    mem_sm.store(2, 80, '#');   mem_sm.store(2, 84, '#');
    mem_sm.store(2, 85, '~');   mem_sm.store(2, 89, '~');
    mem_sm.store(2, 90, ',');   mem_sm.store(2, 94, ',');
    mem_sm.store(2, 95, 'S');   mem_sm.store(2, 99, 'S');

    // p2 store in heap (malloc)
    mem_sm.store(2, 100, '{');   mem_sm.store(2, 104, '}');

    /// load p1 txt
    mem_sm.load(1, 0);
    mem_sm.load(1, 5);
    mem_sm.load(1, 10);
    mem_sm.load(1, 15);
    mem_sm.load(1, 20);


    // load p1 bss
    mem_sm.load(1, 75);


    // // try to fill up swap file !!!!!!!

    mem_sm.store(1, 75, '\'');   mem_sm.store(1, 79, '\'');
    mem_sm.store(1, 80, 'Q');   mem_sm.store(1, 84, 'Q');
    mem_sm.store(1, 85, 'W');   mem_sm.store(1, 89, 'W');
    mem_sm.store(1, 90, 'j');   mem_sm.store(1, 94, 'j');
    mem_sm.store(1, 95, '{');   mem_sm.store(1, 99, '}');


    mem_sm.store(2, 105, '.');   mem_sm.store(2, 109, '.');
    mem_sm.store(2, 110, '3');   mem_sm.store(2, 114, '3');
    mem_sm.store(2, 115, '*');   mem_sm.store(2, 119, '*');
    mem_sm.store(2, 120, 'c');   mem_sm.store(2, 124, 'c');


    mem_sm.store(2, 55, 'f');   mem_sm.store(2, 59, 'f');
    mem_sm.store(2, 60, 'h');   mem_sm.store(2, 64, 'h');
    mem_sm.store(2, 65, 'i');   mem_sm.store(2, 69, 'i');
    mem_sm.store(2, 70, 'h');   mem_sm.store(2, 74, 'h');


    mem_sm.store(1, 45, '_');   mem_sm.store(1, 49, '_');
    mem_sm.store(1, 50, '`');   mem_sm.store(1, 54, '`');
    mem_sm.store(1, 55, '!');   mem_sm.store(1, 59, '!');

    mem_sm.store(1, 60, 'A');   mem_sm.store(1, 64, 'A');
    mem_sm.store(1, 65, 'X');   mem_sm.store(1, 69, 'X');
    mem_sm.store(1, 70, 'Y');   mem_sm.store(1, 74, 'Y');
    mem_sm.store(1, 75, 'X');   mem_sm.store(1, 79, 'X');

    mem_sm.load(1, 0);    mem_sm.load(1, 5);
    mem_sm.load(1, 10);    mem_sm.load(1, 15);
    mem_sm.load(1, 20);

    mem_sm.load(2, 0);
    mem_sm.load(2, 0);
    mem_sm.load(2, 10);


    //   // SWAP FILE SHOULD BE FULL BY NOW !!


    // // bring pages from SWAP
    mem_sm.store(1, 76, 'o');   mem_sm.store(1, 77, 'o');   mem_sm.store(1, 78, 'o');
    mem_sm.store(1, 106, 'I');   mem_sm.store(1, 107, 'I');   mem_sm.store(1, 108, 'I');
    mem_sm.store(1, 111, ':');   mem_sm.store(1, 112, ':');   mem_sm.store(1, 113, ':');



    mem_sm.print_page_table();




    mem_sm.print_memory();

    mem_sm.print_swap();

    return 0;
}

*/



////---------------------------Main from friend ---------------


/*
#include "sim_mem.h"
#include <stdio.h>


#define MEMORY_SIZE 10


int main()
{
    char val;
    sim_mem mem_sm((char*)"exec_file.txt", (char*)"", (char*)"swap_file.txt" ,10, 10, 10,10, 8, 5,1);

    printf("------ basic tests ------\n\n");

    val = mem_sm.load(1, 1);
    if (val != 'A'){
        printf("FAILED load test 1\n");
    }
    else {
        printf("SUCCESS on load test 1\n");
    }
    val = mem_sm.load(1, 27);
    if (val != '0'){
        printf("FAILED load test 2\n");
    }
    else {
        printf("SUCCESS on load test 2\n");
    }

    printf("expected failure: permission denied\n");
    mem_sm.store(1, 8, 'y');
    mem_sm.store(1, 17, 'T');
    val = mem_sm.load(1, 17);
    if (val != 'T'){
        printf("FAILED store test 1\n");
    }
    else {
        printf("SUCCESS on store test 1\n");
    }
    printf("expected failure: heap/stack page uninitialized\n");
    mem_sm.load(1, 34);
    mem_sm.store(1, 34, '$');
    val = mem_sm.load(1, 34);
    if (val != '$'){
        printf("FAILED to store test 2\n");
    }
    else if (mem_sm.load(1, 33) != '0'){
        printf("FAILED to initialize heap/stack page\n");
    }
    else {
        printf("SUCCESS on store test 2\n");
    }

    printf("\ntest your swap, should be empty: ");
    mem_sm.print_swap();

    mem_sm.load(1, 0);
    printf("\n-------final basic tests: ---------\n\n");
    printf("test your RAM. expected: \n");
    printf("[a]\n[A]\n[a]\n[A]\n[a]\n[0]\n[0]\n[0]\n[0]\n[$]\n");
    mem_sm.print_memory();
    printf("test your page table. expected:\n");
    printf("page 0:\n\t");
    printf("V = 1,\t D = 0,\t P = 0,\t frame = 0,\t swap_index = -1\n");
    printf("page 3:\n\t");
    printf("V = 0,\t D = 1,\t P = 1,\t frame = -1,\t swap_index = 0\n");
    printf("page 6:\n\t");
    printf("V = 1,\t D = 1,\t P = 1,\t frame = 1,\t swap_index = -1\n");
    mem_sm.print_page_table();
    printf("test your swap. expected:\n");
    printf("0 - [B]\t1 - [b]\t2 - [T]\t3 - [b]\t4 - [B]\nrest is empty\n");
    mem_sm.print_swap();

    mem_sm.store(1, 15, 'P');
    mem_sm.store(1, 16, 'B');
    mem_sm.store(1, 18, '!');
    mem_sm.store(1, 19, '!');
    mem_sm.store(1, 31, 'N');
    mem_sm.store(1, 32, 'R');
    mem_sm.store(1, 33, '$');
    mem_sm.store(1, 30, 'D');
    mem_sm.store(1, 10, 'S');
    mem_sm.store(1, 11, 'T');
    mem_sm.store(1, 12, 'S');
    mem_sm.store(1, 13, '!');
    mem_sm.store(1, 14, '!');

    printf("\n\n--------------medium tests------------------\n");

    mem_sm.store(1, 20, 'S');
    mem_sm.store(1, 22, 'T');
    mem_sm.store(1, 23, '$');
    mem_sm.store(1, 24, '$');

    mem_sm.store(1, 25, 'E');
    mem_sm.store(1, 26, 'A');
    mem_sm.store(1, 27, 'E');
    mem_sm.store(1, 28, '!');
    mem_sm.store(1, 29, '!');

    mem_sm.store(1, 35, 'A');
    mem_sm.store(1, 36, 'A');
    mem_sm.store(1, 37, 'E');
    mem_sm.store(1, 38, '$');
    mem_sm.store(1, 39, '$');

    mem_sm.load(1, 34);
    mem_sm.load(1, 4);
    mem_sm.load(1, 7);
    mem_sm.load(1, 15);
    mem_sm.load(1, 10);
    mem_sm.load(1, 9);
    mem_sm.store(1, 21, 'M');
    mem_sm.load(1, 0);
    mem_sm.load(1, 34);
    mem_sm.load(1, 0);
    mem_sm.load(1, 9);
    mem_sm.load(1, 4);

    printf("\n-------final tests: ---------\n\n");
    printf("test your RAM. expected: \n");
    printf("[a]\n[A]\n[a]\n[A]\n[a]\n[A]\n[a]\n[A]\n[a]\n[A]\n");
    mem_sm.print_memory();
    printf("test your page table. expected:\n");
    printf("page 0:\n\t");
    printf("V = 1,\t D = 0,\t P = 0,\t frame = 1,\t swap_index = -1\n");
    printf("page 1:\n\t");
    printf("V = 1,\t D = 0,\t P = 0,\t frame = 0,\t swap_index = -1\n");
    printf("page 2:\n\t");
    printf("V = 0,\t D = 1,\t P = 1,\t frame = -1,\t swap_index = 2\n");
    printf("page 3:\n\t");
    printf("V = 0,\t D = 1,\t P = 1,\t frame = -1,\t swap_index = 0\n");
    printf("page 4:\n\t");
    printf("V = 0,\t D = 1,\t P = 1,\t frame = -1,\t swap_index = 3\n");
    printf("page 5:\n\t");
    printf("V = 0,\t D = 1,\t P = 1,\t frame = -1,\t swap_index = 4\n");
    printf("page 6:\n\t");
    printf("V = 0,\t D = 1,\t P = 1,\t frame = -1,\t swap_index = 5\n");
    printf("page 7:\n\t");
    printf("V = 0,\t D = 1,\t P = 1,\t frame = -1,\t swap_index = 1\n");
    mem_sm.print_page_table();

    printf("\ntest your swap. if you can read the columns from left to right your swap is correct\n");
    mem_sm.print_swap();

}

*/