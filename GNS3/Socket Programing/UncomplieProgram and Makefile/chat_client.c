#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

#define Buffer_size 1024

int main( int argc, char *argv[] )  {

    if( argc == 5 ) {
        fd_set fdset, rdset;
        FD_ZERO(&fdset);
        FD_ZERO(&rdset);
        char ch;
        int c;
        char* username = strdup(argv[3]);
        char* message = strdup(argv[4]);
        int   msg_len = strlen(message);
        int clientSocket,byteSend,numOfRecive=0;
        char buffer[Buffer_size];
        struct sockaddr_in serverAddr;
        socklen_t addr_size;
        memset(buffer,0,sizeof(buffer));

        clientSocket=socket(PF_INET, SOCK_STREAM, 0);
        if(clientSocket<0){
            perror("socket failed");
            free(username);
            free(message);
            exit(EXIT_FAILURE);
        }
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons((short)atoi(argv[2])); //port
        serverAddr.sin_addr.s_addr = inet_addr(argv[1]);  //ip
        memset(serverAddr.sin_zero, 0, sizeof serverAddr.sin_zero);
        addr_size = sizeof serverAddr;

        if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)<0){
            perror("connect failed");
            close(clientSocket);
            free(username);
            free(message);
            exit(EXIT_FAILURE);
        }

        printf("connection succeeded!\n");
        //Chat Message
        sprintf(buffer,"%6s%d%d%s\n",username,0,0,message); //check for null byte
        buffer[6]=(char)(msg_len%16);
        buffer[7]=(char)(msg_len/16);

        if((byteSend=send(clientSocket,buffer,Buffer_size,0))<0){
            perror("send failed");
            close(clientSocket);
            free(username);
            free(message);
            exit(EXIT_FAILURE);
        }

        FD_SET(clientSocket,&fdset);
        FD_SET(fileno(stdin),&fdset);

        while(1) {
            rdset=fdset;
            if(select(FD_SETSIZE, &rdset, NULL, NULL, NULL)<0){
                perror("select failed");
                close(clientSocket);
                free(username);
                free(message);
                exit(EXIT_FAILURE);
            }

            if(FD_ISSET(fileno(stdin),&rdset)){
                //FOR BONUS - CHECK IF JUST SPACE OR MESSAGE
                printf("Client Decided to quit\n");
                close(clientSocket);
                free(username);
                free(message);
                return 0;
            } else{     //if the socket is ready
                memset(buffer,0,Buffer_size);
                if((numOfRecive=recv(clientSocket,buffer,Buffer_size,0))>0) {
                    buffer[numOfRecive]='\0';
                    printf("%s\n",buffer);
                }
                if (numOfRecive==0) {
                    printf("server close the connection\n");
                    free(username);
                    free(message);
                    return 0;
                }
                if(numOfRecive<0) {
                    perror("reading failed");
                    close(clientSocket);
                    free(username);
                    free(message);
                    exit(EXIT_FAILURE);
                }
            }
        }
    } else {
        printf("4 argument expected.\n");
        return -1;
    }
}
