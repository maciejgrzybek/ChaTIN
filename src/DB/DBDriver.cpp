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

dbo::ptr<Schema::Message> DBDriver::store(Schema::Message* sub)
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
    // FIXME implement this
}

void DBDriver::deleteAlias(const ChaTIN::Alias& alias)
{
    const char* alias_c = alias.c_str();
    dbo::ptr<Schema::Alias> toRemove = session.find<Schema::Alias>().where("alias = ?").bind(alias_c);
    // FIXME implement this
}

dbo::ptr<Schema::Conference> DBDriver::getConference(std::string ownerIp, std::string conferenceName)
{
    typedef dbo::collection<dbo::ptr<Schema::Conference> > dbCol;
    dbo::ptr<Schema::Conference> conference = session.find<Schema::Conference>().where("ownerIp = ?").bind(ownerIp.c_str()).where("name = ?").bind(conferenceName.c_str());
    return conference;
}

Aliases DBDriver::getAliases()
{
    typedef dbo::collection<dbo::ptr<Schema::Alias> > dbCol;
    dbo::Transaction transaction(session);
    dbCol col = session.find<Schema::Alias>();
    dbCol::const_iterator endIter = col.end();
    Aliases vec;
    for(dbCol::const_iterator iter = col.begin();iter != endIter; ++iter)
    {
        vec.push_back(*iter);
    }
    return vec;
}

Subscriptions DBDriver::getSubscriptions()
{
    typedef dbo::collection<dbo::ptr<Schema::Subscription> > dbCol;
    dbo::Transaction transaction(session);
    dbCol col = session.find<Schema::Subscription>();
    dbCol::const_iterator endIter = col.end();
    Subscriptions vec;
    for(dbCol::const_iterator iter = col.begin();iter != endIter; ++iter)
    {
        vec.push_back(*iter);
    }
    return vec;
}

Messages DBDriver::getMessages(const ChaTIN::IPv6& ip)
{
    typedef dbo::collection<dbo::ptr<Schema::Message> > dbCol;
    dbo::Transaction transaction(session);
    dbCol col = session.find<Schema::Message>().where("ip = ?").bind(ip.c_str()).orderBy("date");
    dbCol::const_iterator endIter = col.end();
    Messages vec;
    for(dbCol::const_iterator iter = col.begin();iter != endIter; ++iter)
    {
        vec.push_back(*iter);
    }
    return vec;
}

Messages DBDriver::getMessages(const ChaTIN::ConferenceId& c)
{
    typedef dbo::collection<dbo::ptr<Schema::Message> > dbCol;
    typedef dbo::ptr<Schema::Conference> dbConf;

    dbConf conf = session.find<Schema::Conference>().where("ownerIp = ?").bind(c.ownerip.c_str()).where("name = ?").bind(c.name.c_str());
    dbCol col = conf->getMessages();
    dbCol::const_iterator endIter = col.end();
    Messages vec;
    for(dbCol::const_iterator iter = col.begin();iter != endIter; ++iter)
    {
        vec.push_back(*iter);
    }
    return vec;
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

