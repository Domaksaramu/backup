all:
	g++ -o test server.cpp Shared_data_access_controller.cpp Database_access_object.cpp jsoncpp.cpp -I/usr/include/mysql -I/usr/include/mysql++ -L/usr/lib64/mysql -L/usr/lib64 -lmysqlpp -lmysqlclient -I. -DJSON_IS_AMALGAMATION

#OBJS = jsoncpp.o Shared_data_access_controller.o Database_access_object.o server.o
#SRCS = jsoncpp.cpp server,cpp Shared_data_access_controller.cpp Database_access_object
#TARGET = wrapping_server

#CXX = g++
#CPPFLAGS = -c
#INC = -I/usr/include/mysql -I/usr/include/mysql++ -I.
#LIBS = -L/usr/lib64/mysql -L/usr/lib64 -lmysqlpp -lmysqlclient 
#DEF = -DJSON_IS_AMALGAMATION

#.SUFFIXES: .cpp .o

#all: $(TARGET)

#$(TARGET): $(OBJS)
#	$(CXX) -o $(TARGET)$(OBJS)$(INC) $(LIBS) $(DEF)

#.c.o:
#	$(CXX) $(CPPFLAGS) $(INC) $(LIBS) $(DEF)

#clean:
#	rm -rf $(OBJS) $(TARGET) core

