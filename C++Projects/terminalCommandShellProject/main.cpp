#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstring>
#include <string>
#include <filesystem>
#include <sys/wait.h>
#include <cstdio>
#include <signal.h>
#include <fstream>
#include <signal.h>
#include "Command.hpp"
using namespace std;

/*
Project 1 - Computer Systems I - Command Shell Project

Author: Max Hoffman

Date: 9/16/2023 - 10/3/2023
*/

/*
Sources:
Built off of the main.cpp you provided
Used for c_str() after you mentioned it to me : https://cplusplus.com/reference/string/string/c_str/
Used for debugging my cd command, I found get_current_dir_name() there : https://linux.die.net/man/3/get_current_dir_name
Used for strcpy() syntax : https://cplusplus.com/reference/cstring/strcpy/
Used to kill zombie processes : https://itsfoss.com/kill-zombie-process-linux/
Used for fopen() syntax for redirects : https://cplusplus.com/reference/cstdio/fopen/
Used for signal syntax : https://en.cppreference.com/w/cpp/utility/program/signal

*/
bool isPipe = false;

int pidBack;//for printing backgrounding commands info
Command backgroundCommand;

int pidPipe;//used for printing pipe command info
Command pipeCommand;

//used for printing out the information after a background command is completed
void handleBackgrounds(int sigNum) {
    cout << "PID of backgrounded command: " << pidBack << endl;
    cout << "Command: ";

    for(int i = 0; i < backgroundCommand.numArgs(); i++) {
        cout << backgroundCommand.args()[i] << " ";
    }
    cout << "&";
    cout << endl;
    signal(sigNum, SIG_IGN);//we need to se the signal to be ignored after using the signal method because if not it will print handleBackgrounds() for other commands that use children to execute commands
    
}

//function for printing out the pipe info after completing the command
void handlePipes(int sigNumPipe) {
    cout << "Completed: PID=" << pidPipe;
    cout << ": ";
    for(int j=0; j < pipeCommand.numArgs(); j++) {
        cout << pipeCommand.args()[j] << " ";
    }
    cout << endl;

    signal(sigNumPipe, SIG_IGN);
}
//function for any command that is not a pipe, background or redirect
void normalCommand(char* *fullC, Command com, bool flag) {
  
  if(com.name() == "cd") {   
        chdir(com.args()[1].c_str());
        string name = get_current_dir_name();
        cout << "You are now in this directory : " << name << endl;
    }
    //for any command
    else {
        if(flag == false) {
            int pid = fork();

            if(pid != 0) {
                waitpid(pid, NULL, 0);
            }
            else {
                execvp(fullC[0], fullC);
            }
        }
        else {
            //because of how I built the program we need to not execvp a command that is part of a pipe and we use a bool to not do that

        }
    }
}

int main(void) {
    Command com;   
    int dataPipe;
    bool normalCom = true;//bool to check if we are dealing with a command that is not a pipe, not backgrounded or a redirect
    // prompt for and read in first command. 
    cout << ">>>> ";
    com.read();
    int pipeInfo[2];
    
    while(com.name() != "exit") {
        normalCom = true;
        //array of character pointers that will contain pointers that point to each part of the full command
        
        if(com.pipeOut()) {//we first must see if it is a pipe since the way the command class works a pipe stops a command from being read and it means this is really two commands

            //make 2 different arrays for each parts of the command
            char* *firstPart = new char*[com.numArgs() + 1];
            
            for(int i = 0; i < com.numArgs(); i++) {
                firstPart[i] = new char[100];
                strcpy(firstPart[i], com.args()[i].c_str());
            }
            firstPart[com.numArgs()] = NULL;//we make the last entry in the array null since it is required for pipes
           
            //create the pipe and see if it worked
            dataPipe = pipe(pipeInfo);
            if(dataPipe == -1) {
                cout << "Failed to pipe this command, try again" << endl;
            }
            else {//continue with pipe stuff
                pipeCommand = com;
                int pidPipe = fork();

                if(pidPipe != 0) {//second part of the command runs here in the parent where the command after the pipe will go through the while loop again
                    //dup2(pipeInfo[0], fileno(stdin));
                    close(pipeInfo[1]);//we arent going to write to the pipe only read
                }
                else {//first part of the command runs here in the child (part before the pipe)
                    dup2(pipeInfo[1], fileno(stdout));
                    close(pipeInfo[0]);//we arent going to read from the pipe only write
                    execvp(firstPart[0], firstPart);
                }
                isPipe = true;
            }
        } 
        else {//if a command is not a pipe command we can treat it as one whole command instead of a 2-part command like the pipe command
            char* *fullC = new char*[com.numArgs()];
            //load in our command into the fullC array
            for(int i = 0; i < com.numArgs();  i++) {
                //basically very similar method to the in class example we did with 
                //allocate the memory for the command as an array of chars 
                fullC[i] = new char[100];

                //copy into the fullC character array a character array (which when combined is like a string), in each of the spots of fullC a pointer points to this character array.
                strcpy(fullC[i], com.args()[i].c_str());
            }
            //should we check first if the command needs to be backgrounded?
            if(com.backgrounded()) {
                signal(SIGCHLD, handleBackgrounds);//call signal to signal when a child has ended which in this case is the backgrounded command
                
                backgroundCommand = com;//also get the command name and store to a global variable
                pidBack = fork();
                if(pidBack == 0) {
                    pidBack = getpid();//get the pid to output later
                    execvp(fullC[0], fullC);
                }
                normalCom = false;
            }
            //INPUT & OUTPUT REDIRECT COMMANDS
            if(com.redirIn() && com.redirOut() && normalCom == true) {
                cout << "testing both input and output redirect" << endl;
                FILE *fileW = fopen(com.outputRedirectFile().c_str(), "w");
                FILE *fileR = fopen(com.inputRedirectFile().c_str(), "r");
                int combinedPID = fork();

                if(combinedPID == 0) {
                    //set the input as the first file from the input redirect and set the output to the output file from 
                    //the output redirect
                    dup2(fileno(fileR), fileno(stdin));
                    dup2(fileno(fileW), fileno(stdout));
                    execvp(fullC[0], fullC);
                }
                else {
                    waitpid(combinedPID, NULL, 0);
                }
                normalCom = false;
            }
            if(com.redirIn() && normalCom == true) {//for input redirects, if normalCom is true at this point that means we did not do a command that was an input and output redirect
                //similar to the dup2ex.cpp we did in class the only difference is we are calling dup2 in the child process so then 
                FILE *fileToLookAt = fopen(com.inputRedirectFile().c_str(), "r");
                int pID = fork();
                if(pID == 0) {
                    dup2(fileno(fileToLookAt), fileno(stdin));
                    execvp(fullC[0], fullC);
                }
                else {
                    waitpid(pID, NULL, 0);
                    fclose(fileToLookAt);
                }
                normalCom = false;
            }
            //OUTPUT REDIRECT COMMANDS
            if(com.redirOut() && normalCom == true) {//for output redirects, same code as input redirects except the file is 
                //going to be written to and we are using stdout instead of cin since we are outputting to the file
               
                FILE *fileToWrite = fopen(com.outputRedirectFile().c_str(), "w");
                int pidOut = fork();

                if(pidOut == 0) {
                    dup2(fileno(fileToWrite), fileno(stdout));
                    execvp(fullC[0], fullC);
                }
                else {
                    waitpid(pidOut, NULL, 0);
                    fclose(fileToWrite);
                }
                normalCom = false;
            }
        
            //if a command is not a pipe, > or < then we run it through the normalCommand() function
            if(normalCom) {
                if(isPipe == true) {//if we are dealing with the rest of a pipe command we will not use normalCommand() we will do it here in main so we have access to pipeInfo[]
                    //we also need to setup a signal to print out the command and the pid like how it is shown in the example output
                    //same code as what we did for background commands
                    signal(SIGCHLD, handlePipes);
                   
                    int pid3 = fork();
                    if(pid3 != 0) {
                        pidPipe = getpid();//get the pid of the process to be used later when printing out the command info
                        waitpid(pid3, NULL, 0);
                    }   
                    else {
                        
                        dup2(pipeInfo[0], fileno(stdin));//set standard input 
                        execvp(fullC[0], fullC);//execute the command with the cin being the information in the pipe
                    }
                    isPipe = false;
                }
                else {
                    normalCommand(fullC, com, isPipe);//if this command is not the "second part" of a pipe command then we will use the normalCommand() method
                } 
            }
        }
       
       
        // prompt for and read next command
	    cout << ">>>> ";
        com.read();
        if(isPipe == true) {
            dup2(pipeInfo[1], fileno(stdin));
        }
        
       
        
    }
    cout << "You have now exited Project 1 Shell, welcome back to your regular command prompt" << endl;
}
