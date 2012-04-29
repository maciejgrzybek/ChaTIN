#include "AliasManager.hpp"
#include "Socket.hpp"

AliasManager::AliasManager( const DBDriver& db, const DialogManager& sender ) : db(db), sender(sender) 
{
    loadSubscriptionsFromDB();
}

Glib::ustring AliasManager::getAlias( const Glib::ustring& ip )
{
    BiStringMap::right_iterator iter = dictionary.right.find( ip );
    if(iter!=dictionary.right.end())
    {
        return iter->second;
    }
    else
    {
        return ip;
    }
}

Glib::ustring AliasManager::getIP( const Glib::ustring& alias )
{
    BiStringMap::left_iterator iter = dictionary.left.find( alias );
    if( iter!=dictionary.left.end() )
    {
        return iter->second; 
    }
    else
    {
        if( Socket::Socket::isValidIP( alias ) )
        {
            return alias;
        }
        else
        {
            //FIXME 
            //THROW AliasDoesNotExistsExeption
        }    
    }
}

void AliasManager::registerAlias( 
    const Glib::ustring& alias, const Glib::ustring ip, bool trySubscribe )
{
    dictionary.insert( BiStringMap::value_type( alias, ip ) );
    if( trySubscribe )
    {
        requestSub( alias );
    }
}

void AliasManager::deleteAliasByIp( const Glib::ustring& ip )
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

void AliasManager::deleteAliasByAlias( const Glib::ustring& alias )
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

void AliasManager::requestSub( const Glib::ustring& alias )
{
      
}

void AliasManager::acceptSub( const Glib::ustring& alias )
{

}

void AliasManager::rejectSub( const Glib::ustring& alias )
{

}

void AliasManager::wasRejected( const Glib::ustring& alias )
{

}

void AliasManager::wasAccepted( const Glib::ustring& alias )
{

}

void AliasManager::loadSubscriptionsFromDB()
{
    //FIXME - when DBDriver interface is known
}

void AliasManager::saveSubscriptionsToDB()
{
    //FIXME - when DBDriver interface is know
}
