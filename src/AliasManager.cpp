#include "AliasManager.hpp"

AliasManager::AliasManager( const DBDriver& db ) : db(db) //FIXME
{ 
    //FIXME: There is no known DBDriver interface
}

Glib::ustring AliasManager::getAlias( const Glib::ustring& )
{
}

Glib::ustring AliasManager::getIP( const Glib::ustring& )
{

}

void AliasManager::registerAlias( 
    const Glib::ustring& alias, const Glib::ustring ip, bool trySubscribe )
{

}

void AliasManager::deleteAliasByIp( const Glib::ustring& ip )
{

}

void AliasManager::deleteAliasByAlias( const Glib::ustring& alias )
{

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
