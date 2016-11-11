#include "json/json.h"
#include <pthread.h>
#define THREAD_NUM 10

class Module_controller;

class thread_control
{
	public:
		thread_control(Json::Value& stored_data, Module_controller *module_controller);
		void create_thread();
		void delete_thread();
		void allocate_data();
		void receive_message(Json::Value message);
		static void *thread_handler(void* pArg);

//		static void _setup(Json::Value& stored_data, Module_controller *module_controller, thread_control *t_control);
		~thread_control();
		struct Thread_args{
			pthread_t tid;
			int thread_index;
			Json::Value allocated_data;
			Json::Value message;
		};
		Json::Value allocation_list;
		Thread_args thread_args[10];
		int thread_cnt;
		int table_cnt;

		Json::Value data;
		Module_controller *module_controller;
};
