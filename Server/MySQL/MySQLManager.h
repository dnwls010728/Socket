#pragma once

#include <functional>

#include "../Singleton.h"
#include "jdbc/cppconn/connection.h"
#include "jdbc/cppconn/resultset.h"

class MySQLManager : public Singleton<MySQLManager>
{
public:
    MySQLManager();
    virtual ~MySQLManager() override = default;

    bool Connect(const sql::SQLString& host_name, const sql::SQLString& user, const sql::SQLString& password);

    void Disconnect();
    void ExecuteQuery(const std::wstring& query, const std::function<void(const sql::ResultSet*)>& callback = nullptr, const std::function<void(void)>& error_callback = nullptr);
    
    int ExecuteUpdate(const std::wstring& query, const std::function<void(void)>& error_callback = nullptr);

    inline sql::Connection* GetConnection() const { return connection_.get(); }

private:
    std::unique_ptr<sql::Connection> connection_;
};
