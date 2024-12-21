#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/exception.h>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <string>

class MySQLConnection {
public:
    MySQLConnection(const std::string& host, const std::string& user, const std::string& password, const std::string& db, unsigned int port) {
        driver = sql::mysql::get_mysql_driver_instance();
        conn = std::unique_ptr<sql::Connection>(driver->connect("tcp://" + host + ":" + std::to_string(port), user, password));
        conn->setSchema(db);
    }

    sql::Connection* get() {
        return conn.get();
    }

private:
    sql::mysql::MySQL_Driver* driver;
    std::unique_ptr<sql::Connection> conn;
};
