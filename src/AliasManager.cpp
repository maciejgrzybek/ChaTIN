#include "AliasManager.hpp"
#include "AliasException.hpp"
#include "XMLPackageCreator.hpp"
#include "Socket.hpp"

AliasManager::AliasManager( DB::DBDriver& db ) 
    : db(db)
{
    loadSubscriptionsFromDB();
}

void AliasManager::setDialogManager( DialogManager& sender_ )
{
    sender = sender_;
}

std::set<FriendRow> AliasManager::getAliasList()
{
    std::set<FriendRow> result;

    //REGISTERED ALIASES
    for( auto& i : dictionary.left )
    {
        result.insert( std::make_pair( i.first, NONE ) );
    }

    //REGISTERED SUBS
    for( auto& i : subscriptions)
    {
        ChaTIN::Alias alias = getAlias(i.first);
        auto it = result.find(std::make_pair(alias,NONE));
        if( it != result.end() )
            result.erase(it);

        result.insert( std::make_pair( alias, static_cast<SubPhase>(i.second->getState()) ));
    }
    return result;
}

ChaTIN::Alias AliasManager::getAlias( const ChaTIN::IPv6& ip ) const
{
    /*BiStringMap::right_iterator*/auto iter = dictionary.right.find( ip );
    if(iter!=dictionary.right.end())
    {
        return iter->second;
    }
    else
    {
        return ChaTIN::Alias(ip);
    }
}

ChaTIN::IPv6 AliasManager::getIP(const ChaTIN::Alias& alias) const
{
    /*BiStringMap::left_iterator*/auto iter = dictionary.left.find( alias );
    if(iter != dictionary.left.end())
    {
        return iter->second; 
    }
    if(Socket::Socket::isValidIP(alias))
    {
        return ChaTIN::IPv6(std::string(alias));
    }
    throw AliasDoesNotExistException(alias);
}

void AliasManager::registerAlias( 
    const ChaTIN::Alias& alias, const ChaTIN::IPv6& ip, bool trySubscribe )
{
    dictionary.insert( BiStringMap::value_type( alias, ip ) );
    if( trySubscribe )
    {
        requestSub( ip );
    }
    DB::Schema::Alias* a = new DB::Schema::Alias(alias,ip);
    db.store(a);
//    saveSubscriptionsToDB();
}

void AliasManager::deleteAliasByIP(const ChaTIN::IPv6& ip)
{
    BiStringMap::right_iterator iter = dictionary.right.find(ip);
    if(iter!=dictionary.right.end())
    {
        dictionary.right.erase(iter);
        db.deleteAlias(ip); // FIXME should be surrounded by try+catch
    }
    else
    {
        throw AliasDoesNotExistException(ip);
    }        
}

void AliasManager::deleteAliasByAlias(const ChaTIN::Alias& alias)
{    
    BiStringMap::left_iterator iter = dictionary.left.find(alias);
    if(iter != dictionary.left.end())
    {
        dictionary.left.erase(iter);
        db.deleteAlias(alias); // FIXME should be surrounded by try+catch
    }
    else
    {
        throw AliasDoesNotExistException(alias);
    }        
}

void AliasManager::requestSub(const ChaTIN::IPv6& ip)
{
    int tid = db.startTransaction(); // transaction to be sure it's saved jsut after addition
    std::map<ChaTIN::IPv6, SubDB>::const_iterator iter = subscriptions.find(ip);
    if(iter != subscriptions.end() && iter->second->getState() == REQUESTED)
    {
        acceptSub(ip);
    }
    else
    {
        if(!sender)
        {
            return;
            //FIXME throw NoDialogManagerGivenException
        }
        XMLPackageCreator xml("iky","Can i subscribe you?");
        sender->sendTo(ip, xml.getXML());
                                        //FIXME try catch - what if he is off AliasNotConnectedException
        (iter->second).modify()->setState(ONE_SIDED);
    }
    db.endTransaction(tid);
}

void AliasManager::acceptSub(const ChaTIN::IPv6& ip)
{
    std::map<ChaTIN::IPv6, SubDB>::const_iterator iter = subscriptions.find(ip);
    if(iter != subscriptions.end() && iter->second->getState() == REQUESTED)
    {
        if(!sender)
        {
            return;
            //FIXME throw NoDialogManagerGivenException
        }
        XMLPackageCreator xml("ikya","Yes you can.");
        sender->sendTo(ip, xml.getXML());
                                        //FIXME try catch - whaat if he is off AliasNotConnectedException
        (iter->second).modify()->setState(FULL);
    }
    else
    {
       //FIXME
       //THROW  YouAreNotRequestedException
    }    
}

void AliasManager::rejectSub(const ChaTIN::IPv6& ip)
{
    std::map<ChaTIN::IPv6, SubDB>::const_iterator iter = subscriptions.find(ip);
    if(iter == subscriptions.end() || iter->second->getState() != REQUESTED)
    {
        
       //FIXME
       //THROW  YouAreNotRequestedException
        return;
    }
    if(!sender)
    {
        return;
        //FIXME throw NoDialogManagerGivenException
    }
    XMLPackageCreator xml("idky","No you can't.");
    sender->sendTo(ip, xml.getXML());
    //FIXME try catch - whaat if he is off AliasNotConnectedException
    (iter->second).modify()->setState(REJECTED);
}

void AliasManager::wasRequested(const ChaTIN::IPv6& ip)
{
    std::map<ChaTIN::IPv6, SubDB>::const_iterator iter = subscriptions.find(ip);
    if(iter == subscriptions.end())
    {
        DB::Schema::Subscription* sub = new DB::Schema::Subscription(ip,REQUESTED);
        subscriptions[ip] = db.store(sub);
    }
    else if(iter->second->getState() == NONE)
    {
        (iter->second).modify()->setState(REQUESTED);
    }
    else if(iter->second->getState() == ONE_SIDED)
    {
        (iter->second).modify()->setState(FULL);
    }
}

void AliasManager::wasRejected(const ChaTIN::IPv6& ip)
{
    std::map<ChaTIN::IPv6, SubDB>::const_iterator iter = subscriptions.find(ip);
    DB::Schema::Subscription* sub = NULL;
    if(iter == subscriptions.end()) // if there was no such subscription
    {
        sub = new DB::Schema::Subscription(ip,REJECTED); // create one with REJECTED state
        subscriptions[ip] = db.store(sub);
    }
    else // otherwise change state to REJECTED
    {
        (iter->second).modify()->setState(REJECTED);
    }
}

void AliasManager::wasAccepted(const ChaTIN::IPv6& ip)
{
    std::map<ChaTIN::IPv6, SubDB>::const_iterator iter = subscriptions.find(ip);
    if(iter == subscriptions.end())
    {
        (iter->second).modify()->setState(REQUESTED);
    }
    if(iter->second->getState() == ONE_SIDED)
    {
        (iter->second).modify()->setState(FULL);
    }
}

void AliasManager::loadSubscriptionsFromDB()
{
    // get aliases
    DB::Aliases aliases;
    aliases = db.getAliases(); // there is transaction inside implementation
                               // of DB::DBDriver::getAliases()
    // TODO shouldn't bimap be erased before insertions?
    DB::Aliases::const_iterator endIter = aliases.end();
    for(DB::Aliases::const_iterator iter = aliases.begin();iter != endIter; ++iter)
    {
        typedef BiStringMap::value_type pos;
        dictionary.insert(pos((*iter)->getAlias(),(*iter)->getIP()));
    }

    // get subscriptions
    DB::Subscriptions subs = db.getSubscriptions();
    DB::Subscriptions::const_iterator endIt = subs.end();
    for(DB::Subscriptions::const_iterator iter = subs.begin();
        iter != endIt; ++iter)
    {
        subscriptions[(*iter)->getIP()] = (*iter);
    }
}

void AliasManager::saveSubscriptionsToDB()
{
    int transid = db.startTransaction();
    // save aliases
    BiStringMap::const_iterator endIter = dictionary.end();
    for(BiStringMap::const_iterator iter = dictionary.begin(); iter != endIter; ++iter)
    {
        DB::Schema::Alias* alias = new DB::Schema::Alias(iter->left,iter->right);
        db.store(alias);
    }
    db.endTransaction(transid);

    transid = db.startTransaction();
    // save subscriptions
    std::map<ChaTIN::IPv6, SubDB>::const_iterator endIt = subscriptions.end();
    for(std::map<ChaTIN::IPv6, SubDB>::const_iterator iter = subscriptions.begin();
        iter != endIt; ++iter)
    {
        DB::Schema::Subscription* sub = new DB::Schema::Subscription(static_cast<std::string>(iter->first),iter->second);
        db.store(sub);
    }
    db.endTransaction(transid);
}
