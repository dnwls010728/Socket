#pragma once
#include "jdbc/cppconn/sqlstring.h"

namespace SQLEntity
{
    struct AccountInfo
    {
        uint32_t account_unique_id;
        sql::SQLString account_id;
        sql::SQLString account_password;
        bool account_logged_in;
        sql::SQLString created_at;
    };
}
