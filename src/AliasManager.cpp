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
    else
    {
        if( Socket::Socket::isValidIP( alias ) )
        {
            return ChaTIN::IPv6(std::string(alias));
        }
        else
        {
            //FIXME 
            //THROW AliasDoesNotExistExeption
        }    
    }
}

void AliasManager::registerAlias( 
    const ChaTIN::Alias& alias, const ChaTIN::IPv6& ip, bool trySubscribe )
{
    dictionary.insert( BiStringMap::value_type( alias, ip ) );
    if( trySubscribe )
    {
        requestSub( ip );
    }
}

void AliasManager::deleteAliasByIP( const ChaTIN::IPv6& ip )
{
    BiStringMap::right_iterator iter = dictionary.right.find( ip );
    if(iter!=dictionary.right.end())
    {
        dictionary.right.erase(iter);
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
        if( !sender ) ; //FIXME throw NoDialogManagerGivenException
        XMLPackageCreator xml("iky","");
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
        if( !sender ) ; //FIXME throw NoDialogManagerGivenException
        XMLPackageCreator xml("ikya","");
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
    if( !sender ) ; //FIXME throw NoDialogManagerGivenException
    XMLPackageCreator xml("idky","");
    sender->sendTo( alias, xml.getXML() );
    //FIXME try catch - whaat if he is off AliasNotConnectedException
    subscriptions[alias] = REJECTED;
}

void AliasManager::wasRequested(const ChaTIN::IPv6& ip )
{
    if( subscriptions.find(ip) == subscriptions.end() )
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
    DB::Aliases aliases;
    aliases = db.getAliases();
    // TODO shouldn't bimap be erased before insertions?
    DB::Aliases::const_iterator endIter = aliases.end();
    for(DB::Aliases::const_iterator iter = aliases.begin();iter != endIter; ++iter)
    {
        typedef BiStringMap::value_type pos;
        dictionary.insert(pos((*iter)->getAlias(),(*iter)->getIP()));
    }
}

void AliasManager::saveSubscriptionsToDB()
{
    BiStringMap::const_iterator endIter = dictionary.end();
    for(BiStringMap::const_iterator iter = dictionary.begin(); iter != endIter; ++iter)
    {
        DB::Schema::Alias alias(iter->left,iter->right);
        db.store(alias);
    }
}
