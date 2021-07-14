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


int main()
{ 
    int server_fd, new_socket, valread;
    struct sockaddr_in address;  
    int opt = 1;
    int addrlen = sizeof(address);
    struct stat sss;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)  
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;   
    address.sin_addr.s_addr = INADDR_ANY;  
    address.sin_port = htons( PORT );    

    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) 
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    int aza,rig,szi;
    int remi,quoi,fdf;
    int iota,ioti;
    while(1)
    {
        bzero(x,mm);
        rig=recv(new_socket,x,mm,0);
        if(rig==-1)
        {
            printf("failed to receive\n");
            exit(1);
        }
        if(rig==0)
        {
            printf("client closed\n");
            break;
        }
        else
        {
            printf("client asked for file %s\n",x);
            fflush(stdout);
            if(access(x,F_OK)==-1)
            {
                sprintf(y,"-1");
                aza=22;
                for (int i = 2; i < 22; i++)
                {
                    y[i]='.';
                }
                send(new_socket,y,aza,0);
                printf("filedet->%s\n",y);
                printf("NO SUCH FILE\n");
                bzero(y,mm);
                continue;
            }
            else
            {
                fdf=open(x,O_RDONLY);
                if(fdf==-1)
                {
                    sprintf(y,"-1");    
                    aza=22;
                    for (int i = 2; i < 22; i++)
                    {
                        y[i]='.';
                    }
                    send(new_socket,y,aza,0);
                    perror("error: ");
                    bzero(y,mm);
                    continue;
                } 
                if(stat(x,&sss)==0)
                {
                    szi=sss.st_size;
                }
                printf("the file is available and its size is %d\n",szi);
                iota=0;ioti=szi;
                while(ioti>0)
                {
                    ioti/=10;
                    iota++;
                }
                
                sprintf(y,"%d",szi);
                for (int i = iota; i < 22; i++)
                {
                    y[i]='.';
                }
                printf("filedet->%s\n",y);
                aza=22;
                send(new_socket,y,aza,0);
            }   
        }
        remi=szi%mm;
        quoi=szi/mm;
        bzero(x,mm);
        bzero(y,mm);
        for (int i=0; i < quoi; i++)
        {
            aza=read(fdf,x,mm); // file to x to sock
            send(new_socket,x,aza,0);
        }
        if(quoi)
        {
            bzero(x,mm);
        }
        if(remi!=0)
        {
            aza=read(fdf,x,remi);   
            printf("sending the part...<-(%s)->\n",x);
            send(new_socket,x,remi,0);
            bzero(x,mm);
        }   // sent all data of file
        close(fdf);   // closing the file descripter           
    }   
    return 0;
}