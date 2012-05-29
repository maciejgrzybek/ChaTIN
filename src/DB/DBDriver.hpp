#pragma once

#include <boost/thread/mutex.hpp>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>

#include "Alias.hpp"
#include "Group.hpp"
#include "Message.hpp"
#include "Subscription.hpp"
#include "Conference.hpp"
#include "ConferenceMember.hpp"

namespace DB
{

class DBDriver
{
public:
    static DBDriver* getInstance();
private:
    /**
     * Opens database and creates tables, if don't exist.
     */
    DBDriver();

    static DBDriver* instance;
    static boost::mutex instanceLock;

};

boost::mutex DBDriver::instanceLock;
DBDriver* DBDriver::instance = NULL;

} // namespace DB

