#include <iostream>
#include <pthread.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "json/json.h"
using namespace std;



void *receive_handler(void *arg){
	char *client_path = (char *)arg;
	int client_sockfd;
	struct sockaddr_un clientaddr;
	int	client_len; 
	Json::Value response;
	Json::StyledWriter writer;
	char buf[1024000];
	Json::Reader reader;
	string str;

	client_sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(client_sockfd<0){
		perror("socket error ");
		exit(0);
	}
	unlink(client_path);

	bzero(&clientaddr, sizeof(clientaddr));
	clientaddr.sun_family = AF_UNIX;
	strcpy(clientaddr.sun_path,strtok(client_path,"\0"));
	if(bind(client_sockfd,(struct sockaddr *)&clientaddr,sizeof(clientaddr))<0){
		perror("bind error ");
		exit(0);
	}
	client_len = sizeof(clientaddr);


	while(true){
		cout<<"receive"<<endl;
		if(recvfrom(client_sockfd ,(void *)&buf, sizeof(buf),0,(struct sockaddr *)&clientaddr, (socklen_t *)&client_len)<0){
			perror("receive error ");
			exit(0);
		}
		//parsing
		cout<<buf<<endl;
		memset(buf,0,sizeof(buf));
		//str = buf;
		//reader.parse(str,response);
		//cout<<writer.write(response)<<endl;
	}
	
}

int main(int argc, char **argv){
	char server_path[1024], client_path[1024];
	int server_sockfd;
	struct sockaddr_un serveraddr;
	int server_len;
	Json::Value message;
	Json::StyledWriter writer;

	strcpy(server_path, argv[2]);
	strcpy(client_path, argv[1]);

	pthread_t tid;
	pthread_create(&tid, NULL, receive_handler, (void *)client_path );


	server_sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(server_sockfd<0){
		perror("Exit ");
		exit(0);
	}
	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sun_family = AF_UNIX;
	strcpy(serveraddr.sun_path, strtok(server_path,"\0"));
	server_len = sizeof(serveraddr);

	char buf[1024];
	const char *temp;
	while(true){
		//send
		//cout<<"Input: ";
		cin>>buf;
		if(buf=="quit")
			exit(0);
		message["sql_ID"] = buf;
		message["client"] = argv[1];

		strcpy(buf, writer.write(message).c_str());

		if(sendto(server_sockfd,(void *)&buf, strlen(buf),0,(struct sockaddr *)&serveraddr,server_len)<0){
			perror("send eror ");
			exit(0);
		}
		//cout<<"send message : "<<buf<<endl;

		//cout<<sizeof(buf)<<endl;
	}
	close(server_sockfd);
}
