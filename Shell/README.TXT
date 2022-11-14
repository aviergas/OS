Shell , Ex3
Authored by Avi Ergas
208783209

==Description==

The program partially simulates the role of the shell, the user is prompted to enter a command and the program takes the user's input, and passes the sentence to the matrix so that each word is in a separate cell.
We will then send the first cell in the matrix and name it to the Execvp command and if the command is correct the son will run it and the appropriate output will be displayed on the screen, in case no error message is displayed.
We added to the previous exercise the operators, tubes, the sign & and the word nohup.
Basically the goal is to continue to simulate the behavior of the shell.

functions:

int spaceInTheBeginOrEnd - Check if the is spaces in in the begin or end .
int countChars 

void freeMatrix - Free the malloc from the system .

int checkingTheWordDone - If the word is done the function would return 1 .

int countWords 

int checkingTheCommandCd - If the word is cd the function would return 1 .

int checkingTheCommandHistory - If the word is history the function would return 1 .

int checkingTheCharMark - If the first char is '!' , the function check the next chars and if its only number she would return the number after .

void switchFromNumberToCommand – input, int (the number after the mark) , and the array that contain the scanner sentence. The function returns the command to the array , according to the number from the file.txt

int findMark

int findPipe

void sig_handler – if there is the & sign , the program will jump to the function and the father will actually execute the wait line , whose job is basically solely to clear the son's resources .

int findAnpresent

int checkingTheWordNoHup

==Program Files==
Ex3.c- contain the main , and all of the functions include the functions that responsibility for opening a file, executing the history command, and selecting a history command using the "exclamation mark" character.
==How to compile?==
compile: gcc Ex3.c  -o Ex3
run: ./Ex3

==Input:==
String from the scanner . 

==Output:==
Return file.txt that contain all of the commands .
In addition, the program will return an appropriate output according to the command entered .



