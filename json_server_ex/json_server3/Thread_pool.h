#include "json/json.h"
#include <queue>
#include <pthread.h>
#define THREAD_NUM 10

class Module_controller;

class Thread_pool
{
	public:
		Thread_pool(Json::Value& stored_data, Module_controller *m_controller);
		bool load_request(Json::Value &response);
		void assign_request(Json::Value message);
		static void* thread_handler(void* pArg);
		void init_Threads();
		virtual ~Thread_pool(){};
		
		Json::Value stored_data;
		std::queue<Json::Value> request_queue;
		Module_controller *m_controller;
		pthread_t t_ids[THREAD_NUM];
		static pthread_mutex_t q_lock;
		static pthread_cond_t q_cond;

		void send_response(Json::Value message, Json::Value response);
};
