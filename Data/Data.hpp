#ifndef __DATA_HPP
#define __DATA_HPP

#include <iostream>
#include <mysql/mysql.h>
#include <memory>

#include "../JsonCpp/json.hpp"
using json=nlohmann::json;


namespace Data
{
class Data
{
public:
    Data(const std::string& url,const std::string& user,const std::string& password,const std::string& databasename)
    :m_url(url),m_user(user),m_password(password),m_databasename(databasename)
    {
        mysql_init(&m_conn);
        if(mysql_real_connect(&m_conn,m_url.c_str(),m_user.c_str(),m_password.c_str(),m_databasename.c_str(),0,nullptr,0)==NULL){
            perror("connect mysql falied");
            mysql_close(&m_conn);
            exit(-1);
        }
    }
    
    virtual ~Data()
    {
        mysql_close(&m_conn);
        printf("close mysql connect\n");
    }
    
    /**
     * @brief 对某个表整体展示
     */
    void Show()
    {
        show_();
    }
    
    /**
     * @brief 对某个表执行sql插入语句
     * 
     * @return 是否成功
     * @returns bool
     */
    bool Insert(const std::string& sql)
    {
        return insert_(std::move(sql));
    }
    
    /**
     * @brief 对某个表执行sql删除语句
     * 
     * @return 是否成功
     * @returns bool
     */
    bool Delete(const std::string& sql)
    {
        return delete_(std::move(sql));
    }

    /**
     * @brief 对某个表执行sql更改语句
     * 
     * @return 是否成功
     * @returns bool
     */
    bool Change(const std::string& sql)
    {
        return change_(std::move(sql));
    }

    /**
     * @brief 对某个表执行sql查询语句
     * 
     * @return 是否成功
     * @returns bool
     */
    bool Query(const std::string& sql)
    {
        return query_(std::move(sql));
    }

protected:
    //总体展示某个表的数据
    virtual void show_(){}
    //增加某个表数据
    virtual bool insert_(const std::string& sql){}
    //删除某个表数据
    virtual bool delete_(const std::string& sql){}
    //改动某个表数据
    virtual bool change_(const std::string& sql){}
    //查询某个表数据
    virtual bool query_(const std::string& sql){}
private:
    MYSQL m_conn;
    std::string m_url;
    std::string m_user;
    std::string m_password;
    std::string m_databasename;
};

class Mission:public Data
{
public:
    Mission(const std::string& url,const std::string& user,const std::string& password,const std::string& databasename)
    :Data(url,user,password,databasename)
    {}
    
    void show_()
    {
        std::cout<<"测试多态"<<std::endl;
    }

private:
    std::string m_mission_id; ///<任务标识 int
    std::string m_user_id; ///<用户id int
    std::string m_create_time; ///<创建时间 datatime
    std::string m_finish_time; ///<完成时间 datetime
    std::string m_position_list; ///<航线点 json
    std::string m_status; ///<任务状态 int
    std::string m_priority; ///<优先级 int
    std::string m_water_data_id_list; ///<水质检测数据 json
    std::string m_navigation_id_list; ///<航行数据表 json
    std::string m_description; ///<任务描述 varchar
    std::string m_video_id; ///<视频id int
};
}
#endif