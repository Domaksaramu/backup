#include <iostream>
#include <fstream>/*for input and output file*/
#include <string.h>/*for string function*/
#include <mysql.h>/*for mysql*/
#include <stdio.h>/*for sprintf*/
#include <stdint.h> /* for uint64 definition */
#include <stdlib.h> /* for exit() definition */
#include <time.h> /* for clock_gettime */


#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWD "hwoml8075"
#define DB_NAME "world_c"
#define DB_PORT 3306
#define TABLE_NAME "dummy_data_c.sql"
#define BILLION 1000000000L

using namespace std;
int line_num;
timespec init_start, init_end;
uint64_t init_result;
bool init(MYSQL *mysql);
int query_execute(MYSQL *mysql, char *buf);
void record_time(MYSQL *mysql,uint64_t responsetime,char *type,int query_stat);
int main(){
	clock_gettime(CLOCK_REALTIME,&init_start);//프로그램 시작 시간 측정
	MYSQL mysql;//MYSQL 변수에 대한 포인터형
	char buf[1024];//buffer for Query
	int query_stat;//query 결과 flag
	char type[10];//query type 저장 문자열
	timespec start, end;//start, end time
	uint64_t response_time;//query responese time
	char *temp;
	int key[2];
	int z;
	if(!init(&mysql))//setting MySQL
		return 1;
	clock_gettime(CLOCK_REALTIME,&start);
	ifstream fin(TABLE_NAME);//Open SQL file
	fin.getline(buf,1024);
	line_num=atoi(buf);
	cout<<line_num<<endl;
	while(!fin.eof()){
		fin.getline(buf,1024);
		if(strlen(buf) && buf[0]!='/' && buf[0]!='-'){
			query_stat = query_execute(&mysql,buf);//execute query
			if(!query_stat){
				clock_gettime(CLOCK_REALTIME,&end);//check end time
				response_time=BILLION*(end.tv_sec - start.tv_sec) +end.tv_nsec - start.tv_nsec;
				clock_gettime(CLOCK_REALTIME,&start);//check start time, nano second단위
			   //이전 쿼리가 끝난 시점의 record 과정 부터 sql file read, query 실행 까지 비교하기 위해 해당 시점에서 start time 체크
				strcpy(type,strtok(buf," "));
				record_time(&mysql, response_time, type, query_stat);
			}
		}	
	}
	fin.close();
	mysql_close(&mysql);

	clock_gettime(CLOCK_REALTIME,&init_end);//프로그램 종료 시간 측정

	init_result = BILLION*(init_end.tv_sec - init_start.tv_sec) + init_end.tv_nsec - init_start.tv_nsec;
	double i_r = init_result/1000000.0;
	cout<<i_r<<endl;//프로그램 종료 시간 출력
	return 0;
}
void record_time(MYSQL *mysql,uint64_t response_time,char *type, int query_stat){
	char buf[1024];
	//response_time*=(response_time<0)?-1:1;
	//cout<<response_time<<endl;
	double r = response_time/1000000.0;
	sprintf(buf,"INSERT INTO records (the_type, response_time, error_flag, c_flag, line_num) VALUES ('%s', %f, %d, %d, %d);",type, r, query_stat, 1,line_num);
	//SQL query for record, query의 type, response time, error 여부, c++인지 python인지 기록
	query_execute(mysql,buf);//execute record query
}/////////////
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
	MYSQL_RES *res;
	MYSQL_ROW row;
	char type[10];
	query_stat=	mysql_query(mysql,buf);//query 실행
	if(query_stat)
		cout<<buf<<endl<<mysql_error(mysql)<<endl;
	else if(buf[0]=='s'){
		res = mysql_store_result(mysql);
		while(row=mysql_fetch_row(res));//////////////////
	//	mysql_free_result(res);
	}

	
	return query_stat;	//error 결과 출
}


