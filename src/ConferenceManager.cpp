#include "ConferenceManager.hpp"
#include "DB/DBDriver.hpp"

void ConferenceManager::addConference( 
        const ChaTIN::ConferenceId& id, const std::vector<ChaTIN::IPv6>& members )
{
    conferenceMap::const_iterator iter = conferences.find(id);
    if(iter != conferences.end())
    {
        // FIXME
        // throw NameAlreadyInUse( name );
    }
    else
    {
        conferences[id] = registerConference(id,members);
    }
}

std::vector<dbo::ptr<DB::Schema::ConferenceMember> > ConferenceManager::getList(const ChaTIN::ConferenceId& id) const throw(ConferenceNotExistsException)
{
    conferenceMap::const_iterator iter = conferences.find(id);
    if(iter != conferences.end())
    {
        return getDboFromVector(iter->second);
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
    conferenceMap::const_iterator iter = conferences.find(id);
    if(iter != conferences.end()) //TODO maybe it should throw exception if list of members isnt same
        return;

    conferences[id] = registerConference(id,members);
}

std::vector<dbo::ptr<DB::Schema::ConferenceMember> > ConferenceManager::getDboFromVector(dbo::ptr<DB::Schema::Conference> conference) const
{
    auto conferenceMembers = conference->getMembers();
    std::vector<dbo::ptr<DB::Schema::ConferenceMember> > vec;
    auto i = conferenceMembers.begin();
    auto iEnd = conferenceMembers.end();
    for(; i != iEnd; ++i)
    {
        vec.push_back(*i);
    }
    return vec;
}

dbo::ptr<DB::Schema::Conference> ConferenceManager::registerConference(const ChaTIN::ConferenceId& id,const std::vector<ChaTIN::IPv6>& members) const
{
    DB::Schema::Conference* conf = new DB::Schema::Conference(id.ownerip,id.name);
    
    auto endIter = members.end();
    for(auto iter = members.begin();iter != endIter; ++iter)
    {
        std::string ip = (Glib::ustring)(*iter);
        DB::Schema::ConferenceMember* cm = new DB::Schema::ConferenceMember(ip);
        DB::DBDriver::getInstance()->store(cm);
        conf->addConferenceMember(*cm);
    }
    
    return DB::DBDriver::getInstance()->store(conf);
}

