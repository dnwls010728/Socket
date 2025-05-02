#include "MySQLManager.h"

#include "../Helper/StringHelper.h"
#include "jdbc/cppconn/driver.h"
#include "jdbc/cppconn/exception.h"
#include "jdbc/cppconn/resultset.h"
#include "jdbc/cppconn/statement.h"

MySQLManager::MySQLManager() :
    connection_(nullptr)
{
}

bool MySQLManager::Connect(const sql::SQLString& host_name, const sql::SQLString& user, const sql::SQLString& password)
{
    try
    {
        sql::Driver* driver = get_driver_instance();
        if (!driver) return false;
        
        connection_ = std::unique_ptr<sql::Connection>(driver->connect("tcp://" + host_name, user, password));
        // connection_->setSchema("socket_db");
        connection_->setSchema("socket_db");
        
        std::cout << "SQL: " << host_name << "에 연결했습니다." << std::endl;
        return true;
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        return false;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
        return false;
    }
}

void MySQLManager::Disconnect()
{
    if (connection_)
    {
        connection_->close();
        connection_.reset();
    }
}

void MySQLManager::ExecuteQuery(const std::wstring& query, const std::function<void(const sql::ResultSet*)>& callback, const std::function<void(void)>& error_callback)
{
    if (!connection_) return;

    try
    {
        std::unique_ptr<sql::Statement> statement(connection_->createStatement());
        std::unique_ptr<sql::ResultSet> result_set(statement->executeQuery(StringHelper::ToString(query)));

        while (result_set->next())
        {
            if (callback) callback(result_set.get());
        }
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        
        if (error_callback) error_callback();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        if (error_callback) error_callback();
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
        if (error_callback) error_callback();
    }
}

int MySQLManager::ExecuteUpdate(const std::wstring& query, const std::function<void(void)>& error_callback)
{
    if (!connection_) return 0;
    
    try
    {
        std::unique_ptr<sql::Statement> statement(connection_->createStatement());
        return statement->executeUpdate(StringHelper::ToString(query));
    }
    catch (sql::SQLException& e)
    {
        std::cerr << "SQLException: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        std::cerr << "SQL State: " << e.getSQLState() << std::endl;
        
        if (error_callback) error_callback();
        return 0;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        if (error_callback) error_callback();
        return 0;
    }
    catch (...)
    {
        std::cerr << "Unknown Exception" << std::endl;
        if (error_callback) error_callback();
        return 0;
    }
}
