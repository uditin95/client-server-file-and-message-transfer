#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<string.h>

void helpMsg()
{
    printf("usage:\n\ttransferserver [options]");
    printf("\n--------------------------------------------------\n");
    printf("options:");
    printf("\n\t-p\t\tPort (Default: 8888)\n\t-f\t\tFile to serve up (Default: bar.txt)\t-h\t\tShow this help message\n");
}

int main(int argc, char *argv[])
{
 
    /*if(argc == 1)
    {
        printf("\nNo arguments!!!\n");
        exit(1);
    }*/

    FILE *fpw; 

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

    int sock, connected, bytes_recieved , port = 8887, true = 1;  
    char recv_file[1024] = "bar.txt"; 
    char *recv_data = (char *) malloc(sizeof(char) * 1024);
    char *len = (char *) malloc(sizeof(char) * 1024);

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
        port = 8887;
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

    printf("\n I got a connection from (%s , %d)\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

    
    for(i = 1; i < argc; i++)
    {
        if(!strcmp("-f",argv[i])){
           strcpy(recv_file,"");
           strcat(recv_file,argv[i+1]);
           break;
        }
    }

    fpw = fopen(recv_file, "w");

    recv(connected,recv_data,2,0);


    int sizeSent , sizeRcvd = 0 ; sscanf(recv_data, "%d" , &sizeSent);

    printf("\n%s\n",recv_data);
    bzero(recv_data,1024);

    while(sizeSent > sizeRcvd){

        bytes_recieved = recv(connected,recv_data,1024,0);
        printf("%d\n", bytes_recieved);
        
        //recv_data[bytes_recieved] = '\0';
        //sleep(3);
        
        //printf("%zd ", bytes_recieved);
        

        fwrite(recv_data,bytes_recieved,1,fpw);

        fseek(fpw, 0L, SEEK_END);
        sizeRcvd += bytes_recieved;
        bzero(recv_data,1024);

        
    }

    printf("\n RECIEVED DATA = %s\n" , recv_data);
    fflush(stdout);

    close(connected);
    close(sock);
}
