.PHONY:all
all:httptest datatest

httptest:HttpTest.cpp ../Http/httplib.cc ../JsonCpp/json.hpp
	g++ -o $@  $^ -lpthread

datatest:DataTest.cpp ../JsonCpp/json.hpp ../Data/Data.hpp ../MysqlCpp/mysql.hpp
	g++ -o $@ $^ -lmysqlclient

.PHONY:clean
clean:
	rm -f httptest datatest