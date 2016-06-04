#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<strings.h>

void helpMsg()
{
    printf("usage:\n\techoserver [options]");
    printf("\n--------------------------------------------------\n");
    printf("options:");
    printf("\n\t-p\t\tPort (Default: 8888)\n\t-h\t\tShow this help message\n");
}

int main(int argc, char *argv[])
{
 
    /*if(argc == 1)
    {
        printf("\nNo arguments!!!\n");
        exit(1);
    }*/

    int i,checkH = 0,checkP = 0;
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

    int sock, connected, bytes_recieved , port = 8888, true = 1;  
    char send_data [1024] , recv_data[1024];

    for(i = 1; i < argc-1; i++)
    {
        if(!strcmp("-p",argv[i]))
        {
           if(!(sscanf(argv[i+1],"%d",&port)) && port < 0)
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

    struct sockaddr_in server_addr,client_addr;    
    int sin_size;   

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
       perror("Setsockopt");
       exit(1);
    }
        
    server_addr.sin_family = AF_INET;         
    server_addr.sin_port = htons(port);     
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(server_addr.sin_zero),8); 

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Unable to bind");
        exit(1);
    }

    if (listen(sock, 5) == -1) {
        perror("Listen");
        exit(1);
    }

    printf("\nTCPServer Waiting for client on port %d\n",port);
    fflush(stdout);

    sin_size = sizeof(struct sockaddr_in);

    connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

    printf("\n I got a connection from (%s , %d)",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

    bytes_recieved = recv(connected,recv_data,1024,0);

    recv_data[bytes_recieved] = '\0';

    printf("\n RECIEVED DATA = %s\n" , recv_data);
    fflush(stdout);

    close(connected);
    close(sock);
}
