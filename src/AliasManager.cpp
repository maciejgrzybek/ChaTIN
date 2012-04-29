#include "AliasManager.hpp"
#include "Socket.hpp"

AliasManager::AliasManager( const DBDriver& db, DialogManager& sender ) : db(db), sender(sender) 
{
    loadSubscriptionsFromDB();
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
            //FIXME change to smart cast in ChaTIN::Alias friended with ChaTIN::IPv6.
            return reinterpret_cast<const ChaTIN::IPv6&>(alias);
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
        requestSub( alias );
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

void AliasManager::requestSub( const ChaTIN::Alias& alias )
{
    if( subscriptions[alias] == REQUESTED )
    {
        acceptSub(alias);
    }
    else
    {
        sender.sendTo( alias , "IKY" ); //TODO package content
                                        //     try catch - what if he is off AliasNotConnectedException
        subscriptions[alias] = ONE_SIDED;
    }
}

void AliasManager::acceptSub( const ChaTIN::Alias& alias )
{
    if( subscriptions[alias] == REQUESTED )
    {
        sender.sendTo( alias, "IKYA" ); //TODO package content
                                        //     try catch - whaat if he is off AliasNotConnectedException
        subscriptions[alias] = FULL;
    }
    else
    {
       //FIXME
       //THROW  YouAreNotRequestedException
    }    
}

void AliasManager::rejectSub( const ChaTIN::Alias& alias )
{
    if( subscriptions[alias] == REQUESTED )
    {
        sender.sendTo( alias, "IDKY" ); //TODO package content
                                        //     try catch - whaat if he is off AliasNotConnectedException
        subscriptions[alias] = REJECTED;
    }
    else
    {
       //FIXME
       //THROW  YouAreNotRequestedException
    }    
}

void AliasManager::wasRejected( const ChaTIN::Alias& alias )
{ 
    if( subscriptions[alias] == ONE_SIDED )
    {
        subscriptions[alias] = REJECTED;
    }
}

void AliasManager::wasAccepted( const ChaTIN::Alias& alias )
{
    if( subscriptions[alias] == ONE_SIDED )
    {
        subscriptions[alias] = FULL;
    }
}

void AliasManager::loadSubscriptionsFromDB()
{
    //FIXME - when DBDriver interface is known
}

void AliasManager::saveSubscriptionsToDB()
{
    //FIXME - when DBDriver interface is know
}
