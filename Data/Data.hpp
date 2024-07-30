#ifndef __DATA_HPP
#define __DATA_HPP

#include <iostream>
#include <memory>
#include <exception>

#include "../MysqlCpp/mysql.hpp"
#define FMT_HEADER_ONLY 
#include "../Fmt/core.h"

bool operator==(const std::string & s1,const char* s2)
{
    if(s1.size()!=strlen(s2)) return false;
    for(int i=0;i<s1.size();i++){
        if(tolower(s1[i])!=tolower(s2[i])) return false; 
    }
    return true;       
}

class Sql
{
public:
    std::string to_string(json& j)
    {
        try{
            std::string action=j["action"].get<std::string>();
            m_table_name=j["table_name"].get<std::string>();
            j.erase("action");
            j.erase("table_name");
            if(action=="insert"){
                return insert_(j);
            }else if(action=="delete"){
                return delete_(j);
            }else if(action=="update"){
                return update_(j);
            }else if(action=="select"){
                return select_(j);
            }else{
                throw "没有匹配的";
            }
        }catch(std::exception& e){
            fprintf(stderr,"%s\n",e.what());
        }
    }

protected:
    std::string insert_(const json& j)
    {
        std::string pre="(",suf="(";
        for(auto& entry: j.items()){
            pre+=entry.key()+", ";
            suf+=entry.value().dump()+", ";
        }
        pre+=")";suf+=")";
        return fmt::format("insert into {} {} values {}",m_table_name,pre,suf);
    }

    std::string delete_(const json& j)
    {
        json con_j=json::parse(j["condition"].get<std::string>());
        std::string con;
        for(auto& entry: con_j.items()){
            con+=entry.key()+"="+entry.value().dump()+" AND";
        }
        for(int i=0;con.size()&&i<4;i++) con.pop_back();
        return fmt::format("delete from {} where {}",m_table_name,con);
    }

    std::string update_(const json& j)
    {
        std::string str,con;
        for(auto& entry: j.items()){
            if(entry.key()=="condition"){
                json con_j=json::parse(entry.value());
                for(auto& e: con_j.items()){
                    con+=e.key()+"="+e.value().dump()+" AND";
                }
            }else{
                str+=entry.key()+" = "+entry.value().dump()+", ";
            }
        }
        for(int i=0;str.size()&&i<2;i++) str.pop_back();
        for(int i=0;con.size()&&i<4;i++) con.pop_back();
        return fmt::format("update {} set {} where {}",m_table_name,str,con);
    }

    std::string select_(const json& j)
    {
        std::string str,con;
        int number=-1;
        for(auto& entry: j.items()){
            if(entry.key()=="condition"){
                json con_j=json::parse(entry.value());
                for(auto& e: con_j.items()){
                    con+=e.key()+"="+e.value().dump()+" AND";
                }
            }else if(entry.key()=="number"){
                number=entry.value().get<std::uint32_t>();
            }else{
                str+=entry.key()+", ";
            }
        }
        for(int i=0;con.size()&&i<4;i++) con.pop_back();
        for(int i=0;str.size()&&i<2;i++) str.pop_back();
        return  fmt::format("select {} from {} {} {}",str,m_table_name,con.size()?"where "+con:"",number>0?"limit "+std::to_string(number):"");
    }

private:
    std::string m_table_name;
};

namespace Data
{
    enum{
        SQL_FAIL=500,
        JSON_FAIL=501
    };
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
            std::cout<<req<<std::endl;
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

    class SignUp:public BasicData
    {
        enum{
            SIGNUP_SUCCESS=200,
            SIGNUP_EXIST=402,
            SIGNUP_EMPTY=403,
        };
    public:
        SignUp(const std::string& url,const std::string& user,const std::string& password,const std::string& databasename)
        :BasicData(url,user,password,databasename)
        {            
        }

        int execute_(const json& req,json& res) override
        {
            res.clear();
            int n;
            res["state"]=SIGNUP_SUCCESS;
            res["message"]="注册成功";
            try{
                std::string acc=req.at("Acc_sign").get<std::string>();
                std::string username=req.at("username").get<std::string>();
                std::string password=req.at("psw_sign").get<std::string>();
                std::string phone_number=req.at("phone_number").get<std::string>();
                //std::cout<<fmt::format("INSERT INTO VALUES({},'{}','{}','{}')",acc,username,password,phone_number)<<std::endl;
                n=m_mysql.Query(fmt::format("INSERT INTO user_accounts(user_id,username,password,phone_number) VALUES({},'{}','{}','{}')",acc,username,password,phone_number));
            }catch(std::exception& e){
                fprintf(stderr,"执行错误\n");
                res["state"]=SIGNUP_EMPTY;
                res["message"]="参数错误 或 解析错误";
                return n;
            }
            if(n){
                if(1062==n){
                    res["state"]=SIGNUP_EXIST;
                    res["message"]="用户已存在";
                }else{
                    res["state"]=SQL_FAIL;
                    res["errno"]=n;
                    res["message"]="执行语句错误";
                }
            }
            return n;
        }

    };

    class DownloadWaterData: public BasicData
    {
        enum{
            DownloadWaterData_SUCCESS=200,
            DownloadWaterData_NOEXIST=404
        };
    public:
        DownloadWaterData(const std::string& url,const std::string& user,const std::string& password,const std::string& databasename)
        :BasicData(url,user,password,databasename)
        {
        }

        int execute_(const json& req,json& res) override
        {
            res.clear();
            int n;
            res["state"]=DownloadWaterData_SUCCESS;
            res["message"]="下载成功";
            try{
                std::string id=req.at("water_data_id").get<std::string>();
                json mysql_res;
                n=m_mysql.Query(fmt::format("SELECT * FROM water_quality_data WHERE water_data_id={}",id),mysql_res);
                if(mysql_res.empty()){
                    res["state"]=DownloadWaterData_NOEXIST;
                    res["message"]="下载失败";
                    return n;
                }
                res["obj"]=mysql_res[0];
            }catch(std::exception& e){
                fprintf(stderr,"执行错误\n");
                res["state"]=JSON_FAIL;
                res["message"]="参数错误 或 解析错误";
                return n;
            }
        }
    };

    class ShowWaterData:public BasicData
    {
        enum{
            ShowWaterData_SUCCESS=200,
            ShowWaterData_ERROR=400,
            ShowWaterData_NOEXIST=404
        };
    public:
        ShowWaterData(const std::string& url,const std::string& user,const std::string& password,const std::string& databasename)
        :BasicData(url,user,password,databasename)
        {
        }

        int execute_(const json& req,json& res) override
        {
            res.clear();
            json mysql_res;
            int n=m_mysql.Query(fmt::format("SELECT * FROM water_quality_data"),mysql_res);
            if(mysql_res.empty()){
                res["state"]=ShowWaterData_NOEXIST;
                res["message"]="未找到";
            }else if(n){
                res["state"]=ShowWaterData_ERROR;
                res["message"]="参数错误";
            }else{
                res["state"]=ShowWaterData_SUCCESS;
                res["message"]="展示数据获取成功";
                res["excel"]=mysql_res;
            }
            return n;
        }
    };
}

#endif