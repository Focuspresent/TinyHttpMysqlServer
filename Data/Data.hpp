#ifndef __DATA_HPP
#define __DATA_HPP

#include <iostream>
#include <memory>

#include "../MysqlCpp/mysql.hpp"

namespace Data
{
class Data
{
public:
    Data(const std::string& url,const std::string& user,const std::string& password,const std::string& databasename)
    :m_mysql(url,user,password,databasename)
    {
    }
    
    virtual ~Data()
    {
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

    json To_Json()
    {
        return std::move(to_json_());
    }

    void From_Json(const json& j)
    {
        from_json_(std::move(j));
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
    //将某个数据变成json
    virtual json to_json_()=0;
    //将json变成数据
    virtual void from_json_(const json& j)=0;
    
    Mysql m_mysql;
};

class Mission:public Data
{
public:
    Mission(const std::string& url,const std::string& user,const std::string& password,const std::string& databasename)
    :Data(url,user,password,databasename)
    {}
    
    void show_() override
    {
        std::cout<<"测试多态"<<std::endl;
        json j;
        int n=m_mysql.Query("SELECT * FROM mission_data",j);
        std::cout<<j<<std::endl;
        n=m_mysql.Query("SELECT misson_id,user_id FROM mission_data",j);
        std::cout<<j<<std::endl;
    }

    json to_json_() override
    {
        json j{{"video_id",m_video_id},
                {"description",m_description},
                {"navigation_id_list",m_navigation_id_list},
                {"water_data_id_list",m_water_data_id_list},
                {"priority",m_priority},
                {"status",m_status},
                {"position_list",m_position_list},
                {"finish_time",m_finish_time},
                {"create_time",m_create_time},
                {"user_id",m_user_id},
                {"mission_id"},m_mission_id};
        return std::move(j);
    }

    ///TODO
    void from_json_(const json& j) override
    {   
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