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
    session.execute("CREATE TRIGGER alias_unique BEFORE INSERT ON alias BEGIN SELECT CASE WHEN ( SELECT COUNT() as cnt FROM (SELECT alias from alias WHERE alias = new.alias) WHERE cnt > 0 ) THEN RAISE(FAIL,'Alias already exists') END; END;"); // Alias uniqueness trigger creation
  }
  //catch(::dbo::backend::Sqlite3Exception& e)
  catch(Wt::Dbo::Exception&) // TODO should catch Wt::Dbo::backend::Sqlite3Exception
  {
  }
}

dbo::ptr<Schema::Alias> DBDriver::store(Schema::Alias* alias)
{
    int transid = startTransaction();
    auto result = session.add(alias);
    endTransaction(transid);
    return result;
}

dbo::ptr<Schema::Subscription> DBDriver::store(Schema::Subscription* sub)
{
    int transid = startTransaction();
    auto result = session.add(sub);
    endTransaction(transid);
    return result;
}

void DBDriver::deleteAlias(const ChaTIN::IPv6& ip)
{
    const char* ip_c = ip.c_str();
    dbo::ptr<Schema::Alias> toRemove = session.find<Schema::Alias>().where("ip = ?").bind(ip_c);
}

void DBDriver::deleteAlias(const ChaTIN::Alias& alias)
{
    const char* alias_c = alias.c_str();
    dbo::ptr<Schema::Alias> toRemove = session.find<Schema::Alias>().where("alias = ?").bind(alias_c);
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
    if(politelty && transactions_[id]->isActive())
        transactions_[id]->commit();
    transactions_[id].reset();
}

