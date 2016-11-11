#include <iostream>
#include <mysql.h>
#include <errno.h>
#include <stdio.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWD "hwoml8075"
#define DB_NAME "performance_test_db"
#define DB_PORT 3306
#define DB_TABLE "test"

MYSQL *connection, mysql;//MYSQL 변수에 대한 포인터형
MYSQL_RES *res;
MYSQL_ROW row;
int query_stat;
char query[256];
char dumy[6] = "dumy1";
int fields;

void _insert();
void _select();
void _update();
void _delete();
//void _join();
using namespace std;
int main(){
	mysql_init(&mysql);//객체 초기화
	connection = mysql_real_connect(&mysql, DB_HOST, DB_USER,DB_PASSWD,DB_NAME, DB_PORT, NULL, false);	
// mysql_real_connect(&MYSQL, host, user , password, db, port, unix_socket, client_flag );


	if(connection==NULL)
	{	
		cout<<mysql_error(&mysql)<<endl;
		return 1;	
	}
	else{
		//mysql_autocommit(&mysql,0);
		mysql_set_character_set(connection,"utf8");
		_insert();
		_select();
		_update();
		}
	mysql_close(&mysql);
}
void _insert(){
	for(int i=0;i<10;i++){
		dumy[4] = '0'+i;
		sprintf(query, "insert into %s values "
			"('%d','%s')",DB_TABLE, i,  dumy);
		query_stat = mysql_query(&mysql, query);
		if(query_stat){
			cout<<mysql_error(&mysql)<<endl;
		}
		else{
			cout<<i<<" insert query successed"<<endl;
		}
	}
}
void _select(){
	sprintf(query,"select * from test");
	query_stat = mysql_query(&mysql,query);
	if(query_stat){
		cout<<mysql_error(&mysql)<<endl;
	}
	else{
		res = mysql_store_result(&mysql);
		fields = mysql_num_fields(res);
		while(row = mysql_fetch_row(res)){
			for(int i = 0;i<fields;i++){
				cout<<row[i];
			}
			cout<<endl;
		}
	}
}
void _update(){
	for(int i=0;i<10;i++){
		dumy[4] = '0'+i;
		sprintf(query, "update %s "
				"set num= %d "
				"where num = %d",DB_TABLE, i+10,  i);
		query_stat = mysql_query(&mysql, query);
		if(query_stat){
			cout<<mysql_error(&mysql)<<endl;
		}
		else{
			cout<<i<<"update query successed"<<endl;
		}
	}
}
void _delete(){
		sprintf(query,"delete from test where i = 0");
		query_stat = mysql_query(&mysql,query);

//		for(int i=0;i<10;i++){
//			sprintf(query, "delete from test where num=%d",i);
//			query_stat = mysql_query(&mysql,query);
//			if(query_stat)
//				cout<<mysql_error(&mysql)<<endl;
//			else
//				cout<<"Deleting successed"<<endl;
//		}
	
}
//void _join(){}
