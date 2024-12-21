#pragma once
#include "MySQLConnection.h"

const std::string DB_HOST= "127.0.0.1";
const std::string DB_USER= "root";
const std::string DB_PASSWORD= "root";
constexpr uint32_t DB_PORT=  3306;
constexpr size_t DB_POOL_SIZE= 32;
const std::string DB_NAME= "gameserver" ;

class MySQLConnectionPool {
public:
    MySQLConnectionPool(const std::string& host, const std::string& user, const std::string& password, const std::string& db, unsigned int port, size_t poolSize)
        : host(host), user(user), password(password), db(db), port(port), poolSize(poolSize) {
        for (size_t i = 0; i < poolSize; ++i) {
            connections.push(std::make_shared<MySQLConnection>(host, user, password, db, port));
        }
    }

    std::shared_ptr<MySQLConnection> acquire() {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]() { return !connections.empty(); });
        auto conn = connections.front();
        connections.pop();
        return conn;
    }

    void release(const std::shared_ptr<MySQLConnection>& conn) {
        std::unique_lock<std::mutex> lock(mtx);
        connections.push(conn);
        cv.notify_one();
    }

private:
    std::queue<std::shared_ptr<MySQLConnection>> connections;
    std::mutex mtx;
    std::condition_variable cv;
    std::string host, user, password, db;
    unsigned int port;
    size_t poolSize;
};

