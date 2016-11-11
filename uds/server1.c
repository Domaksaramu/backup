#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

struct data
{
	int a;
	int b;
	int sum;
};
int main(int argc, char **argv)
{
	int sockfd;
	int clilen;
	struct data mydata;
	struct sockaddr_un clientaddr, serveraddr;
	printf("%d\n", SOCK_DGRAM);
	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0); 
	
	char path[100];

	strcpy(path,argv[1]);
//	printf("%s\n%d\n",argv[1],strlen(argv[1]));	
	if (sockfd < 0)
	{
		perror("socket error : ");
		exit(0);
	}    
	unlink(argv[1]);

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, path);

	if (bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("bind error : ");
		exit(0);
	}
	clilen  = sizeof(clientaddr); 


	while(1)
	{
		recvfrom(sockfd, (void *)&mydata, sizeof(mydata), 0, (struct sockaddr *)&clientaddr, &clilen); 
		printf("%d + %d = %d\n", mydata.a, mydata.b, mydata.a + mydata.b);    
		sendto(sockfd, (void *)&mydata, sizeof(mydata), 0, (struct sockaddr *)&clientaddr, clilen);
	}
	close(sockfd);
	exit(0);
}
