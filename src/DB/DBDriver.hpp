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

typedef std::vector<dbo::ptr<Schema::Alias> > Aliases;
typedef std::vector<dbo::ptr<Schema::Subscription> > Subscriptions;
typedef std::vector<dbo::ptr<Schema::Message> >  Messages;

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
     * @param Schema::Alias& Pointer to Alias to store.
     * @return dbo::ptr<Schema::Alias> dbo pointer for object stored in db
     */
    dbo::ptr<Schema::Alias> store(Schema::Alias*);

    /**
     * Stores given Subscription in database.
     * @param Schema::Subscription* Pointer to Subscription to store.
     * @param dbo::ptr<Schema::Subscription> dbo pointer for object stored in db
     */
    dbo::ptr<Schema::Subscription> store(Schema::Subscription*);

    /**
     * Stores given Message in database.
     * @param Schema::Message* Pointer to Message to store.
     * @param dbo::ptr<Schema::Message> dbo pointer for object stored in db
     */
    dbo::ptr<Schema::Message> store(Schema::Message*);

    /**
     * Removes given alias from database.
     * @param const ChaTIN::IPv6& Reference to IPv6 of alias to be removed.
     */
    void deleteAlias(const ChaTIN::IPv6&);

    /**
     * Removes given alias from database.
     * @param const ChaTIN::Alias& Reference to Alias to be removed.
     */
    void deleteAlias(const ChaTIN::Alias&);

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

    /**
     * Method returns messages collection in descending order..
     * @param const std::string& IP which is part of conversation
     * @return Messages Collection of messages stored in database.
     */
    Messages getMessages(const std::string&);

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

