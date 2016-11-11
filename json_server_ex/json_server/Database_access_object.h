#include <mysql++/mysql++.h>
#include <cstring>
#include "json/json.h"
#include <pthread.h>
class Database_access_object//database 접근 객체, database 연결 및 접근만 관리
//객체의 관리는 controller에서
{
	public:
		Database_access_object(pthread_t id, Json::Value *stored_data, char* db_name, char* db_host, char* db_user, char* db_passwd);
		Json::Value execute_query(Json::Value sql_info);
		~Database_access_object();	
		pthread_t get_thread_id();
		void store_data(Json::Value json_result);
	private:
		mysqlpp::Connection mysqlpp_conn;

};
