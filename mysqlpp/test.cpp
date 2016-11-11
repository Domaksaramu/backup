#include <mysql++/mysql++.h>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <cstring>
#include "json/json.h"


#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASSWD "hwoml8075"
#define DB_NAME "world"
#define DB_PORT 3306
#define DB_TABLE "city"


using namespace std;

int main(int argc, char *argv[]) {
	mysqlpp::Connection conn(false);

	if (conn.connect(DB_NAME, DB_HOST,
			DB_USER	, DB_PASSWD)) {
		mysqlpp::Query query = conn.query("select * from city");
		mysqlpp::StoreQueryResult res = query.store();
		if (res) {

			Json::Value root;
			Json::Value aa1;
			for (size_t i = 0; i < res.num_rows(); ++i) {

				Json::Value aa;
				aa[0]=(string) res[i][1];
				aa[1]=(string) res[i][2];
			//	aa["num"] = (string) res[i]["num"];
			//	aa["weight"] = (string) res[i]["weight"];
			//	aa["price"] = (string) res[i]["price"];
			//	aa["sdate"] = (string) res[i]["sdate"];

				aa1.append(aa);

			}
			ostringstream convert;
			convert << res.num_rows();

			root["count"] =  convert.str();
			root["data"] = aa1;

			Json::StyledWriter styledWriter;
			cout << styledWriter.write(root);

		} else {
			cerr << "Failed to get stock table: " << query.error() << endl;
			return 1;
		}

		return 0;
	} else {
		cerr << "DB connection failed: " << conn.error() << endl;
		return 1;
	}
}
