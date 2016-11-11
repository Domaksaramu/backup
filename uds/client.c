#include <sys/types.h>
//#include <sys/stat.h>
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
	struct sockaddr_un serveraddr;
	int  clilen;
	struct data mydata;

	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0); 
	if (sockfd < 0)
	{
		perror("exit : ");
		exit(0);
	}

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, argv[1]);
	clilen = sizeof(serveraddr);

	mydata.a = atoi(argv[2]);
	mydata.b = atoi(argv[3]);
	mydata.sum =0;

	if (sendto(sockfd, (void *)&mydata, sizeof(mydata), 0, 
				(struct sockaddr *)&serveraddr, clilen) < 0)
	{
		perror("send error : ");
		exit(0);
	}
	if (recvfrom(sockfd, (void *)&mydata, sizeof(mydata), 0,
				(struct sockaddr *)&serveraddr, (socklen_t *)&clilen) < 0)
	{
		perror("recv error : ");
		exit(0);
	}
	printf("result is : %d\n", mydata.sum);


	close(sockfd);
	exit(0);
}
