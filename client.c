#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>

int main(int ac , char **av)
{
    if(ac < 3)
    {
        write(2,"port and hostname needed ! ",27);
        return 1;
    }
    int sockfd,portno , n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[255];
    portno = atoi(av[2]);
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        write(2,"socket failed ! ",16);
        return 1;
    }
    server = gethostbyname(av[1]);
    if(server == NULL)
    {
        write(2,"no such host ! ",15);
        return 1;
    }
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        write(2,"connect failed ! ",17);
        return 1;
    }
    while(1)
    {
        bzero(buffer,255);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if(n < 0)
        {
            write(2,"write failed ! ",15);
            return 1;
        }
        bzero(buffer,255);
        n = read(sockfd,buffer,255);
        if(n < 0)
        {
            write(2,"read failed ! ",14);
            return 1;
        }
        printf("Server : %s\n",buffer);
        if(strncmp("exit",buffer,3) == 0)
        {
            break;
        }
    }
    close(sockfd);
    return 0;
}