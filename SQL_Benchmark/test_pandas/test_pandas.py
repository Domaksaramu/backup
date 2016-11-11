import pymysql;
import pandas as pd;
import plotly.plotly as py;
from plotly.graph_objs import *;
py.sign_in('limjewon', '3lqnh1vlbm');

def make_graph(db_name,the_type,line_num):
	conn = pymysql.connect(host="localhost", user="root", passwd="hwoml8075", db=db_name);
	cursor = conn.cursor();

	cursor.execute("select the_type, response_time, the_date from records where the_type='%s',line_num=%d;"%(the_type,line_num));

	rows = cursor.fetchall();
	str(rows)[0:300];

	df = pd.DataFrame( [[ij for ij in i] for i in rows] )
	df.rename(columns={0: 'Type', 1: 'Response_time', 2: 'Date'}, inplace=True);
	df = df.sort(['Date'], ascending=[1]);

	type_names = df['Type']
	#for i in range(len(country_names)):
	#	try:
	#		country_names[i] = str(country_names[i]).decode('utf-8')
	#	except:
	#		country_names[i] = 'Country name decode error'
	 
	trace1 = Scatter(x=df['Response_time'], y=df['Date'],text=type_names, mode='markers')
	layout = Layout(
			title='Database benchmark %s %d'%(db_name,line_num),
			xaxis=XAxis(title='Response_time'), 
			yaxis=YAxis(title='Date'))
	data = Data([trace1])
	fig = Figure(data=data, layout=layout)
	py.iplot(fig, filename='Database benchmark %s %d'%(db_name,line_num))
	return;


make_graph("world_c","insert",5000)
make_graph("world_c","insert",10000)
make_graph("world_c","insert",15000)
make_graph("world_c","insert",20000)
make_graph("world_c","insert",30000)
make_graph("world_c","insert",50000)

make_graph("world_p","insert",5000)
make_graph("world_p","insert",10000)
make_graph("world_p","insert",15000)
make_graph("world_p","insert",20000)
make_graph("world_p","insert",30000)
make_graph("world_p","insert",50000)
