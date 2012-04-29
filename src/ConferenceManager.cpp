#include "ConferenceManager.hpp"

void ConferenceManager::addConference( 
        const ChaTIN::ConferenceId& id, const std::vector< ChaTIN::IPv6 >& members )
{
    std::map::iterator iter = conferences.find( id ); 
    if( iter!=conferences.end() )
    {
        // FIXME
        // throw NameAlreadyInUse( name );
    }
    else
    {
        conferences[ id ] = members; 
    }
}

const std::vector<ChaTIN::IPv6>& ConferenceManager::getList( const ChaTIN::ConferenceId& id )
{
    std::map::iterator iter = conferences.find( id ); 
    if( iter!=conferences.end() )
    {
        return iter->second; 
    }
    else
    {
        // FIXME
        // throw ConferenceNotFoundException;
    }
}

 void ConferenceManager::removeConfrence( const ChaTIN::ConferenceId& id )
 {
    std::map::iterator iter = conferences.find( id );
    if( iter!=conferences.end() )
    {
        conferences.erase( iter ); 
    }
    else
    {
        // FIXME
        // throw ConferenceNotFoundException;
    }
}

void ConferenceManager::registerIncomingConference( 
            const ChaTIN::ConferenceId& id, const std::vector< ChaTIN::IPv6 >& members )
{
    std::map::iterator iter = conferences.find( id ); 
    if( iter!=conferences.end() )
    {
        if ( std::equal ( iter->second.begin(), iter->second.end(), members ) ) //FIXME test it
        {
        }
        else
        {
            //FIXME unknown behavior
        }
    }
    else
    {
        conferences[ id ] = members;  
    }
}
