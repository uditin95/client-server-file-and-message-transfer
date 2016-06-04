#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void strrev(char *str)
{

    int i,j;
    char ch;

    for(i = strlen(str)-1,j=0 ; i > j ; i--, j++)
    {
        ch = str[i];
        str[i]= str[j];
        str[j] = ch;
    }
}

void helpMsg()
{
    printf("\nusage:\n\techoclient [options]\n-----------------------------------------------------\noptions:\n\t-s\t\tserver (Default: localhost)\n\t-p\t\tPort (Default: 8888)\n\t-f\tOutput filename (Default: foo.txt)\n\t-h\t\tShow this help message\n");
}

int main(int argc, char *argv[])
{
 
    /*if(argc == 1)
    {
        printf("\nNo arguments!!!\n");
        exit(1);
    }*/

    int i,checkH = 0,checkP = 0, port = 8887;

    FILE *fpr;

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
    char send_file[1024] = "foo.txt";

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
        if(!strcmp("-f",argv[i])){
           strcpy(send_file,"");
           strcat(send_file,argv[i+1]);
           break;
        }
    }

    fpr = fopen(send_file, "r");

    if(fpr == NULL){
        printf("\nerror : first file does'nt exist\n");
        exit(1);
    }

    int n ; char buffer[100] = {0}, size[10] = {0};

    fseek(fpr, 0, SEEK_END);
    n = ftell(fpr);
    fseek(fpr,0, SEEK_SET);

    i = 0;
    printf("\n%d\n",n);

    while(n > 0){
        size[i] = '0' + n%10;
        n /= 10;
        i++;
    }

    strrev(size);

    send(sock,size, strlen(size) , 0);

    //bzero(temp, 100);

    fclose(fpr);
     
    FILE *fpr1 = fopen(send_file, "r");
    fseek(fpr1, 0 , SEEK_SET);

    while(!feof(fpr1)){
        fread(buffer, 10 ,1,fpr1);
       // temp[50] = '\0';
        
        send(sock, buffer, strlen(buffer),0);
        printf("%s ",buffer);
        bzero(buffer, 10);        
    } 

    fclose(fpr1);

    close(sock);
}
