#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int ac , char **av)
{
	if(ac < 2)
	{
		write(2,"port needed ! ",14);
		return 1;
	}
	int sockfd , newsockfd,portno , n;
	char buffer[255];
	struct sockaddr_in serv_addr,cli_addr;
	socklen_t clilen;
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		write(2,"socket failed ! ",16);
		return 1;
	}
	bzero((char*)&serv_addr,sizeof(serv_addr));
	portno = atoi(av[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if(bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
	{
		write(2,"bind failed ! ",14);
		return 1;
	}
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	if((newsockfd = accept(sockfd,(struct sockaddr*)&cli_addr,&clilen)) < 0)
	{
		write(2,"accept failed ! ",16);
		return 1;
	}
	while(1)
	{
		bzero(buffer,255);
		n = read(newsockfd,buffer,255);
		if(n < 0)
		{
			write(2,"read failed ! ",14);
			return 1;
		}
		printf("Client : %s\n",buffer);
		bzero(buffer,255);
		fgets(buffer,255,stdin);
		n = write(newsockfd,buffer,strlen(buffer));
		if(n < 0)
		{
			write(2,"write failed ! ",15);
			return 1;
		}
		if(strncmp("exit",buffer,3) == 0)
			break;
	}
	close(newsockfd);
	close(sockfd);
	return 0;

}
