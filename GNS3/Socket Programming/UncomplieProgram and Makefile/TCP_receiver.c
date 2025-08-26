#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define Buffer_size 1024

int main( int argc, char *argv[] )  {

    if( argc == 4 ) {
        int total_data =Buffer_size*((short)atoi(argv[3]));  //num of data unit
        int clientSocket,i=0, numOfByte=0, numOfRecive=0;
        char buffer[Buffer_size];
        struct sockaddr_in serverAddr;
        socklen_t addr_size;
        memset(buffer,0,sizeof(buffer));

        clientSocket=socket(PF_INET, SOCK_STREAM, 0);
        if(clientSocket<0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons((short)atoi(argv[2]));  //port number
        serverAddr.sin_addr.s_addr = inet_addr(argv[1]);    //ip address
        memset(serverAddr.sin_zero, 0, sizeof serverAddr.sin_zero);
        addr_size = sizeof(serverAddr);
        printf("try to connect...\n");

        if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)<0){
            perror("connect failed");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }
        printf("connection succeeded!\n");

        while((numOfByte<total_data&&((numOfRecive=recv(clientSocket,buffer,Buffer_size,0))>0))) {
            numOfByte += numOfRecive;
            buffer[numOfRecive]='\0';
            printf("data is %s: \n",buffer);
            memset(buffer,'0',sizeof(buffer));
        }

        if(numOfRecive<0) {
            perror("reading from socket failed");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }

        printf("Finish receive data...\n");

        printf("Total number of bytes expected is: %d, number of bytes recived is: %d. \n", total_data, numOfByte);
        close(clientSocket);
        return 0;
    }  else {
         printf("3 argument expected.\n");
         return -1;
    }
}