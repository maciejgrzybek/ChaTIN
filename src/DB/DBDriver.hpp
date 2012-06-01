#pragma once

#include <boost/thread/mutex.hpp>

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>

#include <memory>

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
typedef dbo::collection< dbo::ptr<DB::Schema::Subscription> > Subscriptions;

class DBDriver
{
public:
    static DBDriver* getInstance();

    /**
     * Begins transaction.
     */
    void startTransaction();

    /**
     * Ends transaction.
     * @param bool If true, commits changes, otherwise rollbacks.
     */
    void endTransaction(bool = true);

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
     * TODO:
     * Could be better solution, with wrapper on dbo::colection,
     * to ensure encapsulation.
     */
    Aliases getAliases();

    /**
     * Method returns subscriptions collection.
     * @return Subscriptions Collection of subscriptions stored in database.
     * TODO:
     * @see getAliases()
     */
    Subscriptions getSubscriptions();
private:
    /**
     * Opens database and creates tables, if don't exist.
     */
    DBDriver();

    dbo::backend::Sqlite3 sqlite3;
    dbo::Session session;

    std::shared_ptr<dbo::Transaction> transaction_;

    static DBDriver* instance;
    static boost::mutex instanceLock;

};


} // namespace DB

