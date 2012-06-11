#ifndef _CONFERENCE_MANAGER_HPP_
#define _CONFERENCE_MANAGER_HPP_

//#include <map>
#include <unordered_map>
#include <vector>
#include <glibmm/ustring.h>
#include "types.hpp"
#include "ConferenceException.hpp"

#include <Wt/Dbo/Dbo>
#include "DB/Conference.hpp"

namespace dbo = Wt::Dbo;

namespace DB
{
namespace Schema
{
    class Conference;
} // namespace Schema
} // namespace DB


/**
 * Class being able to create / remove conference container holding name of conference 
 * with list of its members.
 */
class ConferenceManager
{
public:
    /**
     * Method that allows to create confrence but only if ConferenceId isnt aleardy
     *  registred.
     * @throw NameDoesNotExistsException when you try to add conference with unexisting name
     * @throw NameAlreadyInUse if name was already used
     */
    void addConference( 
        const ChaTIN::ConferenceId& id, const std::vector<ChaTIN::IPv6>& members );

    /**
     * Method that can be used to find list of ipv6 in conference given by name
     * @param cosnt Glib::ustring& name of conference
     * @throw ConferenceNotFoundException when there is no conference with given name
     * @return reference to list of conference members in conference given by name in argument
     */
    std::vector<dbo::ptr<DB::Schema::ConferenceMember> > getList( const ChaTIN::ConferenceId& id ) const throw(ConferenceNotExistsException);

    /**
     * Remove conference with given name from list
     * @param const Glib::ustring& name of conference
     * @throw ConferenceNotExistsException when there is no conference with given name
     */
    void removeConference( const ChaTIN::ConferenceId& id ) throw(ConferenceNotExistsException);

    /**
     * Find if incoming name+ipv6 pair is already in use
     * If it isnt just do addConferece
     * otherwise - create new name for that and try again
     * If you find conference with same name+ipv6 pair
     *  compare vectors of members to find out if its the same conference
     *  if it is do nothing, otherwise find new name (~1,~2,~3...) for new one and add it
     */
    void registerIncomingConference( 
            const ChaTIN::ConferenceId& id, const std::vector< ChaTIN::IPv6 >& members );
    
protected:
    typedef std::unordered_map<ChaTIN::ConferenceId, dbo::ptr<DB::Schema::Conference> > conferenceMap;

    /**
     * Method returns vector of dbo::ptrs to ConferenceMember from given Conference.
     * @param dbo::ptr<DB::Schema::Conference> dbo pointer to Conference.
     * @return std::vector<dbo::ptr<DB::Schema::ConferenceMember> > Vector of dbo pointers to ConferenceMember.
     */
    std::vector<dbo::ptr<DB::Schema::ConferenceMember> > getDboFromVector(dbo::ptr<DB::Schema::Conference>) const;

    /**
     * Method registeres Conference in DB.
     * @param const ChaTIN::ConferenceId& Reference to ConferenceId to be registered in DB.
     * @param const std::vector<ChaTIN::IPv6>& const reference to vector containing conference members' IPs.
     * @return dbo::ptr<DB::Schema::Conference> dbo pointer to stored in DB object.
     */
    dbo::ptr<DB::Schema::Conference> registerConference(const ChaTIN::ConferenceId&,const std::vector<ChaTIN::IPv6>&) const;

    conferenceMap conferences;
};

#endif
