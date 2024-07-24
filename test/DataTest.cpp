#include <iostream>

#include "../Data/Data.hpp"

using namespace std;
using namespace Data;

int main()
{
    Data::Data* data=new Mission("47.96.237.24","root","Qwe123456789...++","qianduoduo");
    data->Show();
    std::cout<<data->To_Json()<<std::endl;
    delete data;
    return 0;
}