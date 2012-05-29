#pragma once

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
    /**
     * Opens database and creates tables, if don't exist.
     */
    DBDriver();

};

} // namespace DB

