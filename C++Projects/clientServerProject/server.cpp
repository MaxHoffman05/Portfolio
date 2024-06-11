/**
 * Computer Systems II - Programming Project #4 - Server and Client connections
 * 
 * Name: Max Hoffman
 * Date: 4/18/2024 - 4/23/2024
 * Purpose: server.cpp - process commands incoming from client.cpp commands include get, clear, and add
 * 
*/

#include <iostream>
#include <cstdlib>
#include <netdb.h>//for connecting server and client together
#include <unistd.h>
#include <pthread.h>//for threads

using namespace std;
#define MAX_WAITING 25

int accumulator_val = 0;//global value for our accumulator

//same struct from the lecture
struct do_work_struct {
    unsigned int with_sock;
    struct sockaddr_in *fromcli;
};

/*
 * do_work() method for work done by the server
 * generic_ptr - ptr to the client
*/
void* do_work(void *generic_ptr) {
    struct do_work_struct *actual_ptr = (struct do_work_struct *) generic_ptr;
    unsigned int with_sock;
    with_sock = actual_ptr -> with_sock;

    //! read in the command into a string
    int n = 0;//how many bytes did we just read?
    char recvln[81];//actual bytes (characters) read, up to 81
    //read in the command from the client
    bool flag = true;
    while(flag) {
        n = read(with_sock, recvln, 80);
        string str = (string) recvln;
        if(str.find('\n') != -1) {//look for /n and stop reading if we find it
            flag = false;
        }
    }
  
    //! get our command's name
    string totalString = (string) recvln;
    int num = totalString.find(" ");
    string com = totalString.substr(0,num);
    if(com == "add") {//if statment for each command starting with add command
        //get number from command
        string addCom = totalString.substr(num+1, totalString.size());
        accumulator_val += atoi(addCom.c_str());//add to accumulator
    }
    else if(com == "clear\n") {//if it was a clear command
        //clear the accumulator var
        
        accumulator_val = 0;
    }
    if(com == "get\n" || com == "add") {//if it was a get or add command
        //need to return accumulator val to the client for both types of commands
        string strAcc = to_string(accumulator_val);//make the accumulator value a string
        int needed = strAcc.length();
        char * cbuff = (char *) strAcc.c_str();

        //we will want to write the accumulator val to the client so it can be shown to the user
        while(needed > 0) {
            int n = write(with_sock, cbuff, needed);//returns how many characters that you wrote, could be less than needed since it might not send all of the characters in the buffer
            needed -= n;
            cbuff += n;//change the pointer by n number of bytes
        }
    }
    //done processing the command
    close(with_sock);
    return nullptr;
}

/*
 * server() method 
 * portNum - port number entered by the user for the server
*/
int server(unsigned int portNum) {
    //setup the server, same code from lectures except we use portNum for our port number when setting up the connection
    int listen_sock;
    struct sockaddr_in local_addr;
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);

    if(listen_sock < 0) {//sometimes you cannot create a socket you are limited to a certain number (rare)
        cerr << "Could not create socket" << endl;
        return 1;
    }

    local_addr.sin_family = AF_INET;//Use IPv4 address
    local_addr.sin_addr.s_addr = INADDR_ANY;//use "wildcard" IP address
    local_addr.sin_port = htons(portNum);//port host to name server

    int rval = bind(listen_sock, (struct sockaddr *) &local_addr, sizeof(local_addr));
    if(rval != 0) {
        cerr << "Binding failed -possible causes include:" << endl
        << " - invalid port number (access denied or already in use?)" << endl
        << " - invalid local address (did you use the wildcard?)" << endl;
        return 1;
    }

    rval = listen(listen_sock, MAX_WAITING);//listen and wait for clients

    if(rval != 0) {
        cerr << "Error listen() failed " << endl;
        return 1;
    }
    

    while(true) {//same code we had from the lecture we have a while true waiting loop to accept new clients and create a thread for each one
        //accept incomming client
        int accepted_socket; //socket for actual connection
        struct sockaddr_in from; //holds client address data
        unsigned int from_len; //bytes in from
        from_len = sizeof(from);

        accepted_socket = accept(listen_sock, (struct sockaddr*) &from, &from_len);//sits here until it recieves something from the listen_sock
        //process incoming client
        //as soon as you have created a thread we come back to get another client
        struct do_work_struct *params = new struct do_work_struct;
        params -> with_sock = accepted_socket;
        params -> fromcli = &from; 
        //call do_work on a new thread per client
        pthread_create(new pthread_t, NULL,  do_work, params);
    }
    close(listen_sock);//close our socket
    return 0;
}


int main(int argc, char *argv[]) {
    //check to see if we have a command line argument for the port number
    if(!argv[1]) {
        cerr << "Include port number on the command line e.x: " << argv[0] << " <port number> " << endl;
        return 1;
    }
    else {
        int portNum = atoi(argv[1]);//get our port number for the command line
        server(portNum);//setup the server and get ready to process commandss
    }

    return 0;
}