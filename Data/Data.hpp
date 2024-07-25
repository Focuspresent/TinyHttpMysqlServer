#ifndef __DATA_HPP
#define __DATA_HPP

#include <iostream>
#include <memory>

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
    public:
        Login(const std::string& url,const std::string& user,const std::string& password,const std::string& databasename)
        :BasicData(url,user,password,databasename)
        {            
        }

        int execute_(const json& req,json& res) override
        {
            fmt::print("{}\n",1);
        }

    };
}

#endif