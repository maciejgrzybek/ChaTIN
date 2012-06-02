#pragma once

#include <boost/thread/mutex.hpp>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>

#include <memory>
#include <vector>

#include "Alias.hpp"
#include "Group.hpp"
#include "Message.hpp"
#include "Subscription.hpp"
#include "Conference.hpp"
#include "ConferenceMember.hpp"

namespace dbo = Wt::Dbo;

namespace DB
{

typedef std::vector<dbo::ptr<DB::Schema::Alias> > Aliases;
typedef std::vector<dbo::ptr<DB::Schema::Subscription> > Subscriptions;

class DBDriver
{
public:
    static DBDriver* getInstance();

    /**
     * Begins transaction.
     * @return int Transaction id.
     */
    int startTransaction();

    /**
     * Ends transaction.
     * @param int Transaction id to close.
     * @param bool If true, commits changes, otherwise rollbacks.
     */
    void endTransaction(int,bool = true);

    /**
     * Stores given Alias in database.
     * @param Schema::Alias& Reference to Alias to store.
     */
    void store(Schema::Alias&);

    /**
     * Stores given Subscription in database.
     * @param Schema::Subscription& Reference to Subscription to store.
     */
    void store(Schema::Subscription&);

    /**
     * Method returns aliases collection.
     * @return Aliases Collection of aliases stored in database.
     */
    Aliases getAliases();

    /**
     * Method returns subscriptions collection.
     * @return Subscriptions Collection of subscriptions stored in database.
     */
    Subscriptions getSubscriptions();
private:
    /**
     * Opens database and creates tables, if don't exist.
     */
    DBDriver();

    dbo::backend::Sqlite3 sqlite3;
    dbo::Session session;
public:
    std::vector<std::shared_ptr<dbo::Transaction> > transactions_;

    static DBDriver* instance;
    static boost::mutex instanceLock;

};


} // namespace DB

