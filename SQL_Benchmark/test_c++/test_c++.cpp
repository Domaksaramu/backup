#include <iostream>
#include <fstream>/*for input and output file*/
#include <string.h>/*for string function*/
#include <mysql.h>/*for mysql*/
#include <stdio.h>/*for sprintf*/
#include <stdint.h> /* for uint64 definition */
//#include <stdlib.h> /* for exit() definition */
#include <time.h> /* for clock_gettime */


#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWD "hwoml8075"
#define DB_NAME NULL 
#define DB_PORT 3306
#define DB_TABLE "world"

#define BILLION 1000000000L

using namespace std;


bool init(MYSQL *mysql);
void get_response_time(char *buf, MYSQL *mysql);
int main(){
	MYSQL mysql;//MYSQL 변수에 대한 포인터형
	char buf[1024];


///////////////////////////////MySQL Setting///////////////////////////////////////////////
	if(!init(&mysql))
		return 1;
////////////////////////////////////////////////////////////////////////////////////////////
	

	ifstream fin("./world.sql");
	int i = 0;
	while(!fin.eof()){
	//while(i++<20){
		fin.getline(buf,1024);
		if(!strlen(buf) || buf[0]=='/' || buf[0]=='-'){
			continue;//빈 줄이거나 주석일 경우 건너뜀
		}
		else{
			get_response_time(buf,&mysql);
		}	
	}
	fin.close();
	mysql_close(&mysql);
}

bool init(MYSQL *mysql){
	MYSQL *mysql_conn;
	mysql_init(mysql);//객체 초기화
	mysql_conn = mysql_real_connect(mysql, DB_HOST, DB_USER,DB_PASSWD,DB_NAME, DB_PORT, NULL, false);	
	

	if(mysql_conn ==NULL)
	{	
		cout<<mysql_error(mysql)<<endl;
		return false;	
	}
	else{
		//mysql_autocommit(&mysql,0);
		mysql_set_character_set(mysql,"utf8");
	}
	return true;
}


void get_response_time(char* buf, MYSQL *mysql){
	int query_stat;
	char type[10];
	timespec start, end;
	uint64_t diff;
	int error_flag = 0;
	//start
	clock_gettime(CLOCK_REALTIME, &start);
	query_stat=	mysql_query(mysql,buf);
	if(query_stat){
		cout<<mysql_error(mysql)<<endl<<endl;
		error_flag= 1;
	}
	clock_gettime(CLOCK_REALTIME, &end);
	//end

	diff=BILLION*(end.tv_sec - start.tv_sec) +end.tv_nsec - start.tv_nsec;
	strcpy(type,strtok(buf," "));
	sprintf(buf,"INSERT INTO records (the_type, response_time, error_flag, c_flag) VALUES ('%s', %d, %d, %d);",type, diff, error_flag, 1);
	query_stat=mysql_query(mysql,buf);
	if(query_stat)
		cout<<mysql_error(mysql)<<endl<<endl;	
}
