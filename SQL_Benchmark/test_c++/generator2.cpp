#include <iostream>
#include <fstream>/*for input and output file*/
#include <string.h>/*for string function*/
#include <mysql.h>/*for mysql*/
#include <stdio.h>/*for sprintf*/
#include <stdint.h> /* for uint64 definition */
#include <stdlib.h> /* for exit(), system() definition */
#include <time.h> /* for clock_gettime */


#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWD "hwoml8075"
#define DB_NAME "world_c"
#define DB_PORT 3306

#define BILLION 1000000000L

using namespace std;

char table_names[100][100];
int t_cnt=0;
bool init(MYSQL *mysql);
MYSQL_RES *res;
MYSQL_ROW row;
int fields;
ifstream fin;
ofstream fout;
MYSQL mysql;//MYSQL 변수에 대한 포인터형
char buf[1024];//buffer for Query
int query_stat;//query 결과 flag
char *temp;


int query_execute(MYSQL *mysql, char *buf);
void get_table_info();
MYSQL_RES* get_table_values(int i);
void insert_query(int num);

int main(){
	if(!init(&mysql))//setting MySQL
		return 1;
	fout.open("dummy_data.sql",ios_base::out | ios_base::app);
	get_table_info();

	insert_query(3);
	
	fout.close();	
	mysql_close(&mysql);
	return 0;
}
void insert_query(int num){
	int length;
	char buf[2048];
//	for(k=0;k<num;k++){
		for(int i=0;i<t_cnt;i++){
			get_table_values(i);
			int len = (mysql_num_rows(res));
			cout<<table_names[i]<<len<<endl;
			if(len >0)
			while(row=mysql_fetch_row(res)){
				sprintf(buf,"INSERT INTO %s VALUES (",table_names[i]);
				int j;
				for(j=0;j<fields-1;j++){
					strcat(buf,"'");
					strcat(buf,row[j]);
					strcat(buf,"',");
				}
				strcat(buf,"'");
				strcat(buf,row[j]);
				strcat(buf,"');\0");
				fout<<buf<<endl;
			}
		}
//	}
}
MYSQL_RES* get_table_values(int i){
	sprintf(buf,"select * from  %s;",table_names[i]);//
	query_stat = mysql_query(&mysql,buf);
	res = mysql_store_result(&mysql);
	fields = mysql_num_fields(res);
	return res;
}	

void get_table_info(){
	/*fin.open("origin_data.sql");//Open SQL file
	while(!fin.eof()){
	//for(int i=0;i<20;i++){
		fin.getline(buf,1024);
		if(strlen(buf) && buf[0]!='/' && buf[0]!='-'){
			query_stat = query_execute(&mysql,buf);//execute query
			temp=strtok(buf," ");
			if(!strcmp(temp,"CREATE")){//CREATE query일 경우 table 명을 받아옴
				strtok(NULL," ");
				temp = strtok(NULL," (");
				strcpy(table_names[t_cnt++],temp);
			}
		}	
	}
	fin.close();*/
	sprintf(buf,"show tables;");
	mysql_query(&mysql,buf);
	res=mysql_store_result(&mysql);
	fields = mysql_num_fields(res);
	int i=0;
	while(row=mysql_fetch_row(res))
		strcpy(table_names[i++],row[0]);
	t_cnt = i;
/*	for(i=0;i<t_cnt;i++){
		sprintf(buf,"explain %s;",table_names[i]);//각 테이블에 대한 column명과 key 여부 가져옴
		query_stat = mysql_query(&mysql,buf);
		res = mysql_store_result(&mysql);
		fields = mysql_num_fields(res);
		cout<<table_names[i]<<" "<<fields<<endl;
		while(row=mysql_fetch_row(res)){
			cout<<row[0]<<"	"<<row[3]<<endl;
		}
		cout<<endl;

	}*/
	system("cp -f origin_data.sql dummy_data.sql");

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


