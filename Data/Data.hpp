#ifndef __DATA_HPP
#define __DATA_HPP

#include <iostream>
#include <memory>
#include <exception>

#include "../MysqlCpp/mysql.hpp"
#define FMT_HEADER_ONLY 
#include "../Fmt/core.h"

namespace Data
{
    class BasicData
    {
    public:
        BasicData(const std::string& url,const std::string& user,const std::string& password,const std::string& databasename)
        :m_mysql(url,user,password,databasename)
        {
        }
        virtual ~BasicData(){}

        int Execute(const json& req,json& res)
        {
            return execute_(std::move(req),res);
        }

    protected:
        virtual int execute_(const json& req,json& res)=0;
        
        Mysql m_mysql;
    private:
        std::string m_url;
        std::string m_user;
        std::string m_password;
        std::string m_databasename;
    };

    class Login: public BasicData
    {   
        enum{
            LOGIN_SUCCESS=200,
            LOGIN_FAIL=400
        };
    public:
        Login(const std::string& url,const std::string& user,const std::string& password,const std::string& databasename)
        :BasicData(url,user,password,databasename)
        {            
        }

        int execute_(const json& req,json& res) override
        {
            //fmt::print("{}\n",1);
            res.clear();
            bool check=false;
            int n;
            res["state"]=LOGIN_FAIL;
            res["message"]="登录失败";
            try{
                std::string acc=req.at("userAcc").get<std::string>();
                std::string pwd=req.at("userPwd").get<std::string>();
                json mysql_res;
                n=m_mysql.Query(fmt::format("SELECT * FROM user_accounts WHERE user_id={}",acc),mysql_res);
                //std::cout<<mysql_res[0].at("password").get<std::string>()<<std::endl;
                check=pwd==mysql_res[0].at("password").get<std::string>();
            }catch(std::exception &e){
                fprintf(stderr,"执行错误\n");
            }
            if(check){
                res["state"]=LOGIN_SUCCESS;
                res["message"]="登录成功";
            }
            return n;
        }

    };
}

#endif