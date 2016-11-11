#include <iostream>
#include <mysql++/mysql++.h>
//#include <cstring>
//#include "json/json.h"
#include "Module_controller.h"
#include "Thread_pool.h"
//#include <pthread.h> 
//#include <thread> thread.push_back thread.join_all
//
#define DB_NAME "world"
#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWD "hwoml8075"
using namespace std;
void *server_thread_handler(void*){
/// multi_threading 나중에 생각할 것
}

int main(int argc, char** argv){
	//bind
	//listen
	//acept
	Module_controller *module_controller;
	module_controller = new Module_controller(DB_NAME,DB_HOST,DB_USER,DB_PASSWD);

	Json::Value temp;
	
	Json::StyledWriter writer;
	
	temp[0]["sql_ID"] = "a1";
	temp[1]["sql_ID"] = "a2";
	temp[2]["sql_ID"] = "a3";
	temp[3]["sql_ID"] = "b1";
	temp[4]["sql_ID"] = "b2";
	temp[5]["sql_ID"] = "b3";
	temp[6]["sql_ID"] = "c1";
	temp[7]["sql_ID"] = "c2";
	temp[8]["sql_ID"] = "c3";

	temp[9]["sql_ID"] = "d1";//unexpected
	sleep(1);
	for(int i=7;i<=8;i++){
		module_controller->t_pool->assign_request(temp[i]);
	}
	sleep(1);
	

	sleep(5);
	std::cout<<"quit command"<<std::endl;
	for(int i=0;i<10;i++){
		string quit_str = "quit";	
		//module_controller->t_;	
	}	
	
	sleep(5);
}
