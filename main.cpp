#include <iostream>

#include "Http/httplib.h"
#include "Data/Data.hpp"

using namespace std;
using namespace httplib;
using namespace Data;

int main()
{
    Server server;
    string url="47.96.237.24";
    string user="root";
    string password="Qwe123456789...++";
    string databasename="qianduoduo";

    server.Post("/userLogin",[&](const Request& req,Response& res){
        BasicData* data=new Login(url,user,password,databasename);
        json res_j;
        data->Execute(json::parse(req.body.c_str()),res_j);
        res.set_content(res_j.dump(),"text/json");
        //res.set_content("tt","text/plain");
        delete data;
    });

    server.Post("/userSignup",[&](const Request& req,Response& res){
        BasicData* data=new SignUp(url,user,password,databasename);
        json res_j;
        data->Execute(json::parse(req.body.c_str()),res_j);
        res.set_content(res_j.dump(),"text/json");
        //res.set_content("tt","text/plain");
        delete data;
    });

    server.listen("0.0.0.0",8085);

    return 0;
}