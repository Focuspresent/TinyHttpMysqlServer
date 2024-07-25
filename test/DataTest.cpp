#include <iostream>

#include "../Data/Data.hpp"

using namespace std;
using namespace Data;

int main()
{
    BasicData* data=new Login("47.96.237.24","root","Qwe123456789...++","qianduoduo");
    json j;
    data->Execute({{"userAcc","123"},{"userPwd","456"}},j);
    std::cout<<j<<std::endl;
    data->Execute({{"userAcc","8888888"},{"userPwd","111111"}},j);
    std::cout<<j<<std::endl;
    data->Execute({{"userAc","8888888"},{"userPwd","111111"}},j);
    std::cout<<j<<std::endl;
    data->Execute({{"userAcc","8888888"},{"userPwd","1111"}},j);
    std::cout<<j<<std::endl;
    data->Execute({{"userAcc","1069366295"},{"userPwd","qwe123"}},j);
    std::cout<<j<<std::endl;
    delete data;
    return 0;
}