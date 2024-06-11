/**
 * Computer Systems II - Programming Project #4 - Server and Client connections
 * 
 * Name: Max Hoffman
 * Date: 4/18/2024 - 4/23/2024
 * Purpose: client.cpp - send 3 different types of commands to server.cpp: get, clear, add. Depending on the command display a return value
 * 
*/
#include <iostream>
#include <cstdlib>
#include <netdb.h>
#include <unistd.h>

using namespace std;

/*
* client() method
* server - pointer for the server
* portNum - port number entered by the user
* com - command entered by the user
* num - number passed from the user, only used on the add command
*/
int client(char *server, int portNum, string com, int num) {
    
    int rval = 0;
    struct hostent *serverEntry;
    struct sockaddr_in serverInfo;
    serverEntry = gethostbyname(server);

    if(!serverEntry) {
        cerr << "Gethostbyname() failed for name: " << server << endl;
        return 1;
    }
    int mySock = socket(AF_INET, SOCK_STREAM, 0);
    serverInfo.sin_family = AF_INET;//IPv4 address
    serverInfo.sin_addr = *((struct in_addr *) serverEntry -> h_addr_list[0]);//address for the server
    serverInfo.sin_port = htons(portNum);//port host to name server
    
    rval = connect(mySock, (struct sockaddr *) &serverInfo, sizeof(serverInfo));
    if(rval < 0){//negative numbers mean error
        cerr << "Error when connecting to server, error: " << rval << endl;
        return 1;
    }
    
    
    //setup the command string we will add the number if it is an add command and for any command it will end with \n
    //this makes it so the server can tell when to stop reading
    string origCommand = com;
    if(com == "add") {
        com.append(" " + to_string(num) + "\n");
    }
    else {
        com.append("\n");
    }


    //setup our command buffer
    char * cbuff = (char *) com.c_str();
    int needed = com.length();
    int n = 0;//how many bytes did we just read?

    //write the command to the server, regardless of type of command
    while(needed > 0) {
        int n = write(mySock, cbuff, needed);//returns how many characters that you wrote, could be less than needed since it might not send all of the characters in the buffer
        needed -= n;
        cbuff += n;//change the pointer by n number of bytes
    }
    if(origCommand == "get" || origCommand == "add") {//if we had a get or add command we expect to read the accumulator value passed from the client
        int x;
        char answ[81];//asnwer back from server, character array
        while(( x = read(mySock, answ, 80)) > 0) {//we dont know how many things the server will give me, if you call read it is telling you that then you cannot read anymore. this is a blocking call, it will wait for data to be read
            answ[x] = '\0'; //null terminate returned "string", so cout stops printing out things
        }

        cout << answ << endl;   
    }
    close(mySock);
    return 0;
}

int main(int argc, char *argv[]) {
    //client takes two command line arguments
    if(argc != 3) {
        cerr << "Wrong command line arguments, e.x: " << argv[0] << " <IP address of server> <Port number>" << endl;
        return 1;
    }
    else {
        //get our port number
        int portNum = atoi(argv[2]);

        //prompt user for command
        cout << "Enter a command : " << endl;
        string command;
        int num = 0;//number for add command
        cin >> command;
        if(command == "add")//also grab the number we are adding for the add command
        {
            cin >> num;
        }
        client(argv[1], portNum, command, num);//call client connect and process function
    }
    return 0;
}