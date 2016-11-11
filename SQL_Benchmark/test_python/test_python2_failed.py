import pymysql
import sys
import codecs
import datetime
import time
import copy
init_start = time.time()	#프로그램 전체 시작시간 측정


DB_HOST = 'localhost'
DB_USER = 'root'
DB_PASSWD = 'hwoml8075'
DB_NAME = 'world' 
DB_CHARSET = 'utf8'
BILLION = 1000000000

def query_execute(sql):
	error_flag = 0;
	if len(sql)!=1 and sql[0]!='/' and  sql[0]!='-'  :
		curs.execute(sql)	#query 실행
		error_flag = 1;
	return error_flag; #error_flag 출력

############################# Setting MySQL####################################
try:
	conn = pymysql.connect(host=DB_HOST,user=DB_USER,password=DB_PASSWD,charset=DB_CHARSET)
	#mysql connection
except pymysql.Error as e:
	print(str(e))
	sys.exit()	#connection error시 종료

curs = conn.cursor()

#################################################################################3
f = codecs.open("world.sql","r","latin-1")
length = 0

start = time.time()
while True:
	line= f.readline()
	if not line: break
	read_sql = str(line)
	length = len(read_sql)
	if length!=1 and read_sql[0]!='/' and  read_sql[0]!='-'  :
		query_sql = copy.copy(read_sql)
		while read_sql[length-2]!=';':
			line = f.readline()
			read_sql = str(line)
			query_sql += read_sql
			length = len(read_sql)
		print(query_sql)
		error_flag = query_execute(query_sql)	#execute query
		if error_flag!=1:
			end = time.time()	#check end time
			result =int((end - start)*BILLION)
			start=time.time()	#check start time, nano seconds 단위
			#이전 쿼리가 끝난 시점의 record 과정 부터 sql file read, query 실행 까지 비교하기 위해 해당 시점에서 start time 체크
			sql1 = "INSERT INTO records (the_type, response_time, error_flag, c_flag) VALUES('%s',%d,%d,%d);"%(sql.split()[0],result,0,0)	#SQL query for record, query의 type, response time, error 여부, c++인지 python인지 기록
			query_execute(sql1)	#execute record query
conn.close()
f.close()

init_end = time.time()	#프로그램 종료 시간 측정
init_result = int((init_end-init_start)*BILLION)

print(init_result)	#전체 종료 시간 출력
