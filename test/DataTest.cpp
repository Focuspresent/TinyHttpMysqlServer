#include <iostream>

#include "../Data/Data.hpp"

using namespace std;
using namespace Data;

int main()
{
    BasicData* data=new Login("47.96.237.24","root","Qwe123456789...++","qianduoduo");
    json j;
    data->Execute({},j);
    return 0;
}