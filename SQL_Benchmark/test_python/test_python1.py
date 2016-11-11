import os
os.system("netstat -ntlp")
import pymysql
import sys
import codecs
import datetime
import time

init_start = time.time()	#프로그램 전체 시작시간 측정


DB_HOST = 'localhost'
DB_USER = 'root'
DB_PASSWD = 'hwoml8075'
DB_NAME = 'world_p' 
DB_CHARSET = 'utf8'
TABLE_NAME = 'dummy_data_p.sql'
BILLION = 1000000000


############################ Query Excute######################################
def query_execute(sql):
	error_flag = 0;
	if len(sql)!=1 and sql[0]!='/' and  sql[0]!='-'  :
		try:
			curs.execute(sql)	#query 실행
			if sql[0]=='s':
				rows = curs.fetchall()
				
		except pymysql.Error as e:
			error_flag = 1;
	return error_flag; #error_flag 출력

############################# Setting MySQL####################################
try:
	conn = pymysql.connect(host=DB_HOST,user=DB_USER,password=DB_PASSWD,db=DBNAME,charset=DB_CHARSET)
	#mysql connection
except pymysql.Error as e:
	print(str(e))
	sys.exit()	#connection error시 종료

curs = conn.cursor()

###############################################################################

def record_time(result, sql):
	sql1 = "INSERT INTO records (the_type, response_time, error_flag, c_flag) VALUES('%s',%d,%d,%d);"%(sql.split()[0],result,0,0)	#SQL query for record, query의 type, response time, error 여부, c++인지 python인지 기록
	query_execute(sql1)	#execute record query
	return;

f = open(TABLE_NAME,"r")
length = 0

start = time.time()
while True:
	sql=f.readline()
	if not sql: break
	#sql = str(line)
	if len(sql)!=1 and sql[0]!='/' and  sql[0]!='-'  :
		error_flag = query_execute(sql)	#execute query
		if error_flag!=1:
			end = time.time()	#check end time
			result =int((end - start)*BILLION)
			start=time.time()	#check start time, nano seconds 단위
			#이전 쿼리가 끝난 시점의 record 과정 부터 sql file read, query 실행 까지 비교하기 위해 해당 시점에서 start time 체크

			record_time(result,sql)
			conn.close()
f.close()

init_end = time.time()	#프로그램 종료 시간 측정
init_result = int((init_end-init_start)*BILLION)

print(init_result)	#전체 종료 시간 출력
