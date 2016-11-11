import pymysql
import sys
import codecs
import datetime
import time
DB_HOST = 'localhost'
DB_USER = 'root'
DB_PASSWD = 'hwoml8075'
DB_NAME = 'world' 
DB_CHARSET = 'utf8'

############################# MySQL Setting ####################################

try:
	conn = pymysql.connect(host=DB_HOST,user=DB_USER,password=DB_PASSWD,charset=DB_CHARSET)
except pymysql.Error as e:
	print(str(e))
	sys.exit()	

curs = conn.cursor()

#################################################################################3
f = codecs.open("world.sql","r","latin-1")
length = 0

while True:
	try:
		line= f.readline()
		if not line: break
		sql = str(line)
		##print(len(sql))
		if len(sql)!=1 and sql[0]!='/' and  sql[0]!='-'  :
			start = time.time()
			curs.execute(sql)
			end = time.time()
			result =int((end - start)*1000000000.0)
			print(int(result))
			##print(sql)
			try:
				sql1 = "INSERT INTO records (the_type, response_time, error_flag, c_flag) VALUES('%s',%d,%d,%d);"%(sql.split()[0],result,0,0) 
				curs.execute(sql1)
			except pymysql.Error as e:
				print(sql1)
				print(str(e))
				continue
				
	except UnicodeDecodeError as e:
		print("error")
		break;		
conn.close()

