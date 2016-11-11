#include <iostream>)
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

timespec init_start, init_end;
uint64_t init_result;
bool init(MYSQL *mysql);
int query_execute(MYSQL *mysql, char *buf);
int len;
int main(){
	clock_gettime(CLOCK_REALTIME,&init_start);//프로그램 시작 시간 측정
	MYSQL mysql;//MYSQL 변수에 대한 포인터형
	char read_buf[1024];//read buffer
	char query_buf[2048];//query buffer
	int query_stat;//query 결과 flag
	char type[10];//query type 저장 문자열
	timespec start, end;//start, end time
	uint64_t response_time;//query responese time

	if(!init(&mysql))//setting MySQL
		return 1;

	ifstream fin("./world1.sql");//Open SQL file

	while(!fin.eof()){
		fin.getline(read_buf,1024);
		if((len = strlen(read_buf)) && read_buf[0]!='/' && read_buf[0]!='-'){
			sprintf(query_buf,read_buf);
			while(read_buf[len-1]!=';'){
				fin.getline(read_buf,1024);
				strcat(query_buf,read_buf);
				len = strlen(read_buf);
			}
			query_stat = query_execute(&mysql,query_buf);//execute query
			if(!query_stat){
				clock_gettime(CLOCK_REALTIME,&end);//check end time
				response_time=BILLION*(end.tv_sec - start.tv_sec) +end.tv_nsec - start.tv_nsec;
				clock_gettime(CLOCK_REALTIME,&start);//check start time, nano second단위
			   //이전 쿼리가 끝난 시점의 record 과정 부터 sql file read, query 실행 까지 비교하기 위해 해당 시점에서 start time 체크
				strcpy(type,strtok(query_buf," "));
				sprintf(query_buf,"INSERT INTO records (the_type, response_time, error_flag, c_flag) VALUES ('%s', %d, %d, %d);",type, response_time, query_stat, 1);
				//SQL query for record, query의 type, response time, error 여부, c++인지 python인지 기록
				query_stat=query_execute(&mysql,query_buf);//execute record query
			}
		}	
	}
	fin.close();
	mysql_close(&mysql);

	clock_gettime(CLOCK_REALTIME,&init_end);//프로그램 종료 시간 측정

	init_result = BILLION*(init_end.tv_sec - init_start.tv_sec) + init_end.tv_nsec - init_start.tv_nsec;
	cout<<init_result<<endl;//프로그램 종료 시간 출력

	return 0;
}

bool init(MYSQL *mysql){
	MYSQL *mysql_conn;
	mysql_init(mysql);//객체 초기화
	mysql_conn = mysql_real_connect(mysql, DB_HOST, DB_USER,DB_PASSWD,DB_NAME, DB_PORT, NULL, false);//mysql connection
	if(mysql_conn ==NULL)
	{	
		cout<<mysql_error(mysql)<<endl;
		return false;	
	}
	else{
		mysql_set_character_set(mysql,"utf8");
	}
	return true;
}

int query_execute(MYSQL *mysql, char *buf){
	int query_stat;
	char type[10];
	query_stat=	mysql_query(mysql,buf);//query 실행
	return query_stat;	//error 결과 출
}


