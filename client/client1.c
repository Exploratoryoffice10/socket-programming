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
#define ll long long int


char *buf1,*buf2;
char x[200001],y[200001];
int mm=200000,sock,argc;
char *argsi[1000],*buf=NULL;
size_t bf;
#define PORT 8000
#define DELIM " \t\n"


void ssf(char *inp)  // send single file to client directory
{
    ll fns=(int)strlen(inp);   
    char pss[fns];
    strcpy(pss,inp);
    send(sock,pss,sizeof(pss),0);   // send file name
    bzero(x,22);
    int btd=recv(sock,x,22,0);
    ll saa=atoi(x);
    printf("the buffer x is-> %s\n",x);
    bzero(x,22);    // x for size transport
    if(saa==-1)     // y for file writing
    {
        printf("no file named %s,%s in server\n",inp,pss);
        return;
    }
    int fdf1=open(inp,O_WRONLY | O_CREAT | O_TRUNC,0664);
    if (fdf1==-1)
    {
        printf("failed to open a file\n");
        exit(1);
    }
    
    int asa,quo=saa/mm,rem=saa%mm;
    //printf("rem is %d,quo is %d\n",rem,quo);
    for(int i=0;i<quo;++i)
    {
        asa=recv(sock,y,mm,0);
        //printf(">%s..\n",y);
        write(fdf1,y,asa);
    }
    if(quo>0){
        bzero(y,mm);
    }
    if(rem!=0)
    {
        asa=recv(sock,y,rem,0); //printf(">%s..\n",y);
        write(fdf1,y,rem);
        bzero(y,mm);
    }
    printf("file %s downloaded form server\n",inp);
}


int main()
{
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr)); 
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)  // connect to the server address
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    else
    {
        printf("connected");
    }
    x[0]='1';
    while (1)
    {
        argc=0;
        printf("\nclient>");fflush(stdout);
        if((getline(&buf,&bf,stdin))!=-1)   // input
        {
            argsi[0]=strtok(buf,DELIM); 
            if(argsi[0]==NULL)  
            {
                continue;
            }
            argc++;
            while ((argsi[argc]=strtok(NULL,DELIM))!=NULL)
            {
                argc++;
            }
            if(strcmp(argsi[0],"exit")==0)
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
            else if(strcmp(argsi[0],"get")==0)
            {
                if(argc==1)
                {
                    printf("No files passed \n");
                    continue;
                }
                for (int i = 1; i < argc; i++)
                {
                    ssf(argsi[i]);
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
            exit(1);
        }
    }
    close(sock);
    return 0;
}
