#include "AliasManager.hpp"
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

        result.insert( std::make_pair( alias, i.second ));
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

ChaTIN::IPv6 AliasManager::getIP( const ChaTIN::Alias& alias ) const
{
    /*BiStringMap::left_iterator*/auto iter = dictionary.left.find( alias );
    if( iter!=dictionary.left.end() )
    {
        return iter->second; 
    }
    if( Socket::Socket::isValidIP( alias ) )
    {
        return ChaTIN::IPv6(std::string(alias));
    }
    //FIXME 
    //THROW AliasDoesNotExistException  
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

void AliasManager::deleteAliasByIP( const ChaTIN::IPv6& ip )
{
    BiStringMap::right_iterator iter = dictionary.right.find( ip );
    if(iter!=dictionary.right.end())
    {
        dictionary.right.erase(iter);
        db.deleteAlias(ip); // FIXME should be surrounded by try+catch
    }
    else
    {
        //FIXME
        //THROW AliasDoesNotExistsException
    }        
}

void AliasManager::deleteAliasByAlias( const ChaTIN::Alias& alias )
{    
    BiStringMap::left_iterator iter = dictionary.left.find( alias );
    if(iter!=dictionary.left.end())
    {
        dictionary.left.erase(iter);
        db.deleteAlias(alias); // FIXME should be surrounded by try+catch
    }
    else
    {
        //FIXME
        //THROW AliasDoesNotExistsException
    }        
}

void AliasManager::requestSub( const ChaTIN::IPv6& alias )
{
    //FIXME do it like in rejectSub
    if( subscriptions[alias] == REQUESTED )
    {
        acceptSub(alias);
    }
    else
    {
        if(!sender)
        {
            return;
            //FIXME throw NoDialogManagerGivenException
        }
        XMLPackageCreator xml("iky","Can i subscribe you?");
        sender->sendTo( alias , xml.getXML() );
                                        //FIXME try catch - what if he is off AliasNotConnectedException
        subscriptions[alias] = ONE_SIDED;
    }
}

void AliasManager::acceptSub( const ChaTIN::IPv6& alias )
{
    //FIXME do it like in rejectSub
    if( subscriptions[alias] == REQUESTED )
    {
        if(!sender)
        {
            return;
            //FIXME throw NoDialogManagerGivenException
        }
        XMLPackageCreator xml("ikya","Yes you can.");
        sender->sendTo( alias , xml.getXML() );
                                        //FIXME try catch - whaat if he is off AliasNotConnectedException
        subscriptions[alias] = FULL;
    }
    else
    {
       //FIXME
       //THROW  YouAreNotRequestedException
    }    
}

void AliasManager::rejectSub( const ChaTIN::IPv6& alias )
{
    if( subscriptions.find(alias) == subscriptions.end()
        || subscriptions[alias] != REQUESTED )
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
    sender->sendTo( alias, xml.getXML() );
    //FIXME try catch - whaat if he is off AliasNotConnectedException
    subscriptions[alias] = REJECTED;
}

void AliasManager::wasRequested(const ChaTIN::IPv6& ip )
{
    if( subscriptions.find(ip) == subscriptions.end() 
        || subscriptions[ip] == NONE )
    {
        subscriptions[ip] = REQUESTED;
        return;
    }
    if( subscriptions[ip] == ONE_SIDED )
    {
        subscriptions[ip] = FULL;    
        return;
    }
}

void AliasManager::wasRejected( const ChaTIN::IPv6& alias )
{ 
    //FIXME check if exists
    if( subscriptions[alias] == ONE_SIDED )
    {
        subscriptions[alias] = REJECTED;
    }
}

void AliasManager::wasAccepted( const ChaTIN::IPv6& alias )
{
    //FIXME check if exists
    if( subscriptions[alias] == ONE_SIDED )
    {
        subscriptions[alias] = FULL;
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
    DB::Subscriptions subs;
    subs = db.getSubscriptions();
    DB::Subscriptions::const_iterator endIt = subs.end();
    for(DB::Subscriptions::const_iterator iter = subs.begin();
        iter != endIt; ++iter)
    {
        subscriptions[(*iter)->getIP()] = static_cast<SubPhase>((*iter)->getState());
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
    std::map<ChaTIN::IPv6, SubPhase>::const_iterator endIt = subscriptions.end();
    for(std::map<ChaTIN::IPv6, SubPhase>::const_iterator iter = subscriptions.begin();
        iter != endIt; ++iter)
    {
        DB::Schema::Subscription* sub = new DB::Schema::Subscription(static_cast<std::string>(iter->first),iter->second);
        db.store(sub);
    }
    db.endTransaction(transid);
}
