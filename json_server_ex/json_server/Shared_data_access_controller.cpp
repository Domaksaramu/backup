#pragma once
#include <iostream>
#include "Shared_data_access_controller.h"


using namespace std;

Shared_data_access_controller::Shared_data_access_controller(const char* name, const char* host, const char* user, const char* passwd)
{
	//////// database 정보
	strcpy(db_name,name);
	strcpy(db_host,host);
	strcpy(db_user,user);
	strcpy(db_passwd,passwd);

	DAO = new Database_access_object(0,&stored_data,db_name,db_host,db_user,db_passwd);

	init_pre_defined_query();
	init_stored_data();
}

Json::Value Shared_data_access_controller::find_data(Json::Value request)
//Stored data에서 찾는 함수
{
	Json::Value result;
	//stored data에서 검색
	if(!stored_data.isNull()){
		if(!stored_data.isNull()){//////수정할것
			int a = 0;///////////수정할것
			Json::Value temp;
			result = temp;
		}
		else{
			result = request_data(request);				
		}
	}
	else{
		result = request_data(request);
	//stored data에 없을 경우 request_data 호출
	}
	return result;
}

Json::Value Shared_data_access_controller::request_data(Json::Value request)
//Stored data에 없는 data 요청하는 함수
{
		
	Json::Value sql_info =  find_pre_defined_query(request);
	
	Json::Value query_result = DAO->execute_query(sql_info);
	Json::StyledWriter writer;
	if(!query_result.isNull())
		stored_data[sql_info["sql_ID"].asString()]=query_result;

	return query_result;
}
Json::Value Shared_data_access_controller::find_pre_defined_query(Json::Value request)
{
	Json::Value result;
	if(!pre_defined_querys[request.asString()].isNull()){
		result["sql_ID"] = request;
	
		result["sql"] = pre_defined_querys[request.asString()];
	}
	return result;
}
void Shared_data_access_controller::init_pre_defined_query()
{
	pre_defined_querys["a1"] = "select * from city limit 2";
	pre_defined_querys["b1"] = "select * from country limit 2";
	pre_defined_querys["c1"] = "select * from countrylanguage limit 2";

}
void Shared_data_access_controller::init_stored_data()
{
	for(Json::ValueIterator itr=pre_defined_querys.begin(); itr != pre_defined_querys.end(); itr++){
		Json::Value request = itr.key().asString();
		request_data(request);				
	}
}
Shared_data_access_controller::~Shared_data_access_controller()
{
//	delete DAO_list;
}

