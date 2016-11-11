#include "thread_control.h"
#include "Module_controller.h"
#include <iostream>
#include <string.h>
using namespace std;
thread_control::thread_control(Json::Value& stored_data, Module_controller *module_controller){
	Json::StyledWriter writer;
	
 	data = stored_data;
	table_cnt = data.size();
	this->module_controller = module_controller;



	allocate_data();
	create_thread();
}
void *thread_control::thread_handler(void *pArg){
	Thread_args *arg = (Thread_args*)pArg;
	Json::StyledWriter writer;
	while(true){
		if(!arg->message.empty()){
			if(arg->thread_index!=9){
				if(!arg->message.isString()){
					cout<<arg->message["sql_ID"].asString()<<endl;
					cout<<writer.write(arg->allocated_data[arg->message["sql_ID"].asString()])<<endl;
					cout<<"number of fields :"<<arg->allocated_data[arg->message["sql_ID"].asString()].size()<<endl;

					arg->message.clear();
				}
				else{
					cout<<"quit!!"<<endl;
					break;
				}
			}
			else{
				Json::Value result;
				if(!arg->message.isString())
				{
					if(true){//arg->message["isAvaliable"].isNull())
					//수정할 것
						if(!arg->allocated_data[arg->message["sql_ID"].asString()].isNull()){
							cout<<arg->message["sql_ID"].asString()<<endl;
							cout<<writer.write(arg->allocated_data[arg->message["sql_ID"].asString()])<<endl;
							arg->message.clear();
						}
						else{
								cout<<arg->message["sql_ID"].asString()<<" No data!!!!"<<endl;
								arg->message.clear();
						}
					}
					else{
						cout<<" No data!!!!"<<endl;
						arg->message.clear();
					}
				}
				else{
					cout<<"quit!!"<<endl;
					break;
				}
			}
		}
	}
}
void thread_control::create_thread(){
	for(int i = 0;i<THREAD_NUM;i++){
		thread_args[i].thread_index=i;
		
		pthread_create(&thread_args[i].tid,NULL,thread_control::thread_handler,(void*)&thread_args[i]);
	//	pthread_join(thread_args[i].tid,NULL);
	}
}

void thread_control::receive_message(Json::Value message){
	int index;
	//cout<<message["sql_ID"].asString()<<endl;
	if(!message.isString()){
		if(!allocation_list[message["sql_ID"].asString()].empty()){
			index = allocation_list[message["sql_ID"].asString()].asInt();
			thread_args[index].message["sql_ID"]= message["sql_ID"];
		//thread_args[index].message["client"]=message["client"];
		}//unexpected sql_ID
		else{
			index = 9;//9th thread is exception thread;
			Json::Value unexpected_data;
		    unexpected_data= module_controller->find_unexpected_data(message["sql_ID"]);
			if(unexpected_data.isBool()){
				thread_args[index].message["sql_ID"]= message["sql_ID"];
				thread_args[index].message["isAvailable"]= false;
			}
			else{
				thread_args[index].allocated_data[message["sql_ID"].asString()]=unexpected_data;
				thread_args[index].message["sql_ID"]= message["sql_ID"];
			}
		}
	}
	else{
		thread_args[index].message = message;
	}
}
void thread_control::allocate_data(){
	Json::Value::Members member = data.getMemberNames();
	Json::Value::Members::iterator it;
	Json::StyledWriter writer;
    int i;	
	for(it=member.begin(), i=0; it!=member.end();it++, i++ ){
		thread_args[i%9].allocated_data[*it] = Json::Value(data[*it]);
		allocation_list[*it]=i%9;
		
//		cout<<*it<<": "<<writer.write(thread_args[i%10].allocated_data[*it])<<endl;
	}
}

void thread_control::delete_thread(){
	for(int i=0;i<THREAD_NUM;i++){
		thread_args[i].message="quit";
	}	
}
thread_control::~thread_control(){
	delete_thread();
}
