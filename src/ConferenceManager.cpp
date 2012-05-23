#include "ConferenceManager.hpp"

void ConferenceManager::addConference( 
        const ChaTIN::ConferenceId& id, const std::vector<ChaTIN::IPv6>& members )
{
    conferenceMap::const_iterator iter = conferences.find(id);
    if( iter!=conferences.end() )
    {
        // FIXME
        // throw NameAlreadyInUse( name );
    }
    else
    {
        conferences[id] = members;
    }
}

const std::vector<ChaTIN::IPv6>& ConferenceManager::getList( const ChaTIN::ConferenceId& id ) const throw(ConferenceNotExistsException)
{
    conferenceMap::const_iterator iter = conferences.find( id ); 
    if( iter!=conferences.end() )
    {
        return iter->second; 
    }
    else
    {
        throw ConferenceNotExistsException(id);
    }
}

void ConferenceManager::removeConference( const ChaTIN::ConferenceId& id ) throw(ConferenceNotExistsException)
{
    conferenceMap::const_iterator iter = conferences.find( id );
    if( iter!=conferences.end() )
    {
        conferences.erase( iter ); 
    }
    else
    {
        throw ConferenceNotExistsException(id);
    }
}

void ConferenceManager::registerIncomingConference( 
            const ChaTIN::ConferenceId& id, const std::vector< ChaTIN::IPv6 >& members )
{
    conferenceMap::const_iterator iter = conferences.find( id );
    if( iter!=conferences.end() ) //TODO maybe it should throw exception if list of members isnt same
        return;
    conferences[id] = members;
}
