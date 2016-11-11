#pragma once
#include "Database_access_object.h"
#include <iostream>

using namespace std;
Database_access_object::Database_access_object (pthread_t id, Json::Value *stored_data, char* db_name, char* db_host, char* db_user, char* db_passwd)
{
	if(!mysqlpp_conn.connect(db_name, db_host, db_user, db_passwd)){
		std::cout<<"Cannot access Database"<<std::endl;
	}
}
Json::Value Database_access_object::execute_query(Json::Value sql_info)
{
	string sql = sql_info["sql"].asString();
	mysqlpp::Query query =  mysqlpp_conn.query(sql);
	mysqlpp::StoreQueryResult res = query.store();


	Json::Value res_root;
	Json::Value res_data;
	mysqlpp::Field field;
	Json::Value row;
	size_t num_rows, num_fields;
	if(res){
		num_rows = res.num_rows();
		num_fields = res.num_fields();
		
		for(size_t i=0;i<num_fields;i++){
			field= res.fetch_field();
			res_root["field"].append(field.name());
		}
		for(size_t j= 0; j<num_rows;j++){
			for(int k = 0;k<num_fields;k++){
				row[res_root["field"][k].asString()] = (string)res[j][k];
			}

			res_data.append(row);
		}
		res_root["data"] = res_data;
		
		//stored_data에 저장
		//stored_ptr[sql_info["sql_ID"].asString] = res_root;
	//	stored_ptr->append(res_root);
	}
	return res_root;	
}
Database_access_object::~Database_access_object(){
	mysqlpp_conn.disconnect();
}
//void Database_access_object::store_data(Json::Value json_result){
//}
