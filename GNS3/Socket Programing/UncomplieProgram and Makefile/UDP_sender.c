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
        int num_parts=(short)atoi(argv[3]);
        int clientSocket,i=0,j=0;
        char buffer[Buffer_size],ch;
        struct sockaddr_in serverAddr;
        socklen_t addr_size;
        memset(buffer,0,sizeof(buffer));

        clientSocket=socket(PF_INET, SOCK_DGRAM, 0);
        if(clientSocket < 0){
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        memset(serverAddr.sin_zero, 0, sizeof serverAddr.sin_zero);

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons((short)atoi(argv[2]));   //port
        serverAddr.sin_addr.s_addr = inet_addr(argv[1]);  //ip
        addr_size = sizeof(serverAddr);

        printf("UDP start send!\n");

        FILE *fp;
        if((fp= fopen(argv[4],"r"))<0){
            perror("open FILE failed");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }

        while(i<num_parts) {
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
            if(sendto(clientSocket, buffer, Buffer_size, 0, (struct sockaddr*)&serverAddr, addr_size)<0){
                perror("sending failed");
                close(clientSocket);
                exit(EXIT_FAILURE);
            }
            i++;
        }

        printf("Finish send all data \n");
        fclose(fp);
        close(clientSocket);
        return 0;

    } else {
        printf("4 argument expected.\n");
        return -1;
    }
}