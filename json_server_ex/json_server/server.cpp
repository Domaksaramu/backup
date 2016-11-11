#include <iostream>
#include <mysql++/mysql++.h>
//#include <cstring>
//#include "json/json.h"
#include "Shared_data_access_controller.h"
//#include <pthread.h> 
//#include <thread> thread.push_back thread.join_all
//
#define DB_NAME "world"
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWD "hwoml8075"

void *server_thread_handler(void*){
/// multi_threading 나중에 생각할 것
}

int main(int argc, char** argv){
	//bind
	//listen
	//acept
	Shared_data_access_controller *SDA_controller;
	SDA_controller = new Shared_data_access_controller(DB_NAME,DB_HOST,DB_USER,DB_PASSWD);
	Json::Value temp;
	temp["sql_ID"] = "a1";
	
	Json::StyledWriter writer;
	
	SDA_controller->find_data(temp);	
	std::cout<<writer.write(SDA_controller->stored_data)<<std::endl;

}
