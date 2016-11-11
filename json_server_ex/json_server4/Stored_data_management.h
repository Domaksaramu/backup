#include "json/json.h"

class Stored_data_management{
	public:
		Stored_data_management();
		void insert_data(Json::Value data);
		Json::Value get_data(Json::Value sql_id);



		~Stored_data_management();
	private:


};
