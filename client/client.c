#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8000
#define DELIM " \t\n"
char *argv[100],*buf;
long long mm=200000;
char x[200001],y[200001];
int argc;   // no of arguments
size_t bf;
int cs; // socket

void ssf(char *inp)  // send single file to client directory
{
    int fns=(int)strlen(inp)+1;   
    char *pss=(char *) malloc(fns*sizeof(char));
    strcpy(pss,inp);
    send(cs,pss,strlen(pss),0);   // send file name
    int btd=recv(cs,x,23,0);
    int saa=atoi(x);
    if(saa==-1){
        printf("no %s file in server\n",inp);
        free(pss);
        return;
    }
    int fdf1=open(inp,O_WRONLY | O_CREAT | O_TRUNC,0664);
    if (fdf1==-1)
    {
        printf("failed to open a file\n");
        exit(1);
    }
    int asa,quo=saa/mm,rem=saa%mm;
    //first from quo
    for(int i=0;i<quo;++i)
    {
        asa=recv(cs,y,200000,0);
        send(fdf1,y,asa,0);
    }
    if(rem!=0)
    {
        asa=recv(fdf1,y,rem,0);
        send(fdf1,y,asa,0);
    }
    printf("file %s downloaded form server\n",inp);
}


int main()
{
    struct sockaddr_in servaddr;
    cs=socket(AF_INET,SOCK_STREAM,0); // tcp
    if(cs<0)
    {
        perror("error while creating a socket. couldnt create it\n");
        exit(1);
    }

    memset(&servaddr,'0',sizeof(servaddr));

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        exit(1);
    }

    if (connect(cs, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)  // connect to the server address
    {
        printf("\nConnection Failed \n");
        exit(1);
    }
    else
    {
        printf("Connected to server.\n");
    }
    printf("LETS GET TO BUSINESS\n");    
    while (1)
    {
        argc=0;
        printf("client>");fflush(stdout);
        if((getline(&buf,&bf,stdin))!=-1)
        {
            if((argv[0]=strtok(buf,DELIM))!=NULL)
            {
                continue;
            }
            argc++;
            while ((argv[argc]=strtok(NULL,DELIM))!=NULL)
            {
                argc++;
            }
            if(strcmp(argv[0],"exit")==0)
            {
                if(argc==1)
                {
                    break;
                }
                else
                {
                    printf("exit doesnt have any argumets\n");
                    continue;
                }
            }
            else if(strcmp(argv[0],"get")==0)
            {
                if(argc==1)
                {
                    printf("No files passed \n");
                    continue;
                }
                //ssf(argv[1]);
                for (int i = 1; i < argc; i++)
                {
                    ssf(argv[i]);
                }
                
            }
            else
            {
                printf("no such command\n");
            }
        }
        else
        {
            printf("error while reading input\n");
            return 1;
        }
        printf("\n");
    }
    // after exit command done sending data    
    close(cs);
    return 0;
}
