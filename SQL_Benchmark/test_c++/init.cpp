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
#define DB_PORT 3306
#define DB_NAME_C "world_c" 
#define DB_NAME_P "world_p"
#define TABLE_ORI_C "origin_data_c.sql"
#define TABLE_ORI_P "origin_data_p.sql"
#define TABLE_TRG_C "dummy_data_c.sql"
#define TABLE_TRG_P "dummy_data_p.sql"
#define BILLION 1000000000L
#define DBNAME "world"

using namespace std;

int insert_num = 1;
int t_cnt=0;
MYSQL_RES *res;
MYSQL_ROW row;
int fields;
ifstream fin;
ofstream fout;
MYSQL mysql;//MYSQL 변수에 대한 포인터형
char buf[2048];//buffer for Query
int query_stat;//query 결과 flag
char *temp;
int line_num=0;

bool init(MYSQL *mysql,const char* DB_NAME);
int query_execute(MYSQL *mysql, char *buf);
void get_table_info(const char* ori);
MYSQL_RES* get_table_values(int i);
void insert_query(int num, int table_num);
void select_join_query(int num, int key1, int key2);
void select_query(int num, int key);

class table_info{
public:
	char table_name[100];
	char column_name[50][100];
	int key[50];
	char column_type[50][100];
	int c_cnt;
	table_info(){
		c_cnt=0;
		memset(key,false,sizeof(key));
//		memset(table_name,0,sizeof(table_name));
	}
	void insert_column_info(char* col, bool k, char *t){
		strcpy(column_name[c_cnt],col);
		key[c_cnt]=k;
	strcat(column_type[c_cnt],t);
		c_cnt++;
	}
	void insert_table_name(char* t){
		strcpy(table_name,t);
	}
};

table_info *t_info;

void generate_dummy_data(const char* db_name, const char* ori, const char* trg);

int main(){
	cout<<"반복 횟수: ";
	cin>>insert_num;

/*	generate_dummy_data(DBNAME, TABLE_ORI_P,TABLE_TRG_P);
	cout<<endl<<endl;
	generate_dummy_data(DBNAME, TABLE_ORI_C,TABLE_TRG_C);*/
	generate_dummy_data(DB_NAME_P, TABLE_ORI_P,TABLE_TRG_P);
	    cout<<endl<<endl;
    generate_dummy_data(DB_NAME_C, TABLE_ORI_C,TABLE_TRG_C);
	
	return 0;
}


void generate_dummy_data(const char* db_name, const char* ori, const char* trg){
	t_info = new table_info[10];
	if(!init(&mysql,db_name))
		return;
	get_table_info(ori);
	//sprintf(buf,"cp %s %s -f",ori,trg);
	//system(buf);
	//fout.open(trg,ios_base::ate|ios::in | ios::out);
/*	fout.open(trg,ios_base::trunc);
	int i;
	int total_num = insert_num*3;
	cout<<total_num;
	fout<<total_num<<endl;
//	for(i=0;i<t_cnt;i++){
//		insert_query(insert_num,i);
		//cout<<t_info[i].table_name<<endl;
		//for(int k=0;k<t_info[i].c_cnt;k++){
		//	cout<<t_info[i].column_name[k]<<"	";
		//}
		//cout<<endl;
//	}   
	select_query(insert_num,true);//3
//	select_query(20,false);
//	select_join_query(80,true,true);
	fout.close();   
	mysql_close(&mysql);*/
	delete []t_info;
}

MYSQL_RES* get_table_values(int i){
	sprintf(buf,"select * from  %s;",t_info[i].table_name);//
	query_stat = mysql_query(&mysql,buf);
	res = mysql_store_result(&mysql);
	fields = mysql_num_fields(res);
	return res;
}	

void get_table_info(const char* ori){
	
	fin.open(ori);//Open SQL file
	while(!fin.eof()){
		fin.getline(buf,1024);
		if(strlen(buf) && buf[0]!='/' && buf[0]!='-'){
			query_stat = query_execute(&mysql,buf);//execute query
		}	
	}
	fin.close();/*
	sprintf(buf,"show tables;");
	mysql_query(&mysql,buf);
	res=mysql_store_result(&mysql);
	fields = mysql_num_fields(res);
	int i=0;
	while(row=mysql_fetch_row(res)){
		if(strcmp(row[0],"records"))
			t_info[i++].insert_table_name(row[0]);
	}
	t_cnt = i;
	for(i=0;i<t_cnt;i++){
		sprintf(buf,"explain %s;",t_info[i].table_name);//각 테이블에 대한 column명과 key 여부 가져옴
		query_stat = mysql_query(&mysql,buf);
		res = mysql_store_result(&mysql);
		fields = mysql_num_fields(res);
		int j=0;
		while(row=mysql_fetch_row(res)){
			t_info[i].insert_column_info(row[0],strlen(row[3]),strtok(row[1],"("));
		//cout<<t_info[i].table_name<<" ";
			//cout<<t_info[i].column_name[j]<<" "<<t_info[i].key[j]<<endl;
			j++;
		}
	}*/
}


bool init(MYSQL *mysql, const char* DB_NAME){
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
void select_join_query(int num, int key1, int key2){
	int i, j, k ,l, z;
	table_info *t_a, *t_b;
	for(z=0;z<num;z++)
	for(i=0;i<t_cnt-1;i++){
		t_a = &t_info[i];
		for(j=0;j<t_a->c_cnt ;j++){
			if(t_a->key[j]==key1)
			for(k=i+1;k<t_cnt;k++){
				t_b = &t_info[(i+k)%t_cnt];
				for(l=0;l<t_b->c_cnt;l++){		
					if(t_b->key[l]==key2){
					sprintf(buf,"select a.%s, b.%s from %s as a join %s as b on a.%s = b.%s;",t_a->column_name[j],t_b->column_name[l],t_a->table_name, t_b->table_name,t_a->column_name[j],t_b->column_name[l]);
					//cout<<buf<<endl;
					fout<<buf<<endl;
					line_num++;
					}
				}
			}
		}
	}

}

void select_query(int num, int key){
	int i,j,k;
	for(i=0;i<num;i++)
	for(j=0;j<t_cnt;j++){
		for(k=0;k<t_info[j].c_cnt;k++){
			if(t_info[j].key[k]==key){
				sprintf(buf,"select * from %s group by %s;",t_info[j].table_name, t_info[j].column_name[k]);
				//cout<<buf<<endl;
				fout<<buf<<endl;
				line_num++;
			}
		}
	}
	
}

void insert_query(int num, int table_num){
	int len;
	int i, j, k;
	char temp_buf[1024];
	for(k=0;k<num;k++){
		i = table_num;
		res = get_table_values(i);
		len = mysql_num_rows(res);
		//cout<<t_info[i].table_name<<len<<" "<<fields<<endl;
		//num*=(len<3000)?5:1;
		//cout<<len<<endl;
		if(len >0){
			while(row=mysql_fetch_row(res)){
				memset(buf,0,1024);
				sprintf(buf,"INSERT INTO %s VALUES (",t_info[i].table_name);

			for(j=0;j<fields-1;j++){
				if(row[j]==NULL)
					sprintf(temp_buf,"%s,","NULL");
				else if(!strcmp(t_info[i].column_type[j],"int")||!strcmp(t_info[i].column_type[j],"smallint") || !strcmp(t_info[i].column_type[j],"float")||!strcmp(t_info[i].column_type[j],"double"))
					sprintf(temp_buf,"%s,",row[j]);
				else
					sprintf(temp_buf,"'%s',",row[j]);
				strcat(buf,temp_buf);
			}
			sprintf(temp_buf,"'%s');",row[j]);
			strcat(buf,temp_buf);
			fout<<buf<<endl;
			line_num++;
			}
		}
	}
}
int query_execute(MYSQL *mysql, char *buf){
	int query_stat;
	char type[10];
	query_stat=	mysql_query(mysql,buf);//query 실행
	if(query_stat)
			cout<<buf<<endl<<mysql_error(mysql)<<endl;
	return query_stat;	//error 결과 출
}


