#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define Buffer_size 1024

int main( int argc, char *argv[] )  {

    if( argc >0 && argc <11 ) {
        int serverSocket[10],i=0,j=0;
        char buffer[Buffer_size];
        struct sockaddr_in serverAddr[10];
        struct sockaddr_in clientAddr;
        fd_set current_socket, ready_socket;
        socklen_t addr_size[10], from_size;
        FD_ZERO(&current_socket);

        from_size= sizeof(clientAddr);
        memset(clientAddr.sin_zero, 0, sizeof clientAddr.sin_zero);

        for(i=0;i<(argc-1);i++) {
            serverSocket[i] = socket(PF_INET, SOCK_DGRAM, 0);
            if (serverSocket[i] < 0) {
                perror("socket failed");
                for(j=0;j<i;j++) {             //to free all the prev socket
                    close(serverSocket[j]);
                }
                exit(EXIT_FAILURE);
            }
            memset(serverAddr[i].sin_zero, 0, sizeof serverAddr[i].sin_zero);

            serverAddr[i].sin_family = AF_INET;
            serverAddr[i].sin_port = htons((short) atoi(argv[i + 1]));
            serverAddr[i].sin_addr.s_addr = INADDR_ANY;
            addr_size[i] = sizeof(serverAddr[i]);

            if ((bind(serverSocket[i], (struct sockaddr *) &serverAddr[i], addr_size[i])) < 0) {
                perror("binding failed");
                for(j=0;j<=i;j++){    //to free al the prev socket
                    close(serverSocket[i]);
                }
                exit(EXIT_FAILURE);
            }
            FD_SET(serverSocket[i],&current_socket);
        }
        printf("Selcet server is ready and waiting for new connection...\n");

        while(1) {
            ready_socket= current_socket;
            if(select(FD_SETSIZE, &ready_socket,NULL,NULL,NULL)<0){
                perror("select failed");
                for(i=0;i<(argc-1);i++){
                    close(serverSocket[i]);
                }
                exit(EXIT_FAILURE);
            }
            for(i=0;i<FD_SETSIZE;i++){
                if(FD_ISSET(i,&ready_socket)){      //if the socket has set, we want to know the port number and "read" to make it "unset"
                    memset(clientAddr.sin_zero, 0, sizeof clientAddr.sin_zero);
                    getsockname(i,(struct sockaddr*)&clientAddr,&from_size);
                    printf("Client with port %d has sent a message\n", ntohs(clientAddr.sin_port));
                    memset(clientAddr.sin_zero, 0, sizeof clientAddr.sin_zero);
                    recvfrom(i, buffer, Buffer_size, MSG_WAITALL, (struct sockaddr*)&clientAddr, &from_size);
                }
            }
        }
        
    }else {
        printf("1-10 argument expected.\n");
        return -1;
    }
}