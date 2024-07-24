.PHONY:all
all:httptest

httptest:HttpTest.cpp ../Http/httplib.cc ../JsonCpp/json.hpp
	g++ -o $@  $^ -lpthread

.PHONY:clean
clean:
	rm -f httptest