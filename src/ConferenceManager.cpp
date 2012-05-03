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
    if( iter!=conferences.end() )
    {
//        if ( std::equal ( iter->second.begin(), iter->second.end(), members ) ) //FIXME test it
        {
        }
//        else
    /*    {
    *        //FIXME check it
    *        Glib::ustring tempName;
    *        tempName = id->first.name;
    *        ChaTIN::ConferenceId = tempId;
    *        tempId.ownerip = id->first.ownerip;
    *        int maxConference = 256;
    *        for (int i = 1; i <= maxConference ; i++)
    *        {
    *            tempId.name = tempName;
    *            tempId.name += "~";
    *            tempId.name +=(Glib::ustring)i;
    *            iter = conferences.find( tempId );
    *            if( iter!=conferences.end() )
    *            {
    *            }
    *            else
    *            {
    *               if ( std::equal ( iter->second.begin(), iter->second.end(), members ) ) 
    *               {
    *                    return;
    *                }
    *            }
    */        }
        }
    }
    else
    {
        conferences[id] = members;  
    }
}
