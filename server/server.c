#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8000
char *buf1,*buf2;
char x[200001],y[200001];
int mm=200000;

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    struct stat sss;
    int sock, opt=1;
    struct sockaddr_in serv_addr;
    int addrlen=sizeof(serv_addr);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("\n Socket creation error \n");
        exit(1);
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
    {		
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&address,sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    int nsock;
    if ((nsock = accept(sock, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("everything set ready to function\n");
    int aza,rig,szi,retval;
    int remi,quoi,fdf1;
    
    while(1)
    {
        rig=recv(nsock,x,mm,0);
        if(rig==-1){
            printf("failed to receive\n");
            exit(1);
        }
        if(rig==0){
            printf("client closed\n");
            break;
        }
        else
        {
            printf("client asked for file %s\n",x);
            if(access(x,F_OK)==-1){
                y[0]='-';y[1]='1';
                aza=2;
                send(nsock,y,aza,0);
                printf("NO SUCH FILE\n");
                continue;
            }
            else
            {
                fdf1=open(x,O_RDONLY);
                if(fdf1==-1)
                {
                    y[0]='-';y[1]='1';
                    aza=2;
                    send(nsock,y,aza,0);
                    perror("error: ");
                    continue;
                } 
                if(stat(x,&sss))
                {
                    szi=sss.st_size;
                }
                int iota=0,ioti=szi;

                while(ioti>0)
                {
                    ioti/=10;

                    iota++;
                }
                sprintf(y,"%d",iota);
                y[iota]='\0';
                aza=iota;
                send(nsock,y,aza,0);
            }   
        }
        remi=szi%mm;
        quoi=szi/mm;
        for (int i = 0; i < quoi; i++)
        {
            aza=read(fdf1,x,mm);
            send(nsock,y,aza,0);
        }
        if(remi!=0)
        {
            aza=read(fdf1,x,remi);
            send(nsock,y,aza,0);
        }   // sent all data of file
        close(fdf1);   // closing the file descripter   
    }
    return 0;
}


