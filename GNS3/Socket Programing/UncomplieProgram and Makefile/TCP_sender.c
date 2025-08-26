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
        int numOfUnit=((short)atoi(argv[3]));
        int total_data =Buffer_size*numOfUnit, i=0,j=0,byteSend=0, totalByte=0;
        char buffer[Buffer_size], ch;
        memset(buffer,0,sizeof(buffer));
        int welcomeSocket, newSocket;
        struct sockaddr_in serverAddr;
        struct sockaddr_storage serverStorage;
        socklen_t addr_size;

        welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
        if(welcomeSocket<0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons((short)atoi(argv[2]));  //port
        serverAddr.sin_addr.s_addr = inet_addr(argv[1]);  //ip
        memset(serverAddr.sin_zero, 0 , sizeof serverAddr.sin_zero);

        if((bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)))<0){
            close(welcomeSocket);
            perror("binding failed");
            exit(EXIT_FAILURE);
        }

        if(listen(welcomeSocket,5)==0){
            printf("Listening...\n");
        } else {
            perror("listening failed");
            close(welcomeSocket);
            exit(EXIT_FAILURE);
        }

        addr_size = sizeof serverStorage;
        newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);

        if(newSocket<0){
            close(welcomeSocket);
            perror("new socket failed");
            exit(EXIT_FAILURE);
        }

        FILE *fp;
        if((fp= fopen(argv[4],"r"))<0){  //file_name.txt
            close(newSocket);
            close(welcomeSocket);
            perror("open FILE failed");
            exit(EXIT_FAILURE);
        }

        while(i<numOfUnit){
            for(j=0;j<Buffer_size;j++){
                ch= fgetc(fp);
                if(ch!=EOF){
                    buffer[j]=ch;
                } else {
                    fclose(fp);
                    printf("file end :( \n");
                    break;
                }
            }
            i++;
            if((byteSend=send(newSocket,buffer,Buffer_size,0))<0){
                perror("sending failed");
                close(newSocket);
                close(welcomeSocket);
                exit(EXIT_FAILURE);
            }
            totalByte+=byteSend;
        }
        printf("finish send all data, we want to send %d bytes, and we really send: %d.\n", total_data,totalByte);
        close(newSocket);
        close(welcomeSocket);
        return 0;

    }  else {
        printf("4 argument expected...\n");
        return -1;
    }
}