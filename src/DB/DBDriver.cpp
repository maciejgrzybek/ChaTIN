#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>

#include "DBDriver.hpp"

using namespace DB;

boost::mutex DBDriver::instanceLock;
DBDriver* DBDriver::instance = NULL;

DBDriver* DBDriver::getInstance()
{
    if(instance == NULL)
    {
        boost::mutex::scoped_lock lock(DBDriver::instanceLock);
        if(instance == NULL)
        {
            instance = new DBDriver();
        }
    }
    return instance;
}

DBDriver::DBDriver() : sqlite3("chatin.db")
{
  session.setConnection(sqlite3);
  session.mapClass<Schema::Alias>("alias");
  session.mapClass<Schema::Group>("group");
  session.mapClass<Schema::Message>("message");
  session.mapClass<Schema::Subscription>("subscription");
  session.mapClass<Schema::Conference>("conference");
  session.mapClass<Schema::ConferenceMember>("conferenceMember");

  // Create tables if don't exist. Otherwise - exception thrown.
  try
  {
    session.createTables();
  }
  //catch(::dbo::backend::Sqlite3Exception& e)
  catch(Wt::Dbo::Exception&) // TODO should catch Wt::Dbo::backend::Sqlite3Exception
  {
  }
}

void DBDriver::store(Schema::Alias& alias)
{
    ::dbo::ptr<Schema::Alias> a(&alias);
    session.add(a);
}

void DBDriver::store(Schema::Subscription& sub)
{
    ::dbo::ptr<Schema::Subscription> s(&sub);
    session.add(s);
}

Aliases DBDriver::getAliases()
{
    dbo::Transaction transaction(session);
    return session.find<DB::Schema::Alias>();
}

Subscriptions DBDriver::getSubscriptions()
{
    dbo::Transaction transaction(session);
    return session.find<DB::Schema::Subscription>();
}

void DBDriver::startTransaction()
{
    transaction_ = std::shared_ptr<dbo::Transaction>(new dbo::Transaction(session));
}

void DBDriver::endTransaction(bool politelty)
{
    if(politelty)
        transaction_->commit();
    transaction_.reset();
}
