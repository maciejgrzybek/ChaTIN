#include "ConferenceManager.hpp"

void ConferenceManager::addConference( 
        const ConferenceId& id, const std::vector< Glib::ustring >& members )
{
	std::map::iterator iter = conferences.find( id ); 
    if( iter!=conferences.end() )
    {
        throw NameAlreadyInUse( name );
	}
    else
    {
        conferences.insert( std::pair< ConferenceId, std::vector< Glib::ustring > >( id, members); 
	}
}

const std::vector<Glib::ustring>& ConferenceManager::getList( const ConferenceId& id )
{
	std::map::iterator iter = conferences.find( id ); 
    if( iter!=conferences.end() )
    {
        return iter->second; 
    }
    else
    {
        throw ConferenceNotFoundException;
    }
}

 void ConferenceManager::removeConfrence( const ConferenceId& id )
 {
	std::map::iterator iter = conferences.find( id );
    if( iter!=conferences.end() )
    {
        conferences.erase( iter ); 
    }
    else
    {
        throw ConferenceNotFoundException;
    }
}

void ConferenceManager::registerIncomingConference( 
            const ConferenceId& id, const std::vector< Glib::ustring >& members )
{
	std::map::iterator iter = conferences.find( id ); 
    if( iter!=conferences.end() )
    {
		//FIXME
	}
    else
    {
        conferences.insert( std::pair< ConferenceId, std::vector< Glib::ustring > >( id, members); 
	}
}