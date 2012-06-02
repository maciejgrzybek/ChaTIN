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
    dbo::collection< dbo::ptr<DB::Schema::Alias> > aliases
                = session.find<DB::Schema::Alias>();
    std::vector< dbo::ptr<DB::Schema::Alias> > aliasesVec;
    dbo::collection< dbo::ptr<DB::Schema::Alias> >::const_iterator
                                    iter = aliases.begin();
    dbo::collection< dbo::ptr<DB::Schema::Alias> >::const_iterator
                                    endIter = aliases.end();
    for(;iter != endIter;++iter)
    {
        aliasesVec.push_back(*iter);
    }
    return aliasesVec;
}

Subscriptions DBDriver::getSubscriptions()
{
    dbo::Transaction transaction(session);
    dbo::collection< dbo::ptr<DB::Schema::Subscription> > subs
                = session.find<DB::Schema::Subscription>();
    std::vector< dbo::ptr<DB::Schema::Subscription> > subsVec;
    dbo::collection< dbo::ptr<DB::Schema::Subscription> >::const_iterator
                                    iter = subs.begin();
    dbo::collection< dbo::ptr<DB::Schema::Subscription> >::const_iterator
                                    endIter = subs.end();
    for(;iter != endIter;++iter)
    {
        subsVec.push_back(*iter);
    }
    return subsVec;
}

int DBDriver::startTransaction()
{
    transactions_.push_back(std::shared_ptr<dbo::Transaction>(new dbo::Transaction(session)));
    return transactions_.size()-1;
}

void DBDriver::endTransaction(int id,bool politelty)
{
    if(politelty)
        transactions_[id]->commit();

    transactions_[id].reset();
}

