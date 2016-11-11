#include "json/json.h"
#include <cstring>
#include "Database_access_object.h"
class Shared_data_access_controller
{
	public:
		//////////////////functions
		Shared_data_access_controller(const char* name,const char* host, const char* user, const char* passwd);
		//stored_data 인자로 추가
		
		Json::Value find_data(Json::Value request);
		//Stored data에서 찾는 함수
		void init_stored_data(); 

		Json::Value request_data(Json::Value not_found);
		//Stored data에 없는 data 요청하는 함수

		
		Json::Value find_pre_defined_query(Json::Value request);
		void init_pre_defined_query();

		~Shared_data_access_controller();

		
		////////////////variables
		char db_name[100];
		char db_host[100];
		char db_user[100];
		char db_passwd[100];

		int request_queue;
		int not_found_buffer;
		int client_list;
		Json::Value pre_defined_querys; 
		Json::Value stored_data;
		Database_access_object *DAO;
};
