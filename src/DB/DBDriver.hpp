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

namespace dbo = Wt::Dbo;

namespace DB
{

typedef dbo::collection< dbo::ptr<DB::Schema::Alias> > Aliases;

class DBDriver
{
public:
    static DBDriver* getInstance();

    void store(Schema::Alias&);

    /**
     * Method returns aliases collection.
     * @return Aliases Collection of aliases stored in database.
     * TODO:
     * Could be better solution, with wrapper on dbo::colection,
     * to ensure encapsulation.
     */
    Aliases getAliases();
private:
    /**
     * Opens database and creates tables, if don't exist.
     */
    DBDriver();

    dbo::backend::Sqlite3 sqlite3;
    dbo::Session session;

    static DBDriver* instance;
    static boost::mutex instanceLock;

};


} // namespace DB

