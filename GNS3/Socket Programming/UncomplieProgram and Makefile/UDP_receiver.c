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
        int num_parts=(short)atoi(argv[3]);
        int total_data =Buffer_size*num_parts;
        int serverSocket,i=0, numOfByte=0, numOfRecive=0;
        char buffer[Buffer_size];
        struct sockaddr_in serverAddr;
        struct sockaddr_in clientAddr;
        socklen_t addr_size, from_size;
        memset(buffer,0,sizeof(buffer));

        serverSocket=socket(PF_INET, SOCK_DGRAM, 0);
        if(serverSocket < 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        memset(serverAddr.sin_zero, 0, sizeof serverAddr.sin_zero);
        memset(clientAddr.sin_zero, 0, sizeof serverAddr.sin_zero);

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons((short)atoi(argv[2]));  //port
        serverAddr.sin_addr.s_addr = inet_addr(argv[1]);   //ip
        addr_size = sizeof(serverAddr);
        from_size = sizeof(clientAddr);

        if((bind(serverSocket, (struct sockaddr*)&serverAddr, addr_size)) < 0){
            perror("bind failed");
            close(serverSocket);
            exit(EXIT_FAILURE);
        }

        printf("UDP wait to connect...\n");

        while(i<num_parts) {
            numOfRecive= recvfrom(serverSocket, buffer, Buffer_size, MSG_WAITALL, (struct sockaddr*)&clientAddr, &from_size);
            if(numOfRecive<0){
                perror("reading data failed");
                close(serverSocket);
                exit(EXIT_FAILURE);
            }
            numOfByte += numOfRecive;
            buffer[numOfRecive]='\0';
            printf("data is %s: \n",buffer);
            i++;
            memset(buffer,0,sizeof(buffer));
        }

        printf("Finish received the data...\n");

        printf("Total number of bytes expected is: %d, number of bytes recived is: %d. \n", total_data, numOfByte);
        close(serverSocket);
        return 0;

    }  else {
        printf("3 argument expected.\n");
        return -1;
    }
}