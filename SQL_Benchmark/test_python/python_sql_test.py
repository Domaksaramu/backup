import pymysql


def select_query(curs):
	sql = "select * from test"
	curs.execute(sql)
	rows = curs.fetchall()
	num = len(rows)
	for i in range(0,num):
		print(rows[i])

	return 0;

def insert_query(curs, conn):
	sql = "insert into test values (%s,%s)"
	for i in range(0,10):
		temp = curs.execute(sql,(i,'dumy'))
##		conn.commit()
		print(temp)
	return 0;		

def delete_query(curs,conn):
	sql = "delete from test where num=0"
	temp = curs.execute(sql)
##	conn.commit()
	print(temp)

	return 0;

db_host = 'localhost'
db_user = 'root'
db_passwd = 'hwoml8075'
db_name = 'performance_test_db' 
db_char = 'utf8'

conn = pymysql.connect(host=db_host, user=db_user, password=db_passwd,   db=db_name, charset=db_char)
curs = conn.cursor()



#sql = "delete from test"

insert_query(curs,conn)
delete_query(curs,conn)
select_query(curs)

#print('all done')
conn.close()



