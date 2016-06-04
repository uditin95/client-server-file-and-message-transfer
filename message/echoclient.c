#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void helpMsg()
{
    printf("\nusage:\n\techoclient [options]\n-----------------------------------------------------\noptions:\n\t-s\t\tserver (Default: localhost)\n\t-p\t\tPort (Default: 8888)\n\t-m\t\tMessage to send to server (Default: \"Hello World!\")\n\t-h\t\tShow this help message\n");
}

int main(int argc, char *argv[])
{
 
    /*if(argc == 1)
    {
        printf("\nNo arguments!!!\n");
        exit(1);
    }*/

    int i,checkH = 0,checkP = 0, port = 8888;

    for(i = 1; i < argc; i++)
    {
        if(!strcmp("-h",argv[i])){
           checkH = 1;
        }
    }

    if(checkH){ 
        helpMsg();
        exit(0);
    }


    for(i = 1; i < argc-1; i++)
    {
        if(!strcmp("-p",argv[i]))
        {
           if(sscanf(argv[i+1],"%d",&port) <= 0 && port < 0)
           {    
                checkP = 1;
           }
        }
    }

    if(checkP){
        port = 8888;
        printf("\nDefault port: %d is selected.\n",port);
    }
    else printf("\nPort: %d is selected.\n",port);

    int sock, bytes_recieved;  
    char send_data[1024] = "Hello World!",recv_data[1024];
    struct hostent *host;
    struct sockaddr_in server_addr;  

    host = gethostbyname("127.0.0.1");

    if ((sock = socket(AF_INET, SOCK_STREAM, 0))< 0) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;     
    server_addr.sin_port = htons(port);   
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero),8); 

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) < 0) 
    {
        perror("Connect");
        exit(1);
    }

    for(i = 1; i < argc; i++)
    {
        if(!strcmp("-m",argv[i])){
           strcpy(send_data,"");
           while(++i < argc && strcmp("-p",argv[i]) && strcmp("-s",argv[i])){
               strcat(send_data,argv[i]);
           }
        strcat(send_data,"\0");
        }
    }     

    send(sock,send_data,strlen(send_data), 0); 

    close(sock);
}
